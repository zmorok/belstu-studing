#pragma once

#ifdef OS11_HTAPI_EXPORTS
#define OS11_HTAPI_API __declspec(dllexport)
#else
#define OS11_HTAPI_API __declspec(dllimport)
#endif

#include <Windows.h>
#include<iomanip>
#include<iostream>


#define CREATE_HTFILE_ERROR "Create HTFile error";
#define CREATE_FILEMAPPING_ERROR "Create FileMapping error";
#define MAPPING_VIEW_ERROR "Mapping view failed";


namespace HT {
	// API HT - программный интерфейс для доступа к НТ-хранилищу 
	//          НТ-хранилище предназначено для хранения данных в ОП в формате ключ/значение
	//          Персистестеность (сохранность) данных обеспечивается с помощью snapshot-менханизма 
	//          Create - создать  и открыть HT-хранилище для использования   
	//          Open   - открыть HT-хранилище для использования
	//          Insert - создать элемент данных
	//          Delete - удалить элемент данных    
	//          Get    - читать  элемент данных
	//          Update - изменить элемент данных
	//          Snap   - выпонить snapshot
	//          Close  - выполнить Snap и закрыть HT-хранилище для использования
	//          GetLastError - получить сообщение о последней ошибке


	extern "C" OS11_HTAPI_API struct HTHANDLE			// блок управления HT
	{
		OS11_HTAPI_API HTHANDLE();
		OS11_HTAPI_API HTHANDLE(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const char FileName[512]);
		int     Capacity;								// емкость хранилища в количестве элементов 
		int     SecSnapshotInterval;					// переодичность сохранения в сек. 
		int     MaxKeyLength;							// максимальная длина ключа
		int     MaxPayloadLength;						// максимальная длина данных
		char    FileName[2048];							// имя файла 
		HANDLE  File;									// File HANDLE != 0, если файл открыт
		HANDLE  FileMapping;							// Mapping File HANDLE != 0, если mapping создан  
		LPVOID  Addr;									// Addr != NULL, если mapview выполнен  
		char    LastErrorMessage[2048];					// сообщение об последней ошибке или 0x00  
		time_t  lastsnaptime;							// дата последнего snap'a (time())  
		bool	isActive;

		HANDLE mutex;
		int N;											// текущее заполнение
		HANDLE SnapshotThread;
	};

	extern "C" OS11_HTAPI_API struct Element			// элемент 
	{
		OS11_HTAPI_API Element();
		OS11_HTAPI_API Element(const void* key, int keylength);                                             // for Get
		OS11_HTAPI_API Element(const void* key, int keylength, const void* payload, int  payloadlength);    // for Insert
		OS11_HTAPI_API Element(Element* oldelement, const void* newpayload, int  newpayloadlength);         // for update
		const void* key;								// значение ключа 
		int         keylength;							// рахмер ключа
		const void* payload;							// данные 
		int         payloadlength;						// размер данных
	};

	extern "C" OS11_HTAPI_API HTHANDLE * Create			//  создать HT             
	(
		int	  Capacity,									// емкость хранилища
		int   SecSnapshotInterval,						// переодичность сохранения в сек.
		int   MaxKeyLength,								// максимальный размер ключа
		int   MaxPayloadLength,							// максимальный размер данных
		const char  FileName[512]						// имя файла 
	); 	// != NULL успешное завершение  

	extern "C" OS11_HTAPI_API HTHANDLE * Open			//  открыть HT             
	(
		const char    FileName[2048]					// имя файла 
	); 	// != NULL успешное завершение  

	extern "C" OS11_HTAPI_API HTHANDLE * OpenExistHt	//  открыть HT             
	(
		const char    FileName[512]						// имя файла 
	); 	// != NULL успешное завершение  

	extern "C" OS11_HTAPI_API BOOL Snap					// выполнить Snapshot
	(
		const HTHANDLE * hthandle						// управление HT (File, FileMapping)
	);

	extern "C" OS11_HTAPI_API BOOL Close				// Snap и закрыть HT  и  очистить HTHANDLE
	(
		const HTHANDLE * hthandle						// управление HT (File, FileMapping)
	);	//  == TRUE успешное завершение   

	extern "C" OS11_HTAPI_API BOOL CloseExistHt			// Snap и закрыть HT  и  очистить HTHANDLE
	(
		const HTHANDLE * hthandle						// управление HT (File, FileMapping)
	);	//  == TRUE успешное завершение   

	extern "C" OS11_HTAPI_API BOOL Insert				// добавить элемент в хранилище
	(
		const HTHANDLE * hthandle,						// управление HT
		const Element * element							// элемент
	);	//  == TRUE успешное завершение 


	extern "C" OS11_HTAPI_API BOOL Delete				// удалить элемент в хранилище
	(
		const HTHANDLE * hthandle,						// управление HT (ключ)
		const Element * element							// элемент 
	);	//  == TRUE успешное завершение 

	extern "C" OS11_HTAPI_API Element * Get				//  читать элемент в хранилище
	(
		const HTHANDLE * hthandle,						// управление HT
		const Element * element							// элемент 
	); 	//  != NULL успешное завершение 


	extern "C" OS11_HTAPI_API BOOL Update				//  именить элемент в хранилище
	(
		const HTHANDLE * hthandle,						// управление HT
		const Element * oldelement,						// старый элемент (ключ, размер ключа)
		const void* newpayload,							// новые данные  
		int             newpayloadlength				// размер новых данных
	); 	//  != NULL успешное завершение 

	extern "C" OS11_HTAPI_API char* GetLastErrorHt		// получить сообщение о последней ошибке
	(
		HTHANDLE * ht									// управление HT
	);

	extern "C" OS11_HTAPI_API void print				// распечатать элемент 
	(
		const Element * element							// элемент 
	);
}
