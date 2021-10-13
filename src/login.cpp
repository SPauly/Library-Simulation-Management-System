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

bool User::m_user_request(){
	int _tries = 0;
	while(_tries < 3){
		log("                                 Username: ");
		std::getline(std::cin, *mptr_username);
		log("                                 Password: ");
		std::getline(std::cin, *mptr_password);
		std::cout<<std::endl; 
		for(int i = 0; i < mptr_csv_parser->size(); i++){
			if(mptr_csv_parser["USER"][i] == *mptr_username){
				if(mptr_csv_parser["PASSWORD"][i] == *mptr_password){
					log("Login successful\n");
					m_login_flag = true;
					return m_login_flag;
				}
				else {
					log("Wrong Password. Try again!\n");
					break;
				}
			}
		}
		--_tries;
		log("User not found\n");
	}
};

bool User::login(){

	log("**************************       Login/Registration      **************************")<<std::endl;
	if(!m_user_request())
		log("Do you instead want to create a new Account? [y/n]")<<std::endl;
	}
	
    //if new -> ask for password create new
    //if not -> ask for username again
	
    return m_login_flag = false;
};