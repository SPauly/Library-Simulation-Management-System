#include "Library.h"

#include <iostream>

#define log(x) std::cout << x

void print_welcome();

int main() {
	std::string login_type = "";
	print_welcome();
	std::cin.get();

	while (true) {
		system("cls");
		log("Enter type of login (e.g user, publisher, admin - press ENTER for default)>>  ");
		if(!std::getline(std::cin, login_type)){/*do something*/}
		Library lib{login_type};
		if(!lib.run_library())
			break;

	} 

	return 0;
}

void print_welcome(){
	std::fstream file;
	std::string line;

	try{
		file.open((fm::init_workingdir() + "Welcome.txt"), std::ios::in | std::ios::binary);
		if(file.is_open()){
		while(!file.eof()){
			fm::_getline(file, line);
			std::cout<<line<<std::endl;
		}
		}
		else
			log("Could not open Welcome Message\n");
	}
	catch(const std::ifstream::failure &e){}
	file.close();
};

