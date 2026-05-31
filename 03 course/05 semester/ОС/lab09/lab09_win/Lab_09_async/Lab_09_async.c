//#define UNICODE
//#define _UNICODE
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <wchar.h>

#define BLOCK_SIZE   65536
#define MAX_PARALLEL 4
#define MAX_FILES    1024

volatile BOOL g_Running = TRUE;
LONGLONG g_TotalBytes = 0;
LONGLONG g_CopiedBytes = 0;

HANDLE g_Semaphore = NULL;
HANDLE g_Console = NULL;

static BOOL WINAPI ConsoleHandler(DWORD signal)
{
    if (signal == CTRL_C_EVENT) { g_Running = FALSE; return TRUE; }
    return FALSE;
}

static void DrawProgress()
{
    if (g_TotalBytes <= 0) return;

    double percent = (double)g_CopiedBytes / (double)g_TotalBytes;
    if (percent < 0.0) percent = 0.0;
    if (percent > 1.0) percent = 1.0;

    const int width = 50;
    int pos = (int)(percent * width);

    wchar_t bar[64];
    for (int i = 0; i < width; i++) bar[i] = (i < pos) ? L'#' : L'-';
    bar[width] = 0;

    wchar_t line[256];
    _snwprintf_s(line, _countof(line), _TRUNCATE, L"\r[%s] %5.1f%%", bar, percent * 100.0);

    DWORD w = 0;
    WriteConsoleW(g_Console, line, (DWORD)wcslen(line), &w, NULL);
    WriteConsoleW(g_Console, L"          ", 10, &w, NULL);
}

static int EnumerateFiles(const wchar_t* dir, wchar_t** files)
{
    int count = 0;
    wchar_t mask[MAX_PATH];
    swprintf_s(mask, _countof(mask), L"%s\\*", dir);
    WIN32_FIND_DATAW fd;
    HANDLE hFind = FindFirstFileW(mask, &fd);
    if (hFind == INVALID_HANDLE_VALUE) return 0;
    do
    {
        if (wcscmp(fd.cFileName, L".") == 0 || wcscmp(fd.cFileName, L"..") == 0) continue;

        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            if (count >= MAX_FILES) break;

            size_t len = wcslen(fd.cFileName) + 1;
            files[count] = (wchar_t*)HeapAlloc(GetProcessHeap(), 0, len * sizeof(wchar_t));
            if (!files[count]) break;

            wcscpy_s(files[count], len, fd.cFileName);
            count++;
        }
    } while (FindNextFileW(hFind, &fd));

    FindClose(hFind);
    return count;
}

typedef struct coping_struct
{
    wchar_t src[MAX_PATH];
    wchar_t dst[MAX_PATH];
} coping_struct;

static DWORD WINAPI CopyFileAsync(LPVOID param)
{
    coping_struct* ctx = (coping_struct*)param;

    HANDLE hSrc = CreateFileW(
        ctx->src,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED,
        NULL
    );

    if (hSrc == INVALID_HANDLE_VALUE)
    {
        HeapFree(GetProcessHeap(), 0, ctx);
        ReleaseSemaphore(g_Semaphore, 1, NULL);
        return 1;
    }

    HANDLE hDst = CreateFileW(
        ctx->dst,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_FLAG_OVERLAPPED,
        NULL
    );

    if (hDst == INVALID_HANDLE_VALUE)
    {
        CloseHandle(hSrc);
        HeapFree(GetProcessHeap(), 0, ctx);
        ReleaseSemaphore(g_Semaphore, 1, NULL);
        return 1;
    }

    BYTE buffer[BLOCK_SIZE];
    LARGE_INTEGER offset;
    offset.QuadPart = 0;

    while (g_Running)
    {
        OVERLAPPED ovRead = { 0 };
        ovRead.Offset = offset.LowPart;
        ovRead.OffsetHigh = offset.HighPart;

        DWORD bytesRead = 0;
        BOOL r = ReadFile(hSrc, buffer, BLOCK_SIZE, NULL, &ovRead);
        if (!r)
        {
            DWORD e = GetLastError();
            if (e != ERROR_IO_PENDING) break;
        }

        if (!GetOverlappedResult(hSrc, &ovRead, &bytesRead, TRUE)) break;

        if (bytesRead == 0) break;

        OVERLAPPED ovWrite = { 0 };
        ovWrite.Offset = offset.LowPart;
        ovWrite.OffsetHigh = offset.HighPart;

        DWORD bytesWritten = 0;
        BOOL w = WriteFile(hDst, buffer, bytesRead, NULL, &ovWrite);
        if (!w)
        {
            DWORD e = GetLastError();
            if (e != ERROR_IO_PENDING) break;
        }

        if (!GetOverlappedResult(hDst, &ovWrite, &bytesWritten, TRUE))
            break;

        InterlockedAdd64(&g_CopiedBytes, bytesWritten);
        offset.QuadPart += bytesWritten;
    }

    CloseHandle(hSrc);
    CloseHandle(hDst);

    HeapFree(GetProcessHeap(), 0, ctx);
    ReleaseSemaphore(g_Semaphore, 1, NULL);
    return 0;
}

