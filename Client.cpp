#include <iostream>
#include <string>

#include "Manager.h"

using namespace std;

int main()
{
	Manager manager = Manager();
	cout<<"after initialized";
	while(true) {
		try{
			manager.receiveClientRequests();
		} catch(int e) {
			cout<<Javaserver::getErrorMessage(e);
			break;
		}

		try{
			manager.sendServerRequests();
		} catch(int e) {
			cout<<MetaClient::getErrorMessage(e);
			break;
		}

		try{
			manager.receiveServerRequests();
		} catch(int e) {
			cout<<Javaserver::getErrorMessage(e);
			cout<<"receiveserverException";
			break;
		}

		try{
			manager.sendClientRequests();
		} catch(int e) {
			cout<<Javaserver::getErrorMessage(e);
			break;
		}
		break;
	}

	Sleep(2000);
	return 0;
}


