#include <winsock2.h>
#include <iostream>

#include  "cNetEventsProcDLL.h"
#pragma comment(lib, "ws2_32")
#pragma comment(lib, "NetEventsProcDLL") // NetEventsProcDLL.lib placed in ...\FastStart\Client\ 

#pragma once

using namespace std;

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
#define PORT         7001

class MetaClient
{
private:
	int  h_Client;
	DWORD u32_Err;

	char  readData[20];
	DWORD  readBufLen;

	int readSize;

	double doubleBuf[1];
	DWORD  doubleBufLen;
	int    readLen[1];
	int		sendLen[1];

	int intBuf[1];
	DWORD  intBufLen;

	// LocalIp = 0x6401a8c0 -> 192.168.1.100
	// Returns a list of all local IP's on this computer (multiple IP's if multiple network adapters)
	DWORD GetLocalIPs(char s8_IpList[][20], int &s32_IpCount);
	void init(string host, int port) throw(int);

public:
	void sendCharArray(char *tomb) throw(int);
	void sendString(string message) throw(int);
	void sendInt(int number) throw(int);
	void sendDouble(double number) throw(int);
	string readString() throw(int);
	int readInt() throw(int);
	double readDouble() throw(int);
	static string getErrorMessage(int exception);
	MetaClient(void);
	MetaClient(string ip, int port);
	MetaClient(int port);
	~MetaClient(void);
};

