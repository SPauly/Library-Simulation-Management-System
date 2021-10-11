#include "login.h"
#include <iostream>
using namespace std;

int main() {
	User* user = new User{};
	user->login();
	//allow the user to do something with the application
	//user->log_activity(); 
	//user->logout();
	//user->get_activity();
	return 0;
}

