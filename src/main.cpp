#include "login.h"
#include <iostream>

#define log(x) std::cout << x

void print_welcome();

int main() {
	while (true)
	{
		print_welcome();
		std::cin.get();
		User *user = new User{};
		user->login();
		//allow the user to do something with the application
		//user->log_activity();
		//user->logout();
		//user->get_activity();
		std::cin.get();
		delete user;
		system("cls");
	}
	return 0;
}

void print_welcome(){
	std::fstream file;
	std::string line;

	try{
		file.open("E:/Simon/Documents/Visual Studio 2017/Projects/GetIntoCPPagain/Welcome.txt");
		while(!file.eof()){
			std::getline(file, line);
			std::cout<<line<<std::endl;
		}
	}
	catch(const std::ifstream::failure &e){}
	file.close();
};

