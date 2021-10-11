#include "login.h"

User::User(){
	//temporary allocated values
	mptr_username = new std::string;
	mptr_password = new std::string;

	//longer allocation
	mptr_csv_parser = new csv::CSVParser(&m_database);

};

User::~User(){
	delete mptr_csv_parser;
};

bool User::m_user_request(std::string* userptr, std::string* passptr){
	int _tries = 0;
	while(_tries < 3){
		
	}
};

bool User::login(){

	log("**************************       Login/Registration      **************************")<<std::endl;
	log("                                 Username: ");
	std::getline(std::cin, *mptr_username);
	("                                 Password: ");
	std::getline(std::cin, *mptr_password);
	std::cout<<std::endl; 
	if(m_user_request(mptr_username, mptr_password) == true){
		log("Valid User Found and Password correct")<<std::endl;
		return m_login_flag = true;
	} //validate input -> call user class and check for user
	else {
		log("Do you instead want to create a new Account? [y/n]")<<std::endl;
		
	}
	//is not -> ask to create new
    //if new -> ask for password create new
    //if not -> ask for username again
	
    return m_login_flag = false;
};