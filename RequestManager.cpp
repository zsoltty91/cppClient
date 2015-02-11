#include "RequestManager.h"


RequestManager::RequestManager(void)
{
}

RequestManager::~RequestManager(void)
{
}

Request RequestManager::getFirstRequest() {
	return requests.front();
}

void RequestManager::removeFirstRequest() {
	requests.pop_front();
}

void RequestManager::removeRequest(Request request){
	requests.remove(request);
}

void RequestManager::addRequest(Request request) {
	requests.push_back(request);
}

void RequestManager::clearRequests() {
	requests.clear();
}