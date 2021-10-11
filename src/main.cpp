#include "login.h"
#include <iostream>
using namespace std;

int main() {
	User* user = new User{};
	std::string file = "D:/Simon/Documents/Visual Studio 2017/Projects/GetIntoCPPagain/Data/Data.csv";
	csv::CSVParser csv_parser(&file);
	csv_parser.print_csv();
	cin.get();
	//user->login();
	//allow the user to do something with the application
	//user->log_activity(); 
	//user->logout();
	//user->get_activity();
	return 0;
}

