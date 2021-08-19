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
	cout<<endl; 
	userhandler.user_request(username_input, password_input); //validate input -> call user class and check for user
	//is not -> ask to create new
    //if new -> ask for password create new
    //if not -> ask for username again
	username_input.clear();
	password_input.clear();

	return 0;
}

