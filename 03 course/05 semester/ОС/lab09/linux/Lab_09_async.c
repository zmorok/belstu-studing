#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <limits.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdatomic.h>
#include <time.h>

#define BLOCK_SIZE    (64 * 1024)
#define MAX_PARALLEL  4
#define MAX_FILES     1024

static volatile sig_atomic_t g_running = 1;

static atomic_long g_total_bytes    = 0;
static atomic_long g_copied_bytes   = 0;
static atomic_int  g_active_workers = 0;

static sem_t g_sem;

typedef struct {
    char src[PATH_MAX];
    char dst[PATH_MAX];
} coping_struct;

static void on_sigint(int sig)
{
    g_running = 0;
}

static void join_path(const char* dir, const char* name, char out[PATH_MAX])
{
    size_t dirlen = strlen(dir);
    int need = (dirlen > 0 && dir[dirlen - 1] != '/');
    if (need) snprintf(out, PATH_MAX, "%s/%s", dir, name);
    else      snprintf(out, PATH_MAX, "%s%s", dir, name);
}

static int is_dir(const char* path)
{
    struct stat st; if (stat(path, &st) != 0) return 0;
    return S_ISDIR(st.st_mode);
}

static int ensure_dir(const char* path)
{
    if (is_dir(path)) return 1;
    if (mkdir(path, 0755) == 0) return 1;
    if (errno == EEXIST) return is_dir(path);
    return 0;
}

static void draw_progress(void)
{
    long long total  = atomic_load(&g_total_bytes);
    long long copied = atomic_load(&g_copied_bytes);

    if (total <= 0) return;

    double percent = (double)copied / (double)total;
    if (percent < 0.0) percent = 0.0;
    if (percent > 1.0) percent = 1.0;

    const int width = 50;
    int pos = (int)(percent * width);

    char bar[64];
    for (int i = 0; i < width; i++) bar[i] = (i < pos) ? '#' : '-';
    bar[width] = 0;

    printf("\r[%s] %5.1f%%", bar, percent * 100.0);
    fflush(stdout);
}

static int enumerate_files_flat(const char* src_dir, char** out_names, int max_names)
{
    DIR* d = opendir(src_dir);
    if (!d) return -1;

    int count = 0;
    struct dirent* de;

    while ((de = readdir(d)) != NULL)
    {
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) continue;
        if (count >= max_names) break;

        char full[PATH_MAX];
        join_path(src_dir, de->d_name, full);

        struct stat st;
        if (lstat(full, &st) != 0) continue;
        if (!S_ISREG(st.st_mode)) continue; 

        out_names[count] = strdup(de->d_name);
        if (!out_names[count]) break;

        atomic_fetch_add(&g_total_bytes, (long long)st.st_size);
        count++;
    }

    closedir(d);
    return count;
}

static int write_all(int fd, const unsigned char* buf, int n)
{
    int off = 0;
    while (off < n) {
        ssize_t w = write(fd, buf + off, (size_t)(n - off));
        if (w < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        off += (int)w;
    }
    return off;
}

static void* copy_worker(void* p)
{
    coping_struct* ctx = (coping_struct*)p;

    int in  = open(ctx->src, O_RDONLY);
    if (in < 0) {
        free(ctx);
        sem_post(&g_sem);
        atomic_fetch_sub(&g_active_workers, 1);
        return NULL;
    }

    int out = open(ctx->dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out < 0) {
        close(in);
        free(ctx);
        sem_post(&g_sem);
        atomic_fetch_sub(&g_active_workers, 1);
        return NULL;
    }

    unsigned char buf[BLOCK_SIZE];

    while (g_running)
    {
        ssize_t r = read(in, buf, sizeof(buf));
        if (r < 0) { if (errno == EINTR) continue; break;
        }
        if (r == 0) break;
        if (write_all(out, buf, (int)r) < 0) break;
        atomic_fetch_add(&g_copied_bytes, (long long)r);
    }

    close(in);
    close(out);

    free(ctx);

    sem_post(&g_sem);
    atomic_fetch_sub(&g_active_workers, 1);
    return NULL;
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: Lab_09_async <source_dir> <dest_dir>\n");
        return 1;
    }

    const char* src_dir = argv[1];
    const char* dst_dir = argv[2];

    if (!is_dir(src_dir)) { fprintf(stderr, "Source directory does not exist\n"); return 1; }

    if (!ensure_dir(dst_dir)) { fprintf(stderr, "Failed to create destination directory: %s\n", strerror(errno)); return 1; }

    // ctrl+c
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = on_sigint;
    sigaction(SIGINT, &sa, NULL);

    if (sem_init(&g_sem, 0, MAX_PARALLEL) != 0) { fprintf(stderr, "sem_init failed\n"); return 1; }

    char* names[MAX_FILES] = {0};
    int file_count = enumerate_files_flat(src_dir, names, MAX_FILES);
    if (file_count < 0) {
        fprintf(stderr, "Failed to enumerate files: %s\n", strerror(errno));
        sem_destroy(&g_sem);
        return 1;
    }

    // 0%
    draw_progress();

    int i = 0;
    while (i < file_count && g_running)
    {
        if (sem_trywait(&g_sem) == 0)
        {
            coping_struct* ctx = (coping_struct*)calloc(1, sizeof(coping_struct));
            if (!ctx) { sem_post(&g_sem); break; }

            join_path(src_dir, names[i], ctx->src);
            join_path(dst_dir, names[i], ctx->dst);

            atomic_fetch_add(&g_active_workers, 1);

            pthread_t th;
            if (pthread_create(&th, NULL, copy_worker, ctx) == 0) pthread_detach(th);
            else { free(ctx); sem_post(&g_sem); atomic_fetch_sub(&g_active_workers, 1); }

            i++;
        }
        else
        {
            draw_progress();
            struct timespec ts = {0, 50 * 1000 * 1000}; // 50 ms
            nanosleep(&ts, NULL);
        }
    }

    while (atomic_load(&g_active_workers) > 0)
    {
        draw_progress();
        struct timespec ts = {0, 100 * 1000 * 1000}; // 100 ms
        nanosleep(&ts, NULL);
    }

    draw_progress();
    printf("\nDone.\n");

    for (int k = 0; k < file_count; k++) free(names[k]);

    sem_destroy(&g_sem);
    return 0;
}