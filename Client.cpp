//+---------------------------------------------------------------------------+
//|                                            Client.cpp                     |
//|                      Copyright © 2012, http://www.mql4.com/ru/users/more  |
//|                                       tradertobe@gmail.com                |
//+---------------------------------------------------------------------------+

// Client.cpp

#include <winsock2.h>
#pragma comment(lib, "ws2_32")
#pragma comment(lib, "NetEventsProcDLL") // NetEventsProcDLL.lib placed in ...\FastStart\Client\ 
#include <iostream>

#include  "cNetEventsProcDLL.h"
#include "RequestManager.h"

//******* JAVA SERVER
#ifndef INET_ADDRSTRLEN
#define INET_ADDRSTRLEN 16
#endif

#define SERVERLOG

#define BUFSIZE 1024                      // buffer size
#define PORT_NO 2001
//******* JAVA SERVER

// LocalIp = 0x6401a8c0 -> 192.168.1.100
// Returns a list of all local IP's on this computer (multiple IP's if multiple network adapters)
DWORD GetLocalIPs(char s8_IpList[][20], int &s32_IpCount);

/* This is the simple Client.
   It connects to Echo Server, send your input to Echo Server and read Echo from Server.

   ATTENTION !!! If your want to connect to local Server do not use  "127.0.0.1" !!!ATTENTION !!! 
   You may get all your local IP's by means of GetLocalIPs(...) function, here is call example:
   // Local IP's list
	char s8_IpList[10][20];

	int s32_IpCount;
	DWORD u32_Err = GetLocalIPs(s8_IpList, s32_IpCount);

	if (u32_Err)
	{
		printf("Get local IP's error !, no local IP means no network available...");
		return 1;
	}

	printf("\nLocal IP's list:\n");

	for (int i = 0; i<s32_IpCount; i++)
		printf("\n%s\n",s8_IpList[i]);
	//
*/

// This is the Server we want to connect to...
#define SERVER_IP   "192.168.1.101"  //this is mine local IP's get by means of GetLocalIPs(...) function,
                                   // do not use so called loopback  "127.0.0.1" address !!! 
#define PORT         7000

int  h_Client;
DWORD u32_Err;

int initConnection() {
	// Local IP's list
	char s8_IpList[10][20];

	int s32_IpCount;
	u32_Err = GetLocalIPs(s8_IpList, s32_IpCount);

	if (u32_Err)
	{
		printf("Get local IP's error !, no local IP means no network available...");
		return 1;
	}

	printf("\nLocal IP's list:\n");

	for (int i = 0; i<s32_IpCount; i++)
		printf("\n%s\n",s8_IpList[i]);
	//

	char s8_ServerIP[] = SERVER_IP;
	int  s32_Port      = PORT;
	int  ph_Client[1];

	DWORD u32_Error = cNetEventsProcDLL::ConnectTo(SERVER_IP, PORT, ph_Client);

	if (u32_Error)
	{
		printf("\nConnectTo(...) failed with error: %d\n", u32_Error);
		return 0;
	}
	else
		printf("\nConnectTo(...) OK, ph_Client[0] = : %d\n", ph_Client[0]);

	h_Client = ph_Client[0];
	return 1;
}

char  readData[200];
DWORD  readBufLen = 200;

int readSize;

double doubleBuf[1];
DWORD  doubleBufLen = 1;
int    readLen[1];
int		sendLen[1];

int intBuf[1];
DWORD  intBufLen = 1;

int initBufs() {
	sendLen[0]=1;
	return 1;
}

void printError(int errorCode) {
	switch (errorCode)
		{
		case 0:
			printf("\nOperation(...) OK");
			break;
		case ERROR_INVALID_PARAMETER:
			printf("\nERROR_INVALID_PARAMETER -> One of this parms or more: h_Client, ps8_SendData, u32_SendDataLen is invalid...\n");
			break;
		case WSAEWOULDBLOCK:
			printf("\nWSAEWOULDBLOCK -> The data will be send after the next FD_WRITE event, do nouthing\n");
			break;

		case WSA_IO_PENDING:
			printf("\nWSA_IO_PENDING -> Error: A previous Send operation is still pending. This data will not be sent, try latter\n");
			break;

		default:
			printf("\nSendTo(%d...)failed with severe error: %d\n",h_Client, errorCode);
			printf("\nConnection was closed !\n");
			break;
		};
}

