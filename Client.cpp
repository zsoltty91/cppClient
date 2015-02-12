#include <iostream>
#include <string>


#include "RequestManager.h"
#include "Javaserver.h"
#include "MetaClient.h"

using namespace std;

int main()
{
	try {
	MetaClient meta;
	} catch (int exception) {
		cout<<MetaClient::getErrorMessage(exception);
	}
	try {
		Javaserver Jserver;
		string read;
		printf("Server initialized.");
		while(read.compare("FINE")!=0 ) {
			try{
				read=Jserver.readMessage();
				cout<<read;
			} catch(int e) {
				cout<<Javaserver::getErrorMessage(e);
				Sleep(1500);
				exit(1);
			}
		}
	} catch (int e) {
		cout<<Javaserver::getErrorMessage(e);
		Sleep(1500);
		exit(1);
	}
	return 0;
}


