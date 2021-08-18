#include "user.h"
#include <iostream>
using namespace std;

int main() {
	UserHandler userhandler;
	string username_input;
	string password_input;

	cout <<"**************************       Login/Registration      **************************" << endl;
	cout <<"                                 Username: ";
	getline(cin, username_input);
	cout <<"                                 Password: ";
	getline(cin, password_input);
	userhandler.user_request(username_input); //validate input -> call user class and check for user
	//check password -> match regex or right password?
	cout<<endl;
	username_input.clear();
	password_input.clear();

	return 0;
}

