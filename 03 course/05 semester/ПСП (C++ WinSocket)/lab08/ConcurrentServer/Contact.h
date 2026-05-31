#include <list>
#include "windows.h"
#define _CRT_SECURE_NO_WARNINGS

struct Contact
{
	enum TE
	{
		EMPTY,
		ACCEPT,
		CONTACT
	} type;

	enum ST
	{
		WORK,
		ABORT,
		TIMEOUT,
		FINISH
	} sthread;

	SOCKET s;
	SOCKADDR_IN prms;
	int lprms;
	HANDLE hthread;
	HANDLE htimer;
	HANDLE* acceptserver;
	time_t connectTime;
	time_t lastActivityTime;
	char msg[50];
	char srvname[15];

	Contact(TE t = EMPTY, const char* namesrv = "");
	void SetST(ST sth, const char* m = "");
};

typedef std::list<Contact> ListContact;
