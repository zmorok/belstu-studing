#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <locale.h>
#include <stdlib.h>

#define MAX_BUFFER 65536
#define MAX_ROW    512

HANDLE g_hFile      = INVALID_HANDLE_VALUE;
CHAR*  g_Buffer     = NULL;
DWORD  g_BufferSize = 0;

// вспомогательные
static void WriteText(const char* text)
{
    DWORD w;
    if (!text) return;
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), text, lstrlenA(text), &w, NULL);
}

static void WriteLine(const char* text)
{
    DWORD w; if (!text) return;
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), text, lstrlenA(text), &w, NULL);
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), "\r\n", 2, &w, NULL);
}

static BOOL ReadLine(char* buf, DWORD max)
{
    DWORD r = 0;
    if (!ReadConsoleA(GetStdHandle(STD_INPUT_HANDLE), buf, max - 1, &r, NULL)) return FALSE;

    if (r >= 2 && buf[r - 2] == '\r' && buf[r - 1] == '\n') r -= 2;
    else if (r >= 1 && (buf[r - 1] == '\n' || buf[r - 1] == '\r')) r -= 1;

    buf[r] = 0;
    return TRUE;
}

static BOOL LoadFileToBuffer(void)
{
    LARGE_INTEGER size;
    if (!GetFileSizeEx(g_hFile, &size)) return FALSE;
    if (size.QuadPart < 0 || size.QuadPart >= MAX_BUFFER) return FALSE;

    LARGE_INTEGER zero = { 0 };
    SetFilePointerEx(g_hFile, zero, NULL, FILE_BEGIN);

    DWORD read = 0;
    if (!ReadFile(g_hFile, g_Buffer, (DWORD)size.QuadPart, &read, NULL)) return FALSE;

    g_BufferSize = read;
    g_Buffer[g_BufferSize] = 0;
    return TRUE;
}

static BOOL SaveBufferToFile()
{
    SetFilePointer(g_hFile, 0, NULL, FILE_BEGIN);
    SetEndOfFile(g_hFile);

    DWORD w = 0;
    return WriteFile(g_hFile, g_Buffer, g_BufferSize, &w, NULL);
}

static INT CountRows()
{
    if (g_BufferSize == 0) return 0;
    INT n = 0;
    for (DWORD i = 0; i < g_BufferSize; i++) if (g_Buffer[i] == '\n') n++;
    if (g_Buffer[g_BufferSize - 1] != '\n') n++;
    return n;
}

static BOOL FindRow(INT pos, DWORD* start, DWORD* endNL)
{
    INT rows = CountRows();
    if (rows == 0) return FALSE;

    if (pos == 0) pos = 1;
    if (pos == -1) pos = rows;
    if (pos < 1 || pos > rows) return FALSE;

    DWORD s = 0;
    INT row = 1;

    for (DWORD i = 0; i < g_BufferSize; i++)
    {
        if (row == pos) break;
        if (g_Buffer[i] == '\n') { row++; s = i + 1; }
    }

    DWORD e = s;
    while (e < g_BufferSize && g_Buffer[e] != '\n') e++;

    *start = s;
    *endNL = e;
    return TRUE;
}

