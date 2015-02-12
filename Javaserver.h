#include <winsock2.h>
#include <iostream>

#pragma once

#define BUFSIZE 1024                // buffer size
#define CLIENTSIZE 1				// HAVE TO MODIFY SOME CODE IN CASE OF CHANGING VALUE

using namespace std;

class Javaserver
{
private:
   int fd;	        	           		// socket endpt
   int fdc[CLIENTSIZE];                        	// socket endpt
   int flags;                      	// rcv flags
   struct sockaddr_in server;      	// socket name (addr) of server
   struct sockaddr_in client[CLIENTSIZE];	     	// socket name of client
   int server_size;                	// length of the socket addr. server
   int client_size;                	// length of the socket addr. client
   int bytes;		           					// length of buffer 
   int rcvsize[CLIENTSIZE];                    	// received bytes
   int trnmsize;                   	// transmitted bytes
   int err;                        	// error code
   char on;                        	// 
   char buffer[BUFSIZE+1];   // input buffer  
   char outbuffer[BUFSIZE+1];	//output buffer
	WSADATA WsaDat;
	void init(int port) throw(int);

public:
	static string getErrorMessage(int exception);
	string readMessage() throw(int);
	void sendMessage(string message) throw(int);
	Javaserver(void) throw(int);
	Javaserver(int port) throw(int);
	~Javaserver(void);
};

