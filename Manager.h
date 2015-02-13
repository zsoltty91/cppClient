#include "RequestManager.h"
#include "Javaserver.h"
#include "MetaClient.h"

#pragma once
class Manager
{
private:
	boolean debug;
	boolean metaClientInited;
	RequestManager requestHandler;
	Javaserver jServer;
	MetaClient metaClient;
public:
	void receiveClientRequests() throw(int);
	void sendClientRequests() throw(int);
	void sendServerRequests() throw(int);
	void receiveServerRequests() throw(int);
	Manager(void) throw(int);
	~Manager(void);
};

