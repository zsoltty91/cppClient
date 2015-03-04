#include "MetaClient.h"

MetaClient::MetaClient(void)
{
	init(SERVER_IP,PORT);
}

MetaClient::MetaClient(string ip, int port)
{
	init(ip,port);
}

MetaClient::MetaClient(int port)
{
	init(SERVER_IP,port);
}

void MetaClient::init(string host, int port) throw(int) 
{
	readBufLen = 20;
	doubleBufLen = 1;
	intBufLen = 1;
	sendLen[0]=1;

	char s8_IpList[10][20];

	int s32_IpCount;
	u32_Err = GetLocalIPs(s8_IpList, s32_IpCount);

	if (u32_Err)
	{
		throw u32_Err;
	}

	char s8_ServerIP[] = SERVER_IP;
	int  s32_Port      = PORT;
	int  ph_Client[1];

	DWORD u32_Error = cNetEventsProcDLL::ConnectTo(SERVER_IP, PORT, ph_Client);

	if (u32_Error)
	{
		throw u32_Error;
	}
	h_Client = ph_Client[0];
	cout<<"Connected to metatrader.";
}

void MetaClient::sendCharArray(char *tomb) throw(int)
{
	int size = strlen(tomb);
	for(int i=0;i<size;i++) {
		intBuf[0]=tomb[i];
		u32_Err = cNetEventsProcDLL::SendToInt(h_Client,intBuf ,sendLen[0]);
		if(u32_Err) {
			throw u32_Err;
		}
		Sleep(3);
	}
	intBuf[0]='\0';
	u32_Err = cNetEventsProcDLL::SendToInt(h_Client,intBuf ,sendLen[0]);
	if(u32_Err) {
		throw u32_Err;
	}
}

void MetaClient::sendString(string message) throw(int) 
{
	int size = message.size();
	for(int i=0;i<size;i++) {
		intBuf[0]=message[i];
		u32_Err = cNetEventsProcDLL::SendToInt(h_Client,intBuf ,sendLen[0]);
		if(u32_Err) {
			getErrorMessage(u32_Err);
			throw u32_Err;
		}
		//printf("sent %c\n",intBuf[0]);
		Sleep(3);
	}
	intBuf[0]='\0';
	u32_Err = cNetEventsProcDLL::SendToInt(h_Client,intBuf ,sendLen[0]);
	//printf("sent %c\n",intBuf[0]);
	if(u32_Err) {
		getErrorMessage(u32_Err);
		throw u32_Err;
	}
}
void MetaClient::sendInt(int number) throw(int)
{
	intBuf[0]=number;
	u32_Err = cNetEventsProcDLL::SendToInt(h_Client,intBuf ,sendLen[0]);
	if(u32_Err) {
		throw u32_Err;
	}
}

void MetaClient::sendDouble(double number) throw(int)
{
	doubleBuf[0]=number;
	u32_Err = cNetEventsProcDLL::SendToDouble(h_Client,doubleBuf ,sendLen[0]);
	if(u32_Err) {
		throw u32_Err;
	}
}

string MetaClient::readString() throw(int)
{
	int i=0;
	while(true){
		readLen[0]=0;
		while(!readLen[0]) {
			u32_Err= cNetEventsProcDLL::ReadFromInt(h_Client,intBuf,intBufLen,readLen);		
			if(u32_Err) {
				throw u32_Err;
			}
		}
		//printf("read: %c\n",intBuf[0]);
		readData[i++]=intBuf[0];
		if(intBuf[0]=='\0') {
			string read(readData);
			return read;
		}
	}
}

int MetaClient::readInt() throw(int)
{
	readLen[0]=0;
	while(!readLen[0]) {
		u32_Err= cNetEventsProcDLL::ReadFromInt(h_Client,intBuf,intBufLen,readLen);	
		if(u32_Err) {
			throw u32_Err;
		}
	}
	return intBuf[0];
}

double MetaClient::readDouble() throw(int)
{
	readLen[0]=0;
	while(!readLen[0]) {
		u32_Err= cNetEventsProcDLL::ReadFromDouble(h_Client,doubleBuf,doubleBufLen,readLen);	
		if(u32_Err) {
			throw u32_Err;
		}
	}
	return doubleBuf[0];
}

string MetaClient::getErrorMessage(int exception)
{
	switch (exception)
		{
		case 0:
			cout<<"Operation(...) OK";
			return "Operation(...) OK";
			break;
		case ERROR_INVALID_PARAMETER:
			cout<<"ERROR_INVALID_PARAMETER -> One of this parms or more: h_Client, ps8_SendData, u32_SendDataLen is invalid...";
			return "ERROR_INVALID_PARAMETER -> One of this parms or more: h_Client, ps8_SendData, u32_SendDataLen is invalid...";
			break;
		case WSAEWOULDBLOCK:
			cout<<"WSAEWOULDBLOCK -> The data will be send after the next FD_WRITE event, do nothing.";
			return "WSAEWOULDBLOCK -> The data will be send after the next FD_WRITE event, do nothing.";
			break;

		case WSA_IO_PENDING:
			cout<<"WSA_IO_PENDING -> Error: A previous Send operation is still pending. This data will not be sent, try latter.";
			return "WSA_IO_PENDING -> Error: A previous Send operation is still pending. This data will not be sent, try latter.";
			break;
		default:
			cout<<"Failed with severe error.";
			return "Failed with severe error.";
			break;
		};
}
	

MetaClient::~MetaClient(void)
{
	cNetEventsProcDLL::ConnectClose(h_Client);
}

// LocalIp = 0x6401a8c0 -> 192.168.1.100
// Returns a list of all local IP's on this computer (multiple IP's if multiple network adapters)
DWORD MetaClient::GetLocalIPs(char s8_IpList[][20], int &s32_IpCount)
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
	
		//sprintf(s8_IpList[i],"%d.%d.%d.%d",pu8_Addr[0], pu8_Addr[1], pu8_Addr[2], pu8_Addr[3]);
	}
	return 0;
}
