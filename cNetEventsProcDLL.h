//+---------------------------------------------------------------------------+
//|                                            cNetEventsProcDLL.h            |
//|                      Copyright © 2012, http://www.mql4.com/ru/users/more  |
//|                                       tradertobe@gmail.com                |
//+---------------------------------------------------------------------------+

// cNetEventsProcDLL.h

#pragma once

#define EXPFUNC __declspec(dllexport)
	
class cNetEventsProcDLL
{
public:
	static BOOL MessageDLL_PROCESS_ATTACH(void);
	static BOOL MessageDLL_PROCESS_DETACH(void);
//*******************************************************************************************************************
	static EXPFUNC int __stdcall ConnectTo(char* ps8_ServerIP, //in - ps8_ServerIP = "0123456789123456"
										   int   s32_Port,     //in 
										   int*  ph_Client);   //out - int ph_Client[1]

	static EXPFUNC int __stdcall ConnectClose(int h_Client); //in 

	static EXPFUNC int __stdcall ServerOpen(int s32_Port);   //in

	static EXPFUNC int __stdcall GetAllConnections(int* ph_Client,         // out - int ph_Client[62]
												   int* ps32_ClientIP,     // out - int ps32_ClientIP[62]
												   int* ps32_ClientCount); // out - int ps32_ClientCount[1]
	
	static EXPFUNC int __stdcall DisconnectClient(SOCKET h_Client); // in 

	static EXPFUNC int __stdcall ServerClose();

	static EXPFUNC int __stdcall SendToInt(SOCKET h_Client,		   // in
										   int*   ps32_SendBuf,	   // in
										   int    s32_SendBufLen); // in -  SendBuf[] array size in int element

	static EXPFUNC int __stdcall SendToDouble(SOCKET  h_Client,		   // in
											  double* pd_SendBuf,	   // in
											  int     s32_SendBufLen); // in -  SendBuf[] array size in double element

	static EXPFUNC int __stdcall SendToString(SOCKET h_Client,		 // in
											  char*  ps8_SendBuf,	 // in
											  INT   s32_SendBufLen); // SendBuf string size in char element

	static EXPFUNC int __stdcall ReadFromInt(SOCKET h_Client,		// in
											 int*   ps32_ReadBuf,	// in
											 int    s32_ReadBufLen,	// ReadBuf[] array size in int element
											 int*   ps32_ReadLen);	// out  - int ps32_ReadLen[1] - actual count of read data in int element

	static EXPFUNC int __stdcall ReadFromDouble(SOCKET  h_Client,		// in
												double* pd_ReadBuf,		// in
												int     s32_ReadBufLen,	// ReadBuf[] array size in double element
												int*    ps32_ReadLen);	// out - int ps32_ReadLen[1] - actual count of read data in double element

	static EXPFUNC int __stdcall ReadFromString(SOCKET h_Client,	   // in
												char*  ps8_ReadBuf,	   // in
												int    s32_ReadBufLen, // ReadBuf[] array size in char element
												int*   ps32_ReadLen);  // out - int ps32_ReadLen[1] - actual count of read data in char element
//*******************************************************************************************************************
protected:
	static DWORD SendTo(SOCKET h_Client, char* ps8_SendBuf, INT s32_SendBufLen);
	static DWORD ReadFrom(SOCKET h_Client, char* ps8_ReadBuf, INT s32_ReadBufLen, INT& s32_ReadLen);
};

