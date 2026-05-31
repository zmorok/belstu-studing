#define _CRT_SECURE_NO_WARNINGS
#include <list>
#include "windows.h"
#include <time.h>
#include "pch.h"

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

VOID CALLBACK ASStartMessage(ULONG_PTR pPrm);
VOID CALLBACK ASFinishMessage(ULONG_PTR pPrm);

DWORD WINAPI EchoServer(LPVOID pPrm)
{
    Contact& c = *(Contact*)pPrm;
    const HANDLE acceptThread = (c.acceptserver ? *c.acceptserver : nullptr);
    if (acceptThread)
    {
        QueueUserAPC(ASStartMessage, acceptThread, (ULONG_PTR)c.srvname);
    }
    char buffer[256];
    while (true)
    {
        int r = recv(c.s, buffer, sizeof(buffer), 0);
        if (r <= 0) break;
        if (strcmp(buffer, "exit") == 0) break;
        send(c.s, buffer, r, 0);
    }

    if (c.htimer) CancelWaitableTimer(c.htimer);
    closesocket(c.s);
    c.s = INVALID_SOCKET;
    c.sthread = Contact::FINISH;

    if (acceptThread)
    {
        QueueUserAPC(ASFinishMessage, acceptThread, (ULONG_PTR)c.srvname);
    }

    c.type = Contact::EMPTY;
    return 0;
}
DWORD WINAPI TimeServer(LPVOID pPrm)
{
    Contact& c = *(Contact*)pPrm;
    const HANDLE acceptThread = (c.acceptserver ? *c.acceptserver : nullptr);
    if (acceptThread)
    {
        QueueUserAPC(ASStartMessage, acceptThread, (ULONG_PTR)c.srvname);
    }
    char buffer[256];

    while (true)
    {
        int r = recv(c.s, buffer, sizeof(buffer) - 1, 0);
        if (r <= 0) break;

        buffer[r] = '\0';
        SYSTEMTIME st;
        GetLocalTime(&st);

        char timeStr[64];
        sprintf_s(timeStr, "%02d.%02d.%04d %02d:%02d:%02d",
            st.wDay, st.wMonth, st.wYear,
            st.wHour, st.wMinute, st.wSecond);

        send(c.s, timeStr, strlen(timeStr), 0);
    }

    if (c.htimer) CancelWaitableTimer(c.htimer);
    closesocket(c.s);
    c.s = INVALID_SOCKET;
    c.sthread = Contact::FINISH;

    if (acceptThread)
    {
        QueueUserAPC(ASFinishMessage, acceptThread, (ULONG_PTR)c.srvname);
    }

    c.type = Contact::EMPTY;
    return 0;
}
DWORD WINAPI RandServer(LPVOID pPrm)
{
    Contact& c = *(Contact*)pPrm;
    const HANDLE acceptThread = (c.acceptserver ? *c.acceptserver : nullptr);
    if (acceptThread)
    {
        QueueUserAPC(ASStartMessage, acceptThread, (ULONG_PTR)c.srvname);
    }
    srand((unsigned)time(nullptr));

    char buffer[256];
    char sendbuffer[256];
    int32_t randomNumber;

    while (true)
    {
        int r = recv(c.s, buffer, sizeof(buffer) - 1, 0);
        if (r <= 0) break;

        buffer[r] = '\0';
        if (strcmp(buffer, "rand") == 0)
        {
            randomNumber = rand();
            _itoa_s(randomNumber, sendbuffer, 10);
            send(c.s, sendbuffer, sizeof(randomNumber), 0);
        }
        else break;
    }

    if (c.htimer) CancelWaitableTimer(c.htimer);
    closesocket(c.s);
    c.s = INVALID_SOCKET;
    c.sthread = Contact::FINISH;

    if (acceptThread)
    {
        QueueUserAPC(ASFinishMessage, acceptThread, (ULONG_PTR)c.srvname);
    }

    c.type = Contact::EMPTY;
    return 0;
}
VOID CALLBACK ASStartMessage(ULONG_PTR pPrm)
{
    const char* srvname = (const char*)pPrm;
    SYSTEMTIME st;
    GetLocalTime(&st);
    printf("[%02d.%02d.%04d %02d:%02d:%02d] Server %s started\n",
        st.wDay, st.wMonth, st.wYear,
        st.wHour, st.wMinute, st.wSecond,
        srvname ? srvname : "(null)");
}

VOID CALLBACK ASFinishMessage(ULONG_PTR pPrm)
{
    const char* srvname = (const char*)pPrm;
    SYSTEMTIME st;
    GetLocalTime(&st);
    printf("[%02d.%02d.%04d %02d:%02d:%02d] Server %s finished\n",
        st.wDay, st.wMonth, st.wYear,
        st.wHour, st.wMinute, st.wSecond,
        srvname ? srvname : "(null)");
}
