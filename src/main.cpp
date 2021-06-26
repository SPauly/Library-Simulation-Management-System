#include "user.h"
#include <iostream>
using namespace std;

int main() {
	string username_input;
	string password_input;

	cout <<"**************************       Login/Registration      **************************" << endl;
	cout <<"                                     Username: ";
	cin>>username_input;
	//validate input -> call user class and check for user
	cout<<"                                      Password: ";
	cin>>password_input;

	cout<<endl;
	username_input.clear();
	password_input.clear();
	//check password -> match regex or right password?

	return 0;
}

