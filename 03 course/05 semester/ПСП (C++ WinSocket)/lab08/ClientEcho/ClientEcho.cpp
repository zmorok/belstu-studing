#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <string>

#pragma comment(lib, "WS2_32.lib")

using namespace std;

sockaddr_in GetServer(SOCKET bcSock, int broadcastPort, int serverPort, const string& discoverMsg);
sockaddr_in GetServerByName(char* name, char* call);
char* SetErrorMsgText(const char* msgText, int code);

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed\n";
        return 1;
    }

    int serverPort = 2000;
    int broadcastPort = 2000;
    string discoverMsg = "DISCOVER_SERVER_RESPONSE";
    
    char servname[256] = "dualpc";
    char buf[256] = "concurrent";

    SOCKET bcSock = socket(AF_INET, SOCK_DGRAM, 0);
    if (bcSock == INVALID_SOCKET) {
        cerr << "Broadcast socket creation failed\n";
        WSACleanup();
        return 1;
    }

    BOOL broadcast = TRUE;
    setsockopt(bcSock, SOL_SOCKET, SO_BROADCAST, (char*)&broadcast, sizeof(broadcast));

    sockaddr_in clientAddr{};
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(0);
    clientAddr.sin_addr.s_addr = INADDR_ANY;
    bind(bcSock, (sockaddr*)&clientAddr, sizeof(clientAddr));

    sockaddr_in serverAddr{};
    try {
        serverAddr = GetServerByName(servname, buf);
        //serverAddr = GetServer(bcSock, broadcastPort, serverPort, discoverMsg);
    }
    catch (const exception& ex) {
        cerr << ex.what() << "\n";
        closesocket(bcSock);
        WSACleanup();
        return 1;
    }
    closesocket(bcSock);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        cerr << "Socket creation failed\n";
        WSACleanup();
        return 1;
    }

    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Connect failed\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    cout << "Connected to server.\n";

    string command;
    cout << "Choose initial command (echo / rand / time): ";
    getline(cin, command);
    if (command != "echo" && command != "rand" && command != "time") {
        cout << "Invalid choice. Defaulting to 'echo'.\n";
        command = "echo";
    }

    int sent = send(sock, command.c_str(), (int)command.size(), 0);
    if (sent == SOCKET_ERROR) {
        cerr << "Failed to send initial command.\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    char buffer[256];
    int received = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (received > 0) {
        buffer[received] = '\0';
        cout << "Server initial reply: " << buffer << "\n";
    }
    else if (received == 0) {
        cout << "Server closed connection.\n";
        closesocket(sock);
        WSACleanup();
        return 0;
    }
    else {
        cerr << "Recv failed\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    while (true) {
        string msg;
        cout << "Enter message (or 'exit' to quit): ";
        getline(cin, msg);

        if (msg == "exit") break;

        sent = send(sock, msg.c_str(), (int)msg.size(), 0);
        if (sent == SOCKET_ERROR) {
            cerr << "Send failed\n";
            break;
        }

        received = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (received > 0) {
            buffer[received] = '\0';
            cout << "Server replied: " << buffer << "\n";
        }
        else if (received == 0) {
            cout << "Server closed connection\n";
            break;
        }
        else {
            cerr << "Recv failed\n";
            break;
        }
    }

    closesocket(sock);
    WSACleanup();
    cout << "Program terminated.\n";
    return 0;
}


sockaddr_in GetServer(SOCKET bcSock, int broadcastPort, int serverPort, const string& discoverMsg) {
    sockaddr_in broadcastAddr{};
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_port = htons(broadcastPort);
    broadcastAddr.sin_addr.s_addr = inet_addr("192.168.122.255");

    const char* query = "DISCOVER_SERVER_REQUEST";
    sendto(bcSock, query, (int)strlen(query), 0, (sockaddr*)&broadcastAddr, sizeof(broadcastAddr));

    fd_set readfds;
    timeval timeout{};
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    FD_ZERO(&readfds);
    FD_SET(bcSock, &readfds);

    sockaddr_in serverAddr{};
    int addrLen = sizeof(serverAddr);

    if (select(0, &readfds, nullptr, nullptr, &timeout) > 0) {
        char buffer[256];
        int received = recvfrom(bcSock, buffer, sizeof(buffer) - 1, 0, (sockaddr*)&serverAddr, &addrLen);
        if (received > 0) {
            buffer[received] = '\0';
            if (string(buffer) == discoverMsg) {
                cout << "Server found: " << inet_ntoa(serverAddr.sin_addr) << "\n";
                serverAddr.sin_port = htons(serverPort);
                return serverAddr;
            }
        }
    }

    throw runtime_error("Server not found via broadcast.");
}

sockaddr_in GetServerByName(char* name, char* call) {
    UNREFERENCED_PARAMETER(call);
    SOCKET cC;
    if ((cC = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) throw SetErrorMsgText("socket:", WSAGetLastError());

    DWORD timeout = 3000;
    if (setsockopt(cC, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) == SOCKET_ERROR) 
    throw SetErrorMsgText("setsockopt (timeout):", WSAGetLastError());

    hostent* host = gethostbyname(name);
    if (!host) { closesocket(cC); throw SetErrorMsgText("gethostbyname failed:", WSAGetLastError()); }

    const char* REQUEST_TEXT = "DISCOVER_SERVER_REQUEST";
    const char* RESPONSE_TEXT = "DISCOVER_SERVER_RESPONSE";

    SOCKADDR_IN serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(2000);
    memcpy(&serverAddr.sin_addr, host->h_addr, host->h_length);

    char buf[256];

    int sendLen = sendto(cC, REQUEST_TEXT, (int)strlen(REQUEST_TEXT), 0, (LPSOCKADDR)&serverAddr, sizeof(serverAddr));
    if (sendLen == SOCKET_ERROR) { closesocket(cC); throw SetErrorMsgText("sendto failed:", WSAGetLastError()); }

    while (true) {
        sockaddr_in from;
        int flen = sizeof(from);
        int recvLen = recvfrom(cC, buf, sizeof(buf) - 1, 0, (sockaddr*)&from, &flen);
        if (recvLen == SOCKET_ERROR) {
            int err = WSAGetLastError();
            if (err == WSAETIMEDOUT) {
                closesocket(cC);
                throw runtime_error("Server not found via name.");
            }
            closesocket(cC);
            throw SetErrorMsgText("recvfrom failed:", err);
        }

        buf[recvLen] = '\0';

        if (strcmp(buf, RESPONSE_TEXT) == 0) {
            printf("[Server %s responded: %s]\n", name, buf);
            closesocket(cC);
            return serverAddr;
        }
    }
}

char* SetErrorMsgText(const char* msgText, int code)
{
    static char buffer[256];
    sprintf(buffer, "%s %d", msgText, code);
    return buffer;
}
