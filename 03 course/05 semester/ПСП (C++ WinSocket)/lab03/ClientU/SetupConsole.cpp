#include <windows.h>
#include <locale>

#include "SetupConsole.h"

void SetupConsoleLocale()
{
	setlocale(LC_ALL, ".UTF-8");
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
}