// по заданию
BOOL OpenFile(LPSTR path)
{
    if (g_hFile != INVALID_HANDLE_VALUE) return FALSE;

    g_hFile = CreateFileA(
        path,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (g_hFile == INVALID_HANDLE_VALUE) return FALSE;

    g_Buffer = (CHAR*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, MAX_BUFFER);
    if (!g_Buffer)
    {
        CloseHandle(g_hFile);
        g_hFile = INVALID_HANDLE_VALUE;
        return FALSE;
    }

    if (!LoadFileToBuffer())
    {
        HeapFree(GetProcessHeap(), 0, g_Buffer);
        g_Buffer = NULL;
        CloseHandle(g_hFile);
        g_hFile = INVALID_HANDLE_VALUE;
        return FALSE;
    }

    return TRUE;
}

BOOL CloseFile(HANDLE hFile)
{
    if (g_hFile == INVALID_HANDLE_VALUE) return FALSE;
    if (hFile != g_hFile) return FALSE;

    CloseHandle(g_hFile);
    g_hFile = INVALID_HANDLE_VALUE;

    if (g_Buffer) HeapFree(GetProcessHeap(), 0, g_Buffer);

    g_Buffer = NULL;
    g_BufferSize = 0;
    return TRUE;
}

BOOL AddRow(HANDLE hFile, LPSTR row, INT pos)
{
    if (g_hFile == INVALID_HANDLE_VALUE || !row) return FALSE;
    if (hFile != g_hFile) return FALSE; // опционально, но логично

    INT rows = CountRows();

    // -1, 0, 1..rows+1
    if (!(pos == -1 || pos == 0 || (pos >= 1 && pos <= rows + 1))) return FALSE;

    DWORD insert = 0;

    if (pos == -1) insert = g_BufferSize;      // в конец
    else if (pos == 0) insert = 0;             // в начало
    else {
        if (rows == 0) insert = 0;             // пустой файл, pos==1
        else if (pos == rows + 1) insert = g_BufferSize; // после последней
        else {
            DWORD s, e;
            if (!FindRow(pos, &s, &e)) return FALSE;
            insert = s;                        // перед строкой pos
        }
    }

    DWORD rowLen = lstrlenA(row);

    DWORD sepLen = 0;
    CHAR  sep[2];
    if (insert == g_BufferSize && g_BufferSize > 0) {
        CHAR last = g_Buffer[g_BufferSize - 1];

        if (last == '\n') sepLen = 0;
        else if (last == '\r') { sep[0] = '\n'; sepLen = 1; }
        else { sep[0] = '\r'; sep[1] = '\n'; sepLen = 2; }
    }

    DWORD need = sepLen + rowLen + 2; // +2 за "\r\n" после добавляемой строки

    if (g_BufferSize + need >= MAX_BUFFER) return FALSE;

    MoveMemory(g_Buffer + insert + need, g_Buffer + insert, g_BufferSize - insert);
    DWORD off = insert;

    if (sepLen) {
        CopyMemory(g_Buffer + off, sep, sepLen);
        off += sepLen;
    }

    CopyMemory(g_Buffer + off, row, rowLen);
    off += rowLen;

    g_Buffer[off++] = '\r';
    g_Buffer[off++] = '\n';

    g_BufferSize += need;
    g_Buffer[g_BufferSize] = 0;

    return SaveBufferToFile();
}


BOOL RemRow(HANDLE hFile, INT pos)
{
    if (g_hFile == INVALID_HANDLE_VALUE) return FALSE;
    if (hFile != g_hFile) return FALSE;
    DWORD s, e;
    if (!FindRow(pos, &s, &e)) return FALSE;
    if (e < g_BufferSize && g_Buffer[e] == '\n') e++;
    MoveMemory(g_Buffer + s, g_Buffer + e, g_BufferSize - e);
    g_BufferSize -= (e - s);
    g_Buffer[g_BufferSize] = 0;
    return SaveBufferToFile();
}

BOOL PrintRow(HANDLE hFile, INT pos)
{
    if (g_hFile == INVALID_HANDLE_VALUE) return FALSE;
    if (hFile != g_hFile) return FALSE;
    DWORD s, e;
    if (!FindRow(pos, &s, &e)) return FALSE;
    DWORD pe = e;
    if (pe > s && g_Buffer[pe - 1] == '\r') pe--;
    DWORD w;
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), g_Buffer + s, pe - s, &w, NULL);
    WriteLine("");
    return TRUE;
}

BOOL PrintRows(HANDLE hFile)
{
    if (g_hFile == INVALID_HANDLE_VALUE) return FALSE;
    if (hFile != g_hFile) return FALSE;
    DWORD w;
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), g_Buffer, g_BufferSize, &w, NULL);
    WriteLine("");
    return TRUE;
}

int main()
{
    char path[260];
    char row[MAX_ROW];
    char tmp[32];
    int cmd, pos;

    while (1)
    {
        WriteText(
            "Выберите выполняемую операцию:\n"
            "1. Открыть файл.\n"
            "2. Вставить строку.\n"
            "3. Удалить строку.\n"
            "4. Вывести строку.\n"
            "5. Вывести файл.\n"
            "6. Закрыть файл.\n"
            "0. Выход.\n");

        if (!ReadLine(tmp, sizeof(tmp))) break;
        cmd = atoi(tmp);

        if (cmd == 0) break;

        switch (cmd)
        {
        case 1:
            WriteText("Путь до файла: ");
            ReadLine(path, sizeof(path));
            WriteLine(path);
            if (!OpenFile(path)) WriteLine("Open failed");
            else WriteLine("Opened");
            break;

        case 2:
            WriteText("Текстовая строка: ");
            ReadLine(row, sizeof(row));
            WriteText("Позиция (0, -1, 1..): ");
            ReadLine(tmp, sizeof(tmp));
            pos = atoi(tmp);
            if (!AddRow(g_hFile, row, pos)) WriteLine("AddRow failed");
            break;

        case 3:
            WriteText("Позиция (0, -1, 1..): ");
            ReadLine(tmp, sizeof(tmp));
            pos = atoi(tmp);
            if (!RemRow(g_hFile, pos)) WriteLine("RemRow failed");
            break;

        case 4:
            WriteText("Позиция (0, -1, 1..): ");
            ReadLine(tmp, sizeof(tmp));
            pos = atoi(tmp);
            if (!PrintRow(g_hFile, pos)) WriteLine("PrintRow failed");
            break;

        case 5:
            if (!PrintRows(g_hFile)) WriteLine("PrintRows failed");
            break;

        case 6:
            if (!CloseFile(g_hFile)) WriteLine("Close failed");
            else WriteLine("Closed");
            break;

        default:
            WriteLine("Unknown command");
            break;
        }
    }
    if (g_hFile != INVALID_HANDLE_VALUE) CloseFile(g_hFile);
    return 0;
}
