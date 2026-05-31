#define _GNU_SOURCE
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

static void WriteLine(const char* text);
static void PrintTimeT(const char* label, time_t t);

static void PrintInfo(const char* fileName);
static void PrintText(const char* fileName);

int main(int argc, char* argv[])
{
    if (argc < 2) {
        WriteLine("Usage: lab_09a_linux <file_path>\n");
        return 1;
    }

    PrintInfo(argv[1]);
    WriteLine("\n");
    PrintText(argv[1]);

    return 0;
}

static void WriteLine(const char* text)
{
    if (!text) return;

    size_t n = strlen(text);
    if (n == 0) return;

    /*(void)*/ write(STDOUT_FILENO, text, n);
}

static void PrintTimeT(const char* label, time_t t)
{
    struct tm lt;
    if (!localtime_r(&t, &lt)) return;

    char buf[128];
    int len = snprintf(buf, sizeof(buf),
                       "%s - %02d:%02d:%02d %02d.%02d.%04d\n",
                       label,
                       lt.tm_hour, lt.tm_min, lt.tm_sec,
                       lt.tm_mday, lt.tm_mon + 1, lt.tm_year + 1900);
    if (len > 0) WriteLine(buf);
}

static const char* FileTypeToStr(mode_t m)
{
    if (S_ISREG(m))  return "Regular file";
    if (S_ISDIR(m))  return "Directory";
    if (S_ISCHR(m))  return "Character device";
    if (S_ISBLK(m))  return "Block device";
    if (S_ISFIFO(m)) return "FIFO/pipe";
    if (S_ISLNK(m))  return "Symlink";
    if (S_ISSOCK(m)) return "Socket";
    return "Unknown";
}

static void PrintInfo(const char* fileName)
{
    int fd = open(fileName, O_RDONLY);
    if (fd < 0) {
        char buf[256];
        snprintf(buf, sizeof(buf), "Failed to open file: %s\n", strerror(errno));
        WriteLine(buf);
        return;
    }

    struct stat st;
    if (fstat(fd, &st) != 0) {
        char buf[256];
        snprintf(buf, sizeof(buf), "fstat failed: %s\n", strerror(errno));
        WriteLine(buf);
        close(fd);
        return;
    }

    WriteLine("File information\n");

    {
        char buf[512];
        snprintf(buf, sizeof(buf), "File name: %s\n", fileName);
        WriteLine(buf);
    }

    {
        long long size = (long long)st.st_size;

        char buf[256];
        snprintf(buf, sizeof(buf),
                 "Size: %lld B (%.2f KiB, %.2f MiB)\n",
                 size,
                 (double)size / 1024.0,
                 (double)size / (1024.0 * 1024.0));
        WriteLine(buf);
    }

    {
        char buf[256];
        snprintf(buf, sizeof(buf), "File type: %s\n", FileTypeToStr(st.st_mode));
        WriteLine(buf);
    }

    PrintTimeT("Last access time",       st.st_atime);
    PrintTimeT("Last modification time", st.st_mtime);
    PrintTimeT("Last status change time", st.st_ctime);

    close(fd);
}

static void PrintText(const char* fileName)
{
    int fd = open(fileName, O_RDONLY);
    if (fd < 0) {
        char buf[256];
        snprintf(buf, sizeof(buf), "Failed to open file: %s\n", strerror(errno));
        WriteLine(buf);
        return;
    }

    char buffer[4096];
    for (;;) {
        ssize_t r = read(fd, buffer, sizeof(buffer));
        if (r < 0) {
            char buf[256];
            snprintf(buf, sizeof(buf), "read failed: %s\n", strerror(errno));
            WriteLine(buf);
            break;
        }
        if (r == 0) break;
        (void)write(STDOUT_FILENO, buffer, (size_t)r);
    }

    WriteLine("\n");
    close(fd);
}