#include "Javaserver.h"

//******* JAVA SERVER
#ifndef INET_ADDRSTRLEN
#define INET_ADDRSTRLEN 16
#endif

#define SERVERLOG

#define WSA_FAIL "WSA initialization failed."
#define WSA_ERR_CODE 0
#define SOCKET_ERR "Socket creation error."
#define SOCKET_ERR_CODE -1
#define BINDING_ERROR "Cannot bind to the socket."
#define BINDING_ERROR_CODE -2
#define LISTEN_ERROR "Cannot listen to the socket."
#define LISTEN_ERROR_CODE -3
#define ACCEPT_ERROR "Cannot accept on socket."
#define ACCEPT_ERROR_CODE -4
#define CANNOT_RECEIVE "Cannot receive from the socket."
#define CANNOT_RECEIVE_CODE -5
#define CANNOT_SEND "Cannot send data to the client."
#define CANNOT_SEND_CODE -6

#define BUFSIZE 1024                      // buffer size
#define PORT_NO 2001
//******* JAVA SERVER



Javaserver::Javaserver(void) throw(int)
{
	init(PORT_NO);
}

Javaserver::Javaserver(int port) throw(int)
{
	init(port);
}
 
void Javaserver::init(int port) throw(int)
{
	if(WSAStartup(MAKEWORD(2,2), &WsaDat) != 0)
	{
		WSACleanup();
		throw WSA_ERR_CODE;
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
	
	/* Creating socket */
   fd = socket(AF_INET, SOCK_STREAM, 0 );
   if (fd < 0) {
	   throw SOCKET_ERR_CODE;
	}

   /* Setting socket options */
   setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on);
   setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof on);

   /* Binding socket */
   err = bind(fd, (struct sockaddr *) &server, server_size);
   if (err < 0) {
	   throw BINDING_ERROR_CODE;
	}

   /* Listening */
   err = listen(fd, 10);
   if (err < 0) {
	   throw LISTEN_ERROR_CODE;
	}

   printf("wait for client..");
   //wait for java client
   fdc[0] = accept(fd, (struct sockaddr *) &client[0], &client_size);
	if (fdc < 0) {
		throw ACCEPT_ERROR_CODE;
	}
	printf("client connected.");
}

string Javaserver::getErrorMessage(int exception) 
{
	switch(exception) {
	case WSA_ERR_CODE: return WSA_FAIL;break;
		case SOCKET_ERR_CODE: return SOCKET_ERR;break;
		case BINDING_ERROR_CODE: return BINDING_ERROR; break;
		case LISTEN_ERROR_CODE: return LISTEN_ERROR; break;
		case ACCEPT_ERROR_CODE: return ACCEPT_ERROR; break;
		case CANNOT_RECEIVE_CODE: return CANNOT_RECEIVE; break;
		case CANNOT_SEND_CODE: return CANNOT_SEND; break;
		default : return ""; break;
	}
	return "";
}

string Javaserver::readMessage() throw(int) {
	rcvsize[0] = recv( fdc[0], buffer, 1024, flags );
	if (rcvsize[0] < 0) {
		throw CANNOT_RECEIVE_CODE;
	}
	return buffer;
}

void Javaserver::sendMessage(string message) throw(int) {
	int size = message.size();
	for(int i =0; i<size;i++) {
		outbuffer[i]=message[i];
	}
	outbuffer[size]='\n';
	outbuffer[++size]='\0';
	bytes = strlen(outbuffer);
	trnmsize = send(fdc[0], outbuffer, bytes, flags);
	if (trnmsize < 0) {
		throw CANNOT_SEND_CODE;
	}
}

Javaserver::~Javaserver(void)
{
	WSACleanup();
}