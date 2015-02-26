#include "Manager.h"

#include <iostream>
#include <string>

using namespace std;

Manager::Manager(void) throw(int)
{
	debug=true;
	metaClientInited=false;
	requestHandler = RequestManager();
	cout<<"Init manager";
	/*try {
		jServer = Javaserver();
		if(debug) {
			cout<<"Java server initialized.\n";
		}
	} catch (int exception) {
		if(debug) {
			throw exception;
		}
	}
	cout<<"java server initialized";

	try {
		metaClient = MetaClient();
		metaClientInited=true;
		cout<<"meta initialized";
	} catch (int exception) {
		if(debug) {
			cout<<"Failed to connect metatrader server";
			cout<<exception;
		}
	}*/
	cout<<"manager initialized";
}

void Manager::receiveClientRequests() 
{
	cout<<"receiveClientRequests";
	string read;
	Request request;
	Variable var;
	while(true) {
		try{
			read=jServer.readMessage();
			cout<<read;
			if(read.compare("FINE")==0) {
				break;
			}
			request = Request();
			request.setType(read);
			if(read.compare("AVAILABLE")!=0) {
				read=jServer.readMessage();
				request.setName(read);
				cout<<read;
			}
			while(true) {
				read=jServer.readMessage();
				cout<<read;
				if(read.compare("END")==0) {
					break;
				}
				var = Variable();
				var.setName(read);
				if(request.getType().compare("SET")==0) {
					read = jServer.readMessage();
					cout<<read;
					var.setValue(read);
				}
				request.addVariable(var);
			}

			cout<<"\n type:"+request.getType()+"\n";
			if(request.getType().compare("AVAILABLE")!=0) {
				cout<<"name: "+request.getName();
				cout<<"\nvariable name: "+request.getFirstVariable().getName();
				cout<<"\n variable value: "+request.getFirstVariable().getValue();
			}

			requestHandler.addRequestToSendServer(request);
		} catch(int e) {
			throw e;
		}
	}
}

void Manager::sendClientRequests() throw(int) {
	cout<<"sendClientRequest";
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
			//nem küldjük ki a seteseket
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
	cout<<"sendServerRequest\n";
	Request request;
	list<Variable> variables;
	while(!requestHandler.isEmptyRequestsToSendServer()) {
		cout<<"van requests to send server";
		request = requestHandler.getFirstRequestToSendServer();
		metaClient.sendString(request.getType());
		cout<<"sent type";
		if(request.getType().compare("AVAILABLE")!=0) {
			metaClient.sendString(request.getName());
			cout<<"sent name";
			variables = request.getVariables();
			for (list<Variable>::iterator it = variables.begin(); it != variables.end(); it++){
				cout<<"send variable ciklus\n";
				metaClient.sendString(it->getName());
				cout<<"sent variable"+it->getName();
				if(request.getType().compare("SET")==0) {
					metaClient.sendString(it->getValue());
					cout<<"sent value";
				}
			}
		}
		metaClient.sendString("END");
		cout<<"sent end\n";
		requestHandler.removeFirstRequestToSendServer();
		requestHandler.addRequestToProcessServer(request);
	}
	metaClient.sendString("FINE");
	cout<<"sent fine";
}

void Manager::receiveServerRequests() throw(int) {
	cout<<"receive server requests\n";
	Request request, processRequest;
	Variable variable;
	list<Variable> variables;
	string read;
	while(!requestHandler.isEmptyRequestsToProcessServer()) {
		cout<<"van request to get\n";
		processRequest = requestHandler.getFirstRequestToProcessServer();
		read = metaClient.readString();
		cout<<"típus "+read;
		if(read.compare("FINE")==0) {
			cout<<"There was more request to get.";
			break;
		}

		if(processRequest.getType().compare("AVAILABLE")==0) {
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
				cout<<"Request type mismatch(receiveServerRequests).";
				throw 10;
			}
		
			read = metaClient.readString();
			cout<<read+"\n";
			cout<<processRequest.getName();
			printf("%d \n",processRequest.getName().size());
			printf("%d \n",read.size());

			if(read.compare(processRequest.getName())!=0) {
				cout<<"kilép név";
				throw 11;
			}
			variables = request.getVariables();
			for (list<Variable>::iterator it = variables.begin(); it != variables.end(); it++){
				read = metaClient.readString();
				cout<<"variable: "+read;
				if(read.compare(it->getName())!=0) {
					throw 12;
				}
				if(read.compare("END")==0) {
					cout<<"There was more variables to get";
					break;
				}
				read = metaClient.readString();
				cout<<"value: "+read;
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
