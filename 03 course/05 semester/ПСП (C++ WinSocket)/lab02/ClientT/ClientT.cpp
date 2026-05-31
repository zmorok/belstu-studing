#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <stdlib.h>

#include "SetErrorMsgText.h"
#include "SetupConsole.h"
#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib")

using namespace std;

int main()
{
	// настройка консоли и дата вод виндовс сокет
	SetupConsoleLocale();
	WSADATA wsaData;
	
	try {
		// инициализация виндовс сокет
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) throw SetErrorMsgText("WSAStartup:", WSAGetLastError());
		
		// создание сокета
		SOCKET sS;
		if ((sS = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET) throw SetErrorMsgText("серверный сокет:", WSAGetLastError());
		
		// заполнение структуры сокета сервера == куда отправляем запрос на подключение
		// адрес, порт и протокол
		// ===========================================
		SOCKADDR_IN serv;
		serv.sin_family = AF_INET;
		serv.sin_port = htons(2000);

		string ip_addr;
		cout << "ip-адрес сервера: " << endl;
		getline(cin, ip_addr);

		serv.sin_addr.s_addr = inet_addr(ip_addr.c_str());
		// ===========================================


		// подключение к серверу
		if(connect(sS, (sockaddr*)&serv, sizeof(serv)) == SOCKET_ERROR) throw SetErrorMsgText("подключение к серверу:", WSAGetLastError());
		else cout << "подключён к серверу" << endl;

		cout << "кол-во передач: ";
		int n; cin >> n;

		string msgto;
		cout << "сообщение: ";
		cin.ignore();
		getline(cin, msgto);
		msgto += " 0";

		// буферы для приёма и передачи, а также счётчики принятых и переданных байт
		char ibuf[100], obuf[100];
		strcpy_s(obuf, msgto.c_str());
		int countibuf = 0, countobuf = 0;

		// условная строка сервера
		string server = string(inet_ntoa(serv.sin_addr)) + ':' + to_string(ntohs(serv.sin_port));

		// цикл n приёмов-передачи
		for (int i = 0; i < n; i++)
		{
			// отправка
			if ((countobuf = send(sS, obuf, strlen(obuf) + 1, NULL)) == SOCKET_ERROR) throw SetErrorMsgText("send:", WSAGetLastError());
			else cout << i <<  ". отправлено байт: " << countobuf << endl;
			
			// приём
			if ((countibuf = recv(sS, ibuf, sizeof(ibuf), NULL)) == SOCKET_ERROR) throw SetErrorMsgText("recv:", WSAGetLastError());
			
			if (countibuf == 0) {
				cout << "соединение закрыто сервером";
				break;
			}
			else cout << i << ". принято (" + server + ") байт: " << countibuf << " | сообщение: " << ibuf << endl;

			// подготовка буфера для отправки
			// получение числа в конце строки и его инкремент
			// инкремент происходит в строке у КЛИЕНТА, сервер просто возвращает обратно сообщение
			string m = (string)ibuf;

			int p = m.find_last_of(' ');
			int num = stoi(m.substr(p + 1, m.length() - p - 1));
			string tmp = m.substr(0, p) + " " + to_string(++num);
			strcpy_s(obuf, tmp.c_str());

			Sleep(5);
		}
	}
	catch (string err)
	{
		cout << "!! WSAGetLastError -- " + err << endl;
	}

	cout << "Клиент завершил работу. Нажмите любую клавишу, чтобы закрыть консоль..." << endl;
	system("pause");
	return 0;
}