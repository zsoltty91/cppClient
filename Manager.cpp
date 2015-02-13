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
			requestHandler.addRequest(request);
		} catch(int e) {
			throw e;
		}
	}
}

void Manager::sendServerRequests()
{

}


Manager::~Manager(void)
{
}
