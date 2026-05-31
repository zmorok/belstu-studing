#define _GNU_SOURCE
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <locale.h>

#define MAX_BUFFER 65536
#define MAX_ROW    512

typedef int BOOL;
#define TRUE  1
#define FALSE 0

static int   g_fd        = -1;
static char* g_Buffer    = NULL;
static size_t g_BufSize  = 0;

// helpers
static ssize_t WriteAll(int fd, const void* p, size_t n)
{
    const char* s = (const char*)p;
    size_t off = 0;
    while (off < n) {
        ssize_t w = write(fd, s + off, n - off);
        if (w < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        off += (size_t)w;
    }
    return (ssize_t)off;
}

static void WriteText(const char* text)
{
    if (!text) return;
    WriteAll(STDOUT_FILENO, text, strlen(text));
}

static void WriteLine(const char* text)
{
    if (!text) return;
    WriteAll(STDOUT_FILENO, text, strlen(text));
    WriteAll(STDOUT_FILENO, "\n", 1);
}

static BOOL ReadLine(char* buf, size_t max)
{
    if (!buf || max < 2) return FALSE;

    if (!fgets(buf, (int)max, stdin)) return FALSE;

    size_t n = strlen(buf);
    while (n > 0 && (buf[n - 1] == '\n' || buf[n - 1] == '\r')) {
        buf[n - 1] = 0;
        n--;
    }
    return TRUE;
}

static BOOL LoadFileToBuffer(void)
{
    if (g_fd < 0 || !g_Buffer) return FALSE;

    struct stat st;
    if (fstat(g_fd, &st) != 0) return FALSE;

    if (st.st_size < 0) return FALSE;
    if ((size_t)st.st_size >= MAX_BUFFER) return FALSE;

    if (lseek(g_fd, 0, SEEK_SET) < 0) return FALSE;

    size_t need = (size_t)st.st_size;
    size_t got = 0;

    while (got < need) {
        ssize_t r = read(g_fd, g_Buffer + got, need - got);
        if (r < 0) {
            if (errno == EINTR) continue;
            return FALSE;
        }
        if (r == 0) break;
        got += (size_t)r;
    }

    g_BufSize = got;
    g_Buffer[g_BufSize] = 0;
    return TRUE;
}

static BOOL SaveBufferToFile(void)
{
    if (g_fd < 0 || !g_Buffer) return FALSE;

    if (lseek(g_fd, 0, SEEK_SET) < 0) return FALSE;
    if (ftruncate(g_fd, 0) != 0) return FALSE;

    size_t off = 0;
    while (off < g_BufSize) {
        ssize_t w = write(g_fd, g_Buffer + off, g_BufSize - off);
        if (w < 0) {
            if (errno == EINTR) continue;
            return FALSE;
        }
        off += (size_t)w;
    }
    return TRUE;
}

static int CountRows(void)
{
    if (!g_Buffer || g_BufSize == 0) return 0;
    int n = 0;
    for (size_t i = 0; i < g_BufSize; i++) {
        if (g_Buffer[i] == '\n') n++;
    }
    if (g_Buffer[g_BufSize - 1] != '\n') n++;
    return n;
}

static BOOL FindRow(int pos, size_t* start, size_t* endNL)
{
    int rows = CountRows();
    if (rows == 0) return FALSE;

    if (pos == 0) pos = 1;
    if (pos == -1) pos = rows;
    if (pos < 1 || pos > rows) return FALSE;

    size_t s = 0;
    int row = 1;

    for (size_t i = 0; i < g_BufSize; i++) {
        if (row == pos) break;
        if (g_Buffer[i] == '\n') {
            row++;
            s = i + 1;
        }
    }

    size_t e = s;
    while (e < g_BufSize && g_Buffer[e] != '\n') e++;

    *start = s;
    *endNL = e;
    return TRUE;
}

// main operations
BOOL OpenFile(const char* path)
{
    if (!path) return FALSE;
    if (g_fd != -1) return FALSE;

    g_fd = open(path, O_RDWR);
    if (g_fd < 0) return FALSE;

    g_Buffer = (char*)calloc(MAX_BUFFER, 1);
    if (!g_Buffer) {
        close(g_fd);
        g_fd = -1;
        return FALSE;
    }

    if (!LoadFileToBuffer()) {
        free(g_Buffer);
        g_Buffer = NULL;
        close(g_fd);
        g_fd = -1;
        return FALSE;
    }

    return TRUE;
}

BOOL CloseFile(int fd)
{
    if (g_fd < 0) return FALSE;
    if (fd != g_fd) return FALSE;

    close(g_fd);
    g_fd = -1;

    free(g_Buffer);
    g_Buffer = NULL;
    g_BufSize = 0;

    return TRUE;
}

BOOL AddRow(int fd, char* row, int pos)
{
    if (g_fd < 0 || !row) return FALSE;
    if (fd != g_fd) return FALSE;

    int rows = CountRows();

    // -1, 0, 1..rows+1
    if (!(pos == -1 || pos == 0 || (pos >= 1 && pos <= rows + 1))) return FALSE;

    size_t insert = 0;

    if (pos == -1) insert = g_BufSize;        // end
    else if (pos == 0) insert = 0;            // start
    else {
        if (rows == 0) insert = 0;            
        else if (pos == rows + 1) insert = g_BufSize;
        else {
            size_t s, e;
            if (!FindRow(pos, &s, &e)) return FALSE;
            insert = s;                        // before pos
        }
    }

    size_t rowLen = strlen(row);

    size_t sepLen = 0;
    char sep[2];

    if (insert == g_BufSize && g_BufSize > 0) {
        char last = g_Buffer[g_BufSize - 1];
        if (last == '\n') sepLen = 0;
        else if (last == '\r') { sep[0] = '\n'; sepLen = 1; }
        else { sep[0] = '\r'; sep[1] = '\n'; sepLen = 2; }
    }

    size_t need = sepLen + rowLen + 2;

    if (g_BufSize + need >= MAX_BUFFER) return FALSE;

    memmove(g_Buffer + insert + need, g_Buffer + insert, g_BufSize - insert);

    size_t off = insert;

    if (sepLen) {
        memcpy(g_Buffer + off, sep, sepLen);
        off += sepLen;
    }

    memcpy(g_Buffer + off, row, rowLen);
    off += rowLen;

    g_Buffer[off++] = '\r';
    g_Buffer[off++] = '\n';

    g_BufSize += need;
    g_Buffer[g_BufSize] = 0;

    return SaveBufferToFile();
}

BOOL RemRow(int fd, int pos)
{
    if (g_fd < 0) return FALSE;
    if (fd != g_fd) return FALSE;

    size_t s, e;
    if (!FindRow(pos, &s, &e)) return FALSE;

    if (e < g_BufSize && g_Buffer[e] == '\n') e++;

    memmove(g_Buffer + s, g_Buffer + e, g_BufSize - e);
    g_BufSize -= (e - s);
    g_Buffer[g_BufSize] = 0;

    return SaveBufferToFile();
}

BOOL PrintRow(int fd, int pos)
{
    if (g_fd < 0) return FALSE;
    if (fd != g_fd) return FALSE;

    size_t s, e;
    if (!FindRow(pos, &s, &e)) return FALSE;

    size_t pe = e;
    if (pe > s && g_Buffer[pe - 1] == '\r') pe--;

    if (pe > s) {
        if (WriteAll(STDOUT_FILENO, g_Buffer + s, pe - s) < 0) return FALSE;
    }
    WriteLine("");
    return TRUE;
}

BOOL PrintRows(int fd)
{
    if (g_fd < 0) return FALSE;
    if (fd != g_fd) return FALSE;

    if (g_BufSize > 0) {
        if (WriteAll(STDOUT_FILENO, g_Buffer, g_BufSize) < 0) return FALSE;
    }
    WriteLine("");
    return TRUE;
}

int main()
{
    char path[512];
    char row[MAX_ROW];
    char tmp[32];
    int cmd, pos;

    while (1) {
        WriteText(
            "Выберите выполняемую операцию:\n"
            "1. Открыть файл.\n"
            "2. Вставить строку.\n"
            "3. Удалить строку.\n"
            "4. Вывести строку.\n"
            "5. Вывести файл.\n"
            "6. Закрыть файл.\n"
            "0. Выход.\n"
        );

        if (!ReadLine(tmp, sizeof(tmp))) break;
        cmd = atoi(tmp);

        if (cmd == 0) break;

        switch (cmd) {
        case 1:
            WriteText("Путь до файла: ");
            if (!ReadLine(path, sizeof(path))) break;
            WriteLine(path);
            if (!OpenFile(path)) WriteLine("Open failed");
            else WriteLine("Opened");
            break;

        case 2:
            WriteText("Текстовая строка: ");
            if (!ReadLine(row, sizeof(row))) break;
            WriteText("Позиция (0, -1, 1..): ");
            if (!ReadLine(tmp, sizeof(tmp))) break;
            pos = atoi(tmp);
            if (!AddRow(g_fd, row, pos)) WriteLine("AddRow failed");
            break;

        case 3:
            WriteText("Позиция (0, -1, 1..): ");
            if (!ReadLine(tmp, sizeof(tmp))) break;
            pos = atoi(tmp);
            if (!RemRow(g_fd, pos)) WriteLine("RemRow failed");
            break;

        case 4:
            WriteText("Позиция (0, -1, 1..): ");
            if (!ReadLine(tmp, sizeof(tmp))) break;
            pos = atoi(tmp);
            if (!PrintRow(g_fd, pos)) WriteLine("PrintRow failed");
            break;

        case 5:
            if (!PrintRows(g_fd)) WriteLine("PrintRows failed");
            break;

        case 6:
            if (!CloseFile(g_fd)) WriteLine("Close failed");
            else WriteLine("Closed");
            break;

        default:
            WriteLine("Unknown command");
            break;
        }
    }

    if (g_fd != -1) CloseFile(g_fd);
    return 0;
}