#include "Request.h"
#include <list>

using namespace std;

#pragma once
class RequestManager
{
private:
	list<Request> requests;

public:
	Request getFirstRequest();
	void removeFirstRequest();
	void removeRequest(Request request);
	void addRequest(Request request);
	void clearRequests();
	RequestManager(void);
	~RequestManager(void);
};

