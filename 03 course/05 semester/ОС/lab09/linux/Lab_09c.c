#define _GNU_SOURCE
#include <sys/inotify.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <locale.h>
#include <limits.h>

#define BUFFER_SIZE (64 * 1024)

static void WriteText(const char* s) { if (!s) return; write(STDOUT_FILENO, s, strlen(s)); }

static void WriteLine(const char* s)
{
    if (!s) return;
    write(STDOUT_FILENO, s, strlen(s));
    write(STDOUT_FILENO, "\n", 1);
}

static int IsDirPath(const char* path)
{
    struct stat st;
    if (stat(path, &st) != 0) return 0;
    return S_ISDIR(st.st_mode);
}

static void PrintDirectoryContent(const char* dirPath)
{
    DIR* d = opendir(dirPath);
    if (!d) { WriteLine("Ошибка чтения содержимого директории."); return; }

    WriteLine("Содержимое директории:");

    struct dirent* de;
    while ((de = readdir(d)) != NULL)
    {
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) continue;

        char full[PATH_MAX];
        int n = snprintf(full, sizeof(full), "%s/%s", dirPath, de->d_name);
        if (n <= 0 || (size_t)n >= sizeof(full)) continue;

        struct stat st;
        if (lstat(full, &st) == 0 && S_ISDIR(st.st_mode)) WriteText("[DIR] ");
        else WriteText("[FILE] ");

        WriteLine(de->d_name);
    }

    closedir(d);
}

static const char* MaskToText(uint32_t m)
{
    if (m & IN_CREATE)     return "Файл добавлен";
    if (m & IN_DELETE)     return "Файл удалён";
    if (m & IN_MODIFY)     return "Файл изменён";
    if (m & IN_MOVED_FROM) return "Старое название файла";
    if (m & IN_MOVED_TO)   return "Новое название файла";

    if (m & IN_ACCESS)        return "[inotify] Файл прочитан";
    if (m & IN_ATTRIB)        return "[inotify] Изменены атрибуты";
    if (m & IN_OPEN)          return "[inotify] Файл открыт";
    if (m & IN_CLOSE_WRITE)   return "[inotify] Закрыт после записи";
    if (m & IN_CLOSE_NOWRITE) return "[inotify] Закрыт без записи";
    if (m & IN_DELETE_SELF)   return "[inotify] Удалён наблюдаемый объект";
    if (m & IN_MOVE_SELF)     return "[inotify] Перемещён наблюдаемый объект";

    return "Unknown event";
}

static void PrintChangeWithFile(const char* dirPath, const struct inotify_event* ev)
{
    const char* name = (ev->len > 0 && ev->name[0]) ? ev->name : "(без имени)";

    const char* kind = " | ? : ";

    if (ev->len > 0 && ev->name[0])
    {
        char full[PATH_MAX];
        int n = snprintf(full, sizeof(full), "%s/%s", dirPath, ev->name);
        if (n > 0 && (size_t)n < sizeof(full)) {
            struct stat st;
            if (lstat(full, &st) == 0 && S_ISDIR(st.st_mode)) kind = " | DIR : ";
            else kind = " | FILE: ";
        }
    }
    else {
        if (ev->mask & IN_ISDIR) kind = " | DIR : ";
        else kind = " | FILE: ";
    }

    WriteText("Event: ");
    WriteText(MaskToText(ev->mask));
    WriteText(kind);
    WriteText(name);

    WriteLine("");
}

static void MonitorDirectory(const char* dirPath)
{
    int ifd = inotify_init1(0);
    if (ifd < 0) {
        WriteLine("Ошибка инициализации inotify.");
        return;
    }

    uint32_t mask = IN_ALL_EVENTS;

    int wd = inotify_add_watch(ifd, dirPath, mask);
    if (wd < 0) {
        WriteLine("Ошибка добавления наблюдения (inotify_add_watch).");
        close(ifd);
        return;
    }

    WriteLine("");
    WriteLine("Мониторинг изменений в директории...");

    char buffer[BUFFER_SIZE];

    for (;;)
    {
        ssize_t r = read(ifd, buffer, sizeof(buffer));
        if (r < 0) {
            if (errno == EINTR) continue;
            WriteLine("read(inotify) failed");
            break;
        }
        if (r == 0) continue;

        ssize_t off = 0;
        while (off < r)
        {
            struct inotify_event* ev = (struct inotify_event*)(buffer + off);
            PrintChangeWithFile(dirPath, ev);
            off += (ssize_t)(sizeof(struct inotify_event) + ev->len);
        }
    }

    inotify_rm_watch(ifd, wd);
    close(ifd);
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        WriteLine("Использование: Lab_09c <directory_path>");
        return 1;
    }

    if (!IsDirPath(argv[1])) {
        WriteLine("Эта директория не существует.");
        return 1;
    }

    PrintDirectoryContent(argv[1]);
    MonitorDirectory(argv[1]);
    return 0;
}