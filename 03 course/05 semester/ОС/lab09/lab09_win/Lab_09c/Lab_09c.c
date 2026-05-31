#include <windows.h>

#define BUFFER_SIZE (64 * 1024)

void WriteLine(const wchar_t* text)
{
    DWORD written;
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), text, lstrlenW(text), &written, NULL);
}

void PrintDirectoryContent(LPCWSTR dirPath)
{
    WIN32_FIND_DATAW ffd;
    wchar_t searchPath[MAX_PATH];

    lstrcpyW(searchPath, dirPath);
    lstrcatW(searchPath, L"\\*");

    HANDLE hFind = FindFirstFileW(searchPath, &ffd);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        WriteLine(L"Ошибка чтения содержимого директории.\n");
        return;
    }

    WriteLine(L"Содержимое директории:\n");

    do
    {
        if (lstrcmpW(ffd.cFileName, L".") == 0 || lstrcmpW(ffd.cFileName, L"..") == 0) continue;

        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) WriteLine(L"[DIR] ");
        else WriteLine(L"[FILE] ");

        WriteLine(ffd.cFileName); WriteLine(L"\n");
    } while (FindNextFileW(hFind, &ffd));

    FindClose(hFind);
}

static const wchar_t* ActionToText(DWORD action)
{
    switch (action)
    {
        case FILE_ACTION_ADDED:              return L"Файл добавлен";
        case FILE_ACTION_REMOVED:            return L"Файл удалён";
        case FILE_ACTION_MODIFIED:           return L"Файл изменён";
        case FILE_ACTION_RENAMED_OLD_NAME:   return L"Старое название файла";
        case FILE_ACTION_RENAMED_NEW_NAME:   return L"Новое название файла";
        default:                             return L"Unknown event";
    }
}

static void PrintChangeWithFile(LPCWSTR dirPath, FILE_NOTIFY_INFORMATION* info)
{
    DWORD action = info->Action;

    size_t nameChars = info->FileNameLength / sizeof(WCHAR);
    WCHAR* name = (WCHAR*)malloc((nameChars + 1) * sizeof(WCHAR));
    if (!name) return;

    memcpy(name, info->FileName, info->FileNameLength);
    name[nameChars] = L'\0';

    const wchar_t* kind = L" | ? : ";

    size_t dirLen = wcslen(dirPath);
    BOOL needSlash = (dirLen > 0 && dirPath[dirLen - 1] != L'\\' && dirPath[dirLen - 1] != L'/');

    size_t fullLen = dirLen + (needSlash ? 1 : 0) + nameChars;
    WCHAR* fullPath = (WCHAR*)malloc((fullLen + 1) * sizeof(WCHAR));
    if (fullPath)
    {
        wcscpy_s(fullPath, fullLen + 1, dirPath);
        if (needSlash) wcscat_s(fullPath, fullLen + 1, L"\\");
        wcscat_s(fullPath, fullLen + 1, name);

        DWORD attr = GetFileAttributesW(fullPath);
        
        if (attr & FILE_ATTRIBUTE_DIRECTORY) kind = L" | DIR : ";
        else kind = L" | FILE: ";
        
        free(fullPath);
    }
    

    WriteLine(L"Event: ");
    WriteLine(ActionToText(action));
    WriteLine(kind);
    WriteLine(name);
    WriteLine(L"\r\n");

    free(name);
}

void MonitorDirectory(LPCWSTR dirPath)
{
    HANDLE hDir = CreateFileW(
        dirPath,
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS,
        NULL
    );

    if (hDir == INVALID_HANDLE_VALUE)
    {
        WriteLine(L"Ошибка открытия директории.\n");
        return;
    }

    BYTE buffer[BUFFER_SIZE];
    DWORD bytesReturned;

    WriteLine(L"\nМониторинг изменений в директории...\n");

    while (TRUE)
    {
        if (!ReadDirectoryChangesW(
            hDir,
            buffer,
            BUFFER_SIZE,
            FALSE,
            FILE_NOTIFY_CHANGE_FILE_NAME |
            FILE_NOTIFY_CHANGE_DIR_NAME |
            FILE_NOTIFY_CHANGE_ATTRIBUTES |
            FILE_NOTIFY_CHANGE_SIZE |
            FILE_NOTIFY_CHANGE_LAST_WRITE |
            FILE_NOTIFY_CHANGE_CREATION,
            &bytesReturned,
            NULL,
            NULL))
        {
            WriteLine(L"ReadDirectoryChangesW failed\n");
            break;
        }

        FILE_NOTIFY_INFORMATION* info = (FILE_NOTIFY_INFORMATION*)buffer;

        while (TRUE)
        {
            PrintChangeWithFile(dirPath, info);
            if (info->NextEntryOffset == 0) break;
            info = (FILE_NOTIFY_INFORMATION*)((BYTE*)info + info->NextEntryOffset);
        }
    }

    CloseHandle(hDir);
}

static int wmain(int argc, wchar_t* argv[])
{
    if (argc < 2)
    {
        WriteLine(L"Использование: Lab_09c <directory_path>\n");
        return 1;
    }
    DWORD attrs = GetFileAttributesW(argv[1]);
    if (attrs == INVALID_FILE_ATTRIBUTES || !(attrs & FILE_ATTRIBUTE_DIRECTORY))
    {
        WriteLine(L"Эта директория не существует.\n");
        return 1;
    }
    PrintDirectoryContent(argv[1]);
    MonitorDirectory(argv[1]);

    return 0;
}