#include "Manager.h"

#include <iostream>
#include <string>

Manager::Manager(void) throw(int)
{
	debug=false;
	metaClientInited=false;
	requestHandler = RequestManager();

	try {
		jServer = Javaserver();
		if(debug) {
			cout<<"Java server initialized.\n";
		}
	} catch (int exception) {
		if(debug) {
			throw exception;
		}
	}

	try {
		metaClient = MetaClient();
		metaClientInited=true;
	} catch (int exception) {
		if(debug) {
			cout<<"Failed to connect metatrader server";
		}
	}
}

void Manager::receiveClientRequests() 
{
	string read;
	Request request;
	Variable var;
	while(true) {
		try{
			read=jServer.readMessage();
			if(read.compare("FINE")==0) {
				break;
			}
			request = Request();
			request.setType(read);
			if(read.compare("AVAILABLE")!=0) {
				read=jServer.readMessage();
				request.setName(read);
			}
			while(true) {
				read=jServer.readMessage();
				if(read.compare("END")==0) {
					break;
				}
				var = Variable();
				var.setName(read);
				if(request.getType().compare("SET")==0) {
					read = jServer.readMessage();
					var.setValue(read);
				}
				request.addVariable(var);
			}
			requestHandler.addRequestToSendServer(request);
		} catch(int e) {
			throw e;
		}
	}
}

void Manager::sendClientRequests() throw(int) {
	Request request;
	list<Variable> variables;
	while(!requestHandler.isEmptyRequestsToSendClient()) {
		request = requestHandler.getFirstRequestToSendClient();
		if(request.getType().compare("AVAILABLE")==0) {
			jServer.sendMessage("AVAILABLE_FINE");
			requestHandler.removeFirstRequestToSendClient();
			continue;
		}
		jServer.sendMessage(request.getType());
		jServer.sendMessage(request.getName());
		variables = request.getVariables();
		for (list<Variable>::iterator it = variables.begin(); it != variables.end(); it++){
			jServer.sendMessage(it->getName());
			if(request.getType().compare("SET")!=0) {
				jServer.sendMessage(it->getValue());
			}
		}
		jServer.sendMessage("END");
		requestHandler.removeFirstRequestToSendClient();
	}
	jServer.sendMessage("FINE");
}

void Manager::sendServerRequests() throw(int) {
	Request request;
	list<Variable> variables;
	while(!requestHandler.isEmptyRequestsToSendServer()) {
		request = requestHandler.getFirstRequestToSendServer();
		metaClient.sendString(request.getType());
		metaClient.sendString(request.getName());
		for (list<Variable>::iterator it = variables.begin(); it != variables.end(); it++){
			metaClient.sendString(it->getName());
			if(request.getType().compare("SET")==0) {
				metaClient.sendString(it->getValue());
			}
		}
		metaClient.sendString("END");
		requestHandler.removeFirstRequestToSendServer();
		requestHandler.addRequestToProcessServer(request);
	}
	metaClient.sendString("FINE");
}

void Manager::receiveServerRequests() throw(int) {
	Request request, processRequest;
	Variable variable;
	list<Variable> variables;
	string read;
	while(!requestHandler.isEmptyRequestsToProcessServer()) {
		processRequest = requestHandler.getFirstRequestToProcessServer();
		read = metaClient.readString();
		if(read.compare("FINE")==0) {
			cout<<"There was more request to get.";
			break;
		}
		if(processRequest.getType().compare("AVAILABLE")) {
			while(true) {
				if(read.compare("AVAILABLE_FINE")==0) {
					break;
				}
				request = Request();
				request.setType(read);
				read = metaClient.readString();
				request.setName(read);
				while(true) {
					read = metaClient.readString();
					if(read.compare("END")==0) {
						break;
					}
					variable = Variable();
					variable.setName(read);
					read=metaClient.readString();
					variable.setValue(read);
					request.addVariable(variable);
				}
				requestHandler.addRequestToSendClient(request);
			}
			requestHandler.addRequestToSendClient(processRequest);
		} else {
			if(read.compare(processRequest.getType())!=0){
				throw 10;
			}
		
			read = metaClient.readString();
			if(read.compare(processRequest.getName())!=0) {
				throw 11;
			}
			variables = request.getVariables();
			for (list<Variable>::iterator it = variables.begin(); it != variables.end(); it++){
				read = metaClient.readString();
				if(read.compare(it->getName())!=0) {
					throw 12;
				}
				if(read.compare("END")==0) {
					cout<<"There was more variables to get";
					break;
				}
				read = metaClient.readString();
				it->setValue(read);
			}
			requestHandler.removeFirstRequestToProcessServer();
			requestHandler.addRequestToSendClient(processRequest);
		}
	}
}


Manager::~Manager(void)
{
}
