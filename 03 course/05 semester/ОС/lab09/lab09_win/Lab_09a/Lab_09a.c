#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

static void WriteLine(const char* text);
static void PrintFileTime(const char* label, FILETIME ft);
static BOOL IsTextFile(HANDLE hFile);
void PrintInfo(LPSTR FileName);
void PrintText(LPSTR FileName);

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        WriteLine("Usage: Lab_09a <file_path>\n");
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
    DWORD written;
    size_t n = strlen(text);

    if (n > 0 && text[n - 1] == '\n') {
        WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), text, (DWORD)(n - 1), &written, NULL);
        WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), "\r\n", 2, &written, NULL);
    }
    else WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), text, (DWORD)n, &written, NULL);
}



static void PrintFileTime(const char* label, FILETIME ft)
{
    SYSTEMTIME stUTC, stLocal;
    if (!FileTimeToSystemTime(&ft, &stUTC)) return;
    if (!SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal)) return;

    char buffer[128];
    wsprintfA(buffer,
        "%s - %02d:%02d:%02d %02d.%02d.%04d\r\n",
        label, stLocal.wHour, stLocal.wMinute, stLocal.wSecond, stLocal.wDay, stLocal.wMonth, stLocal.wYear);
    WriteLine(buffer);
}

static BOOL IsTextFile(HANDLE hFile)
{
    BYTE buffer[512];
    DWORD read = 0;
    LARGE_INTEGER zero = {0};
    SetFilePointerEx(hFile, zero, NULL, FILE_BEGIN);
    if (!ReadFile(hFile, buffer, sizeof(buffer), &read, NULL)) return FALSE;
    for (DWORD i = 0; i < read; i++) if (buffer[i] == 0) return FALSE;
    return TRUE;
}

void PrintInfo(LPSTR FileName)
{
    HANDLE hFile = CreateFileA(
        FileName,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        WriteLine("Failed to open file\n");
        return;
    }

    BY_HANDLE_FILE_INFORMATION info;
    if (!GetFileInformationByHandle(hFile, &info))
    {
        WriteLine("GetFileInformationByHandle failed\n");
        CloseHandle(hFile);
        return;
    }

    LARGE_INTEGER size;
    if (!GetFileSizeEx(hFile, &size))
    {
        WriteLine("GetFileSizeEx failed\n");
        CloseHandle(hFile);
        return;
    }

    char buffer[256];

    WriteLine("File information\n");

    wsprintfA(buffer, "File name: %s\n", FileName); WriteLine(buffer);

    _snprintf(buffer, sizeof(buffer),
        "Size: %lld B (%.2f KiB, %.2f MiB)\n",
        (long long)size.QuadPart,
        (double)size.QuadPart / 1024.0,
        (double)size.QuadPart / (1024.0 * 1024.0));
    WriteLine(buffer);

    DWORD type = GetFileType(hFile);
    WriteLine("File type: ");
    if (type == FILE_TYPE_DISK) WriteLine("Regular file\n");
    else if (type == FILE_TYPE_CHAR) WriteLine("Character device\n");
    else if (type == FILE_TYPE_PIPE) WriteLine("Pipe\n");
    else WriteLine("Unknown\n");

    PrintFileTime("Creation time", info.ftCreationTime);
    PrintFileTime("Last access time", info.ftLastAccessTime);
    PrintFileTime("Last modification time", info.ftLastWriteTime);

    if (IsTextFile(hFile)) WriteLine("Text file\n");
    else WriteLine("Binary file\n");

    CloseHandle(hFile);
}

void PrintText(LPSTR FileName)
{
    HANDLE hFile = CreateFileA(
        FileName,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        WriteLine("Failed to open file\n");
        return;
    }

    if (!IsTextFile(hFile))
    {
        WriteLine("The file is not a text file\n");
        CloseHandle(hFile);
        return;
    }

    SetFilePointer(hFile, 0, NULL, FILE_BEGIN);

    char buffer[1024];
    DWORD read = 0;

    while (ReadFile(hFile, buffer, sizeof(buffer), &read, NULL) && read)
    {
        DWORD written;
        WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE),
            buffer,
            read,
            &written,
            NULL);
    }

    WriteLine("\n");
    CloseHandle(hFile);
}