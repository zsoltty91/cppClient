#include "Request.h"
#include <list>

using namespace std;

#pragma once
class RequestManager
{
private:
	list<Request> requestsToSendServer;
	list<Request> requestsToProcessServer;

	list<Request> requestsToSendClient;
	list<Request> requestsToProcessClient;

public:
	Request getFirstRequestToSendServer();
	void removeFirstRequestToSendServer();
	void removeRequestToSendServer(Request request);
	void addRequestToSendServer(Request request);
	void clearRequestsToSendServer();
	bool isEmptyRequestsToSendServer();
	Request getFirstRequestToSendClient();
	void removeFirstRequestToSendClient();
	void removeRequestToSendClient(Request request);
	void addRequestToSendClient(Request request);
	void clearRequestsToSendClient();
	bool isEmptyRequestsToSendClient();
	Request getFirstRequestToProcessServer();
	void removeFirstRequestToProcessServer();
	void removeRequestToProcessServer(Request request);
	void addRequestToProcessServer(Request request);
	void clearRequestsToProcessServer();
	bool isEmptyRequestsToProcessServer();
	Request getFirstRequestToProcessClient();
	void removeFirstRequestToProcessClient();
	void removeRequestToProcessClient(Request request);
	void addRequestToProcessClient(Request request);
	void clearRequestsToProcessClient();
	bool isEmptyRequestsToProcessClient();
	RequestManager(void);
	~RequestManager(void);
};