int sendString(char *tomb) {
	readSize = strlen(tomb);
	for(int i=0;i<readSize;i++) {
		intBuf[0]=tomb[i];
		printf("send: *%c*",tomb[i]);
		u32_Err = cNetEventsProcDLL::SendToInt(h_Client,intBuf ,sendLen[0]);
		if(u32_Err) {
			printError(u32_Err);
			return 0;
		}
	}
	intBuf[0]='\0';
	u32_Err = cNetEventsProcDLL::SendToInt(h_Client,intBuf ,sendLen[0]);
	if(u32_Err) {
		printError(u32_Err);
		return 0;
	}
	return 1;
}

int sendInt(int number) {
	intBuf[0]=number;
	u32_Err = cNetEventsProcDLL::SendToInt(h_Client,intBuf ,sendLen[0]);
	if(u32_Err) {
		printError(u32_Err);
		return 0;
	}
	return 1;
}

int sendDouble(double number) {
	doubleBuf[0]=number;
	u32_Err = cNetEventsProcDLL::SendToDouble(h_Client,doubleBuf ,sendLen[0]);
	if(u32_Err) {
		printError(u32_Err);
		return 0;
	}
	return 1;
}

int readString() {
	int i=0;
	while(true){
		readLen[0]=0;
		while(!readLen[0]) {
			u32_Err= cNetEventsProcDLL::ReadFromInt(h_Client,intBuf,intBufLen,readLen);		
			if(u32_Err) {
				printError(u32_Err);
				return 0;
			}
		}
		readData[i++]=intBuf[0];
		if(intBuf[0]=='\0') {
			return 1;
		}
	}
}

int readInt() {
	readLen[0]=0;
	while(!readLen[0]) {
		u32_Err= cNetEventsProcDLL::ReadFromInt(h_Client,intBuf,intBufLen,readLen);	
		if(u32_Err) {
			printError(u32_Err);
			return 0;
		}
	}
	return 1;
}

int readDouble() {
	readLen[0]=0;
	while(!readLen[0]) {
		u32_Err= cNetEventsProcDLL::ReadFromDouble(h_Client,doubleBuf,intBufLen,readLen);	
		if(u32_Err) {
			printError(u32_Err);
			return 0;
		}
	}
	return 1;
}

//******* JAVA SERVER VARIABLES
/*alap*/
	/* Declarations */
   int fd;	        	           		// socket endpt
   int fdc[2];                        	// socket endpt
   int flags;                      	// rcv flags
   struct sockaddr_in server;      	// socket name (addr) of server
   struct sockaddr_in client[2];	     	// socket name of client
   int server_size;                	// length of the socket addr. server
   int client_size;                	// length of the socket addr. client
   int bytes;		           					// length of buffer 
   int rcvsize[2];                    	// received bytes
   int trnmsize;                   	// transmitted bytes
   int err;                        	// error code
   char on;                        	// 
   char buffer[2][BUFSIZE+1];   // input buffer  
	char outbuffer[BUFSIZE+1];	//output buffer
	WSADATA WsaDat;
//******* JAVA SERVER VARIABLES

int initJavaServer() {
	if(WSAStartup(MAKEWORD(2,2), &WsaDat) != 0)
	{
		printf("WSA Initialization failed!\r\n");
		WSACleanup();
		system("PAUSE");
		return 0;
	}

	/* Initialization */
   on                     = 1;
   flags                  = 0;
   bytes                  = BUFSIZE;
   server_size            = sizeof server;
   client_size            = sizeof client[0];
   server.sin_family      = AF_INET;
   server.sin_addr.s_addr = INADDR_ANY;
   server.sin_port        = htons(PORT_NO);
	
	printf("SERVER STARTED\n");
   /* Creating socket */
   fd = socket(AF_INET, SOCK_STREAM, 0 );
   if (fd < 0) {
      fprintf(stderr, "Socket creation error\n");
      exit(1);
	}

   /* Setting socket options */
   setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
   setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

   /* Binding socket */
   err = bind(fd, (struct sockaddr *) &server, server_size);
   if (err < 0) {
      fprintf(stderr, "Cannot bind to the socket\n");
      exit(2);
	}

   /* Listening */
   err = listen(fd, 10);
   if (err < 0) {
      fprintf(stderr," Cannot listen to the socket\n");
      exit(3);
	}


   //wait for java client
   fdc[0] = accept(fd, (struct sockaddr *) &client[0], &client_size);
	if (fdc < 0) {
		fprintf(stderr, "Cannot accept on socket\n");
		exit(4);
	}
}

