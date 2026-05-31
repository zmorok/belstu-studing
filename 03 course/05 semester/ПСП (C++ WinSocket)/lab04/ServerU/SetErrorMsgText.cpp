#include <string>
#include "Winsock2.h"
#include "SetErrorMsgText.h"
using namespace std;

string GetErrorMsgText(int code)    // cформировать текст ошибки 
{
	string msgText;
	switch (code)                      // проверка кода возврата  
	{
		case WSAEINTR:					msgText = "WSAEINTR:\tРабота функции прервана ";									break;
		case WSAEACCES:					msgText = "WSAEACCES:\tРазрешение отвергнуто";										break;
		case WSAEFAULT:					msgText = "WSAEFAULT:\tОшибочный адрес";											break;
		case WSAEINVAL:					msgText = "WSAEINVAL:\tОшибка в аргументе";											break;
		case WSAEMFILE:					msgText = "WSAEMFILE:\tСлишком много файлов открыто";								break;
		case WSAEWOULDBLOCK:			msgText = "WSAEWOULDBLOCK:\tРесурс временно недоступен";							break;
		case WSAEINPROGRESS:			msgText = "WSAEINPROGRESS:\tОперация в процессе развития";							break;
		case WSAEALREADY:				msgText = "WSAEALREADY:\tОперация уже выполняется";									break;
		case WSAENOTSOCK:				msgText = "WSAENOTSOCK:\tСокет задан неправильно";									break;
		case WSAEDESTADDRREQ:			msgText = "WSAEDESTADDRREQ:\tТребуется адрес расположения";							break;
		case WSAEMSGSIZE:				msgText = "WSAEMSGSIZE:\tСообщение слишком длинное";								break;
		case WSAEPROTOTYPE:				msgText = "WSAEPROTOTYPE:\tНеправильный тип протокола для сокета";					break;
		case WSAENOPROTOOPT:			msgText = "WSAENOPROTOOPT:\tОшибка в опции протокола";								break;
		case WSAEPROTONOSUPPORT:		msgText = "WSAEPROTONOSUPPORT:\tПротокол не поддерживается";						break;
		case WSAESOCKTNOSUPPORT:		msgText = "WSAESOCKTNOSUPPORT:\tТип сокета не поддерживается";						break;
		case WSAEOPNOTSUPP:				msgText = "WSAEOPNOTSUPP:\tОперация не поддерживается";								break;
		case WSAEPFNOSUPPORT:			msgText = "WSAEPFNOSUPPORT:\tТип протоколов не поддерживается";						break;
		case WSAEAFNOSUPPORT:			msgText = "WSAEAFNOSUPPORT:\tТип адресов не поддерживается протоколом";				break;
		case WSAEADDRINUSE:				msgText = "WSAEADDRINUSE:\tАдрес уже используется";									break;
		case WSAEADDRNOTAVAIL:			msgText = "WSAEADDRNOTAVAIL:\tЗапрошенный адрес не может быть использован";			break;
		case WSAENETDOWN:				msgText = "WSAENETDOWN:\tСеть отключена";											break;
		case WSAENETUNREACH:			msgText = "WSAENETUNREACH:\tСеть не достижима";										break;
		case WSAENETRESET:				msgText = "WSAENETRESET:\tСеть разорвала соединение";								break;
		case WSAECONNABORTED:			msgText = "WSAECONNABORTED:\tПрограммный отказ связи";								break;
		case WSAECONNRESET:				msgText = "WSAECONNRESET:\tСвязь восстановлена";									break;
		case WSAENOBUFS:				msgText = "WSAENOBUFS:\tНе хватает памяти для буферов";								break;
		case WSAEISCONN:				msgText = "WSAEISCONN:\tСокет уже подключен";										break;
		case WSAENOTCONN:				msgText = "WSAENOTCONN:\tСокет не подключен";										break;
		case WSAESHUTDOWN:				msgText = "WSAESHUTDOWN:\tНельзя выполнить send: сокет завершил работу";			break;
		case WSAETIMEDOUT:				msgText = "WSAETIMEDOUT:\tЗакончился отведенный интервал времени";					break;
		case WSAECONNREFUSED:			msgText = "WSAECONNREFUSED:\tСоединение отклонено";									break;
		case WSAEHOSTDOWN:				msgText = "WSAEHOSTDOWN:\tХост в неработоспособном состоянии";						break;
		case WSAEHOSTUNREACH:			msgText = "WSAEHOSTUNREACH:\tНет маршрута для хоста";								break;
		case WSAEPROCLIM:				msgText = "WSAEPROCLIM:\tСлишком много процессов";									break;
		case WSASYSNOTREADY:			msgText = "WSASYSNOTREADY:\tСеть не доступна";										break;
		case WSAVERNOTSUPPORTED:		msgText = "WSAVERNOTSUPPORTED:\tДанная версия не поддерживается";					break;
		case WSANOTINITIALISED:			msgText = "WSANOTINITIALISED:\tНе выполнена инициализация WS2_32.DLL";				break;
		case WSAEDISCON:				msgText = "WSAEDISCON:\tВыполняется отключение";									break;
		case WSATYPE_NOT_FOUND:			msgText = "WSATYPE_NOT_FOUND:\tКласс не найден";									break;
		case WSAHOST_NOT_FOUND:			msgText = "WSAHOST_NOT_FOUND:\tХост не найден";										break;
		case WSATRY_AGAIN:				msgText = "WSATRY_AGAIN:\tНеавторизированный хост не найден";						break;
		case WSANO_RECOVERY:			msgText = "WSANO_RECOVERY:\tОшибка восстановления";									break;
		case WSANO_DATA:				msgText = "WSANO_DATA:\tНет записи запрошенного типа";								break;
		case WSA_INVALID_HANDLE:		msgText = "WSA_INVALID_HANDLE:\tУказанный дескриптор события с ошибкой";			break;
		case WSA_INVALID_PARAMETER:		msgText = "WSA_INVALID_PARAMETER:\tОдин или более параметров с ошибкой";			break;
		case WSA_IO_INCOMPLETE:			msgText = "WSA_IO_INCOMPLETE:\tОбъект ввода-вывода не в сигнальном состоянии";		break;
		case WSA_IO_PENDING:			msgText = "WSA_IO_PENDING:\tОперация завершится позже";								break;
		case WSA_NOT_ENOUGH_MEMORY:		msgText = "WSA_NOT_ENOUGH_MEMORY:\tНе достаточно памяти";							break;
		case WSA_OPERATION_ABORTED:		msgText = "WSA_OPERATION_ABORTED:\tОперация отвергнута";							break;
		case WSAEINVALIDPROCTABLE:		msgText = "WSAEINVALIDPROCTABLE:\tОшибочный сервис";								break;
		case WSAEINVALIDPROVIDER:		msgText = "WSAEINVALIDPROVIDER:\tОшибка в версии сервиса";							break;
		case WSAEPROVIDERFAILEDINIT:	msgText = "WSAEPROVIDERFAILEDINIT:\tНе удалось инициализировать сервис";			break;
		case WSASYSCALLFAILURE:			msgText = "WSASYSCALLFAILURE:\tАварийное завершение системного вызова";				break;

		default:						msgText = "***ERROR***";															break;
	};
	return msgText;
};

string SetErrorMsgText(string msgText, int code)
{
	return msgText + GetErrorMsgText(code);
};