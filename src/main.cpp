#include "Library.h"

#include <iostream>

#define log(x) std::cout << x

void print_welcome();

int main() {
	while (true)
	{
		print_welcome();
		std::cin.get();
		system("cls");
		User *user = new User{};
		user->login();
		Library lib{user};
		lib.run_library();
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
		file.open((fm::init_workingdir() + "Welcome.txt"));
		if(file.is_open()){
		while(!file.eof()){
			std::getline(file, line);
			std::cout<<line<<std::endl;
		}
		}
		else
			log("Could not open Welcome Message\n");
	}
	catch(const std::ifstream::failure &e){}
	file.close();
};