int main()
{
	initJavaServer();

	outbuffer[0]='\0';
	strcat(outbuffer,"wait\n");
	bytes = strlen(outbuffer);
	trnmsize = send(fdc[0], outbuffer, bytes, flags);
	if (trnmsize < 0) {
		fprintf(stderr, "%s: Cannot send data to the client.\n",outbuffer);
	}

	while(strcmp("END", buffer[0])!=0) {
		rcvsize[0] = recv( fdc[0], buffer[0], 1024, flags );
				if (rcvsize[0] < 0 || rcvsize[1] < 0) {
					fprintf(stderr,"Cannot receive from the socket\n");
					exit(0);
				}
				printf("%s: read.\n",buffer);
	}
	Sleep(5000);
	WSACleanup();


	if(!initConnection()) {
		printf("Init failed.");
		Sleep(1000);
		return 0;
	}

	if(!initBufs()){
		printf("InitBufs failed.");
		return 0;
	}
	// Connection is established successfully ! Let's  have some SendTo(...) data on this connection...
	

	while(true)
	{
		std::cout << "\nEnter something to send to Server or quit 'q':\n" << std::endl;

		std::cin.getline(readData, 200);

		readSize = strlen(readData);

		if (readData[0] == 'q')
			break;
		printf("\nReadFrom: %s\n", readData);
		if (sendString(readData))
		{
			int ReadLen = 1;

			while(!ReadLen)
			{
				//u32_Error = cNetEventsProcDLL::ReadFromString(h_Client, ps8_ReadBuf, s32_ReadBufLen, ps32_ReadLen);
				
				if (u32_Err)
				{
					printf("\nReadFromString(%d...) failed with error: %d\n", h_Client, u32_Err);
					break;
				}

				//ReadLen = ps32_ReadLen[0];
			}
			if (u32_Err)
			{
				printf("\nReadFromString(%d...) failed with error: %d\n", h_Client, u32_Err);
			}
			else
			{
				printf("\nReadFromString(%d...) OK, read %d  bytes\n", h_Client, ReadLen);
			}
				
			if (ReadLen > 0)
			{
				//CharToOemBuff(ps8_ReadBuf, ps8_ReadBuf, s32_SendDataLen);
				//ps8_ReadBuf[ReadLen] = 0;
				//printf("\nReadFromString(%d...) Read Data: %s\n", h_Client, ps8_ReadBuf);
			}

		}

	}

	u32_Err = cNetEventsProcDLL::ConnectClose(h_Client);

	if (u32_Err)
		printf("\nConnectClose(...) failed with error: %d\n", u32_Err);
	else
		printf("\nConnectClose(...) OK.\n");

	Sleep(5000);

	return 0;
}

// LocalIp = 0x6401a8c0 -> 192.168.1.100
// Returns a list of all local IP's on this computer (multiple IP's if multiple network adapters)
DWORD GetLocalIPs(char s8_IpList[][20], int &s32_IpCount)
{
	// Winsock version 2.0 is available on ALL Windows operating systems 
	// except Windows 95 which comes with Winsock 1.1
	WSADATA k_Data;
	DWORD u32_Error = WSAStartup(MAKEWORD(2,0), &k_Data);
	if (u32_Error)
		return u32_Error;

	int ps32_IpList[20];

	char s8_Host[500];
	if (gethostname(s8_Host, sizeof(s8_Host)) == SOCKET_ERROR)
		return WSAGetLastError();
	
	struct hostent* pk_Host = gethostbyname(s8_Host);
	if (!pk_Host)
		return WSAGetLastError();

	s32_IpCount = 0;

	for (DWORD i=0; TRUE; i++)
	{
		if (!pk_Host->h_addr_list[i])
			break; // The IP list is zero terminated

		ps32_IpList[i] = *((DWORD*)pk_Host->h_addr_list[i]);

		s32_IpCount++;
	}

	if (!s32_IpCount)
		return WSAENETDOWN; // no local IP means no network available

	for (int i = 0; i<s32_IpCount; i++)
	{
		BYTE* pu8_Addr = (BYTE*)&ps32_IpList[i];
	
		sprintf(s8_IpList[i],"%d.%d.%d.%d",pu8_Addr[0], pu8_Addr[1], pu8_Addr[2], pu8_Addr[3]);
	}
	return 0;
}
