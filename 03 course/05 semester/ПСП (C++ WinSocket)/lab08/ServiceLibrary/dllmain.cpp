#include "pch.h"
#include <WinSock2.h>
#include "DefineTableService.h"
#include "PrototypeService.h"
#pragma comment(lib, "WS2_32.lib")

BEGIN_TABLESERVICE
	ENTRYSERVICE("echo", EchoServer),
	ENTRYSERVICE("time", TimeServer),
	ENTRYSERVICE("rand", RandServer)
END_TABLESERVICE


extern "C" __declspec(dllexport) HANDLE SSS(char* id, LPVOID prm)
{
	HANDLE rc = NULL;
	int i = 0;
	while (i < SIZETS && strcmp(TABLESERVICE_ID(i), id) != 0) i++;
	if (i < SIZETS) rc = CreateThread(NULL, NULL, TABLESERVICE_FN(i), prm, NULL, NULL);
	return rc;
}

BOOL APIENTRY DllMain(HANDLE hinst, DWORD rcall, LPVOID wres)
{
	return TRUE;
}