static int AnyThreadAlive(HANDLE* threads, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (!threads[i]) continue;
        DWORD wr = WaitForSingleObject(threads[i], 0);
        if (wr == WAIT_TIMEOUT) return 1;
    }
    return 0;
}

static int wmain(int argc, wchar_t* argv[])
{
    if (argc != 3)
    {
        wprintf(L"Usage: Lab_09_async <source_dir> <dest_dir>\n");
        return 1;
    }

    const wchar_t* srcDir = argv[1];
    const wchar_t* dstDir = argv[2];

    g_Console = GetStdHandle(STD_OUTPUT_HANDLE);

    DWORD attr = GetFileAttributesW(srcDir);
    if (attr == INVALID_FILE_ATTRIBUTES || !(attr & FILE_ATTRIBUTE_DIRECTORY))
    {
        wprintf(L"Source directory does not exist\n");
        return 1;
    }

    if (!CreateDirectoryW(dstDir, NULL))
    {
        DWORD e = GetLastError();
        if (e != ERROR_ALREADY_EXISTS)
        {
            wprintf(L"Failed to create destination directory (error %lu)\n", e);
            return 1;
        }
    }

    SetConsoleCtrlHandler(ConsoleHandler, TRUE);

    wchar_t* files[MAX_FILES] = { 0 };
    int fileCount = EnumerateFiles(srcDir, files);

    for (int i = 0; i < fileCount; i++)
    {
        wchar_t full[MAX_PATH];
        swprintf_s(full, _countof(full), L"%s\\%s", srcDir, files[i]);
        HANDLE h = CreateFileW(full, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
        if (h != INVALID_HANDLE_VALUE)
        {
            LARGE_INTEGER size;
            if (GetFileSizeEx(h, &size)) g_TotalBytes += size.QuadPart;
            CloseHandle(h);
        }
    }

    g_Semaphore = CreateSemaphoreW(NULL, MAX_PARALLEL, MAX_PARALLEL, NULL);
    if (!g_Semaphore)
    {
        wprintf(L"CreateSemaphore failed\n");
        for (int i = 0; i < fileCount; i++) HeapFree(GetProcessHeap(), 0, files[i]);
        return 1;
    }

    HANDLE timer = CreateWaitableTimerW(NULL, FALSE, NULL);
    LARGE_INTEGER li;
    li.QuadPart = -1000000LL;
    SetWaitableTimer(timer, &li, 100, NULL, NULL, FALSE);

    HANDLE threads[MAX_FILES] = { 0 };
    int threadCount = 0;


    DrawProgress();
    for (int i = 0; i < fileCount && g_Running; )
    {
        DWORD wr = WaitForSingleObject(g_Semaphore, 50);
        DrawProgress();

        if (wr == WAIT_OBJECT_0)
        {
            coping_struct* ctx = (coping_struct*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(coping_struct));
            if (!ctx) { ReleaseSemaphore(g_Semaphore, 1, NULL); break; }

            swprintf_s(ctx->src, _countof(ctx->src), L"%s\\%s", srcDir, files[i]);
            swprintf_s(ctx->dst, _countof(ctx->dst), L"%s\\%s", dstDir, files[i]);

            HANDLE th = CreateThread(NULL, 0, CopyFileAsync, ctx, 0, NULL);
            if (th) threads[threadCount++] = th;
            else
            {
                HeapFree(GetProcessHeap(), 0, ctx);
                ReleaseSemaphore(g_Semaphore, 1, NULL);
            }

            i++;
        }
    }

    while (AnyThreadAlive(threads, threadCount))
    {
        WaitForSingleObject(timer, INFINITE);
        DrawProgress();
    }

    DrawProgress();
    wprintf(L"\nDone.\n");

    for (int i = 0; i < threadCount; i++) if (threads[i]) CloseHandle(threads[i]);

    for (int i = 0; i < fileCount; i++) if (files[i]) HeapFree(GetProcessHeap(), 0, files[i]);

    CloseHandle(g_Semaphore);
    CloseHandle(timer);
    return 0;
}