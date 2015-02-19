#include "RequestManager.h"


RequestManager::RequestManager(void)
{
}

RequestManager::~RequestManager(void)
{
}

Request RequestManager::getFirstRequestToSendServer() {
	return requestsToSendServer.front();
}

void RequestManager::removeFirstRequestToSendServer() {
	requestsToSendServer.pop_front();
}

void RequestManager::removeRequestToSendServer(Request request){
	requestsToSendServer.remove(request);
}

void RequestManager::addRequestToSendServer(Request request) {
	requestsToSendServer.push_back(request);
}

void RequestManager::clearRequestsToSendServer() {
	requestsToSendServer.clear();
}

bool RequestManager::isEmptyRequestsToSendServer() {
	return requestsToSendServer.empty();
}

bool RequestManager::isEmptyRequestsToSendClient() {
	return requestsToSendClient.empty();
}

Request RequestManager::getFirstRequestToSendClient() {
	return requestsToSendClient.front();
}

void RequestManager::removeFirstRequestToSendClient() {
	requestsToSendClient.pop_front();
}

void RequestManager::removeRequestToSendClient(Request request){
	requestsToSendClient.remove(request);
}

void RequestManager::addRequestToSendClient(Request request) {
	requestsToSendClient.push_back(request);
}

void RequestManager::clearRequestsToSendClient() {
	requestsToSendClient.clear();
}


Request RequestManager::getFirstRequestToProcessServer() {
	return requestsToProcessServer.front();
}

void RequestManager::removeFirstRequestToProcessServer() {
	requestsToProcessServer.pop_front();
}

void RequestManager::removeRequestToProcessServer(Request request){
	requestsToProcessServer.remove(request);
}

void RequestManager::addRequestToProcessServer(Request request) {
	requestsToProcessServer.push_back(request);
}

void RequestManager::clearRequestsToProcessServer() {
	requestsToProcessServer.clear();
}

bool RequestManager::isEmptyRequestsToProcessServer() {
	return requestsToProcessServer.empty();
}

bool RequestManager::isEmptyRequestsToProcessClient() {
	return requestsToProcessClient.empty();
}

Request RequestManager::getFirstRequestToProcessClient() {
	return requestsToProcessClient.front();
}

void RequestManager::removeFirstRequestToProcessClient() {
	requestsToProcessClient.pop_front();
}

void RequestManager::removeRequestToProcessClient(Request request){
	requestsToProcessClient.remove(request);
}

void RequestManager::addRequestToProcessClient(Request request) {
	requestsToProcessClient.push_back(request);
}

void RequestManager::clearRequestsToProcessClient() {
	requestsToProcessClient.clear();
}