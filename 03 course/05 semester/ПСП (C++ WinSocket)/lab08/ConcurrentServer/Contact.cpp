#define _CRT_SECURE_NO_WARNINGS
#include <list>
#include "Winsock2.h"
#include "Contact.h"
Contact::Contact(TE t, const char* namesrv)
{
	memset(&prms, 0, sizeof(SOCKADDR_IN));
	lprms = sizeof(SOCKADDR_IN);
	type = t;
	sthread = WORK;
	s = INVALID_SOCKET;
	hthread = nullptr;
	htimer = nullptr;
	acceptserver = nullptr;
	connectTime = 0;
	lastActivityTime = 0;
	strcpy(srvname, namesrv);
	msg[0] = 0;
}
void Contact::SetST(ST sth, const char* m)
{
	sthread = sth;
	strcpy(msg, m);
}
