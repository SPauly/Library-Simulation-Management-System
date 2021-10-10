#include "login.h"

User::User(){
	mptr_username = new std::string;
	mptr_password = new std::string;
};
User::~User(){

};

bool User::m_user_request(const std::string* userptr, const std::string* passptr){};

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