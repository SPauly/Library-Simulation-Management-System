#include "login.h"

bool User::login(){

	log("**************************       Login/Registration      **************************")<<std::endl;
	log("                                 Username: ");
	std::getline(std::cin, *mptr_username);
	("                                 Password: ");
	std::getline(std::cin, *mptr_password);
	std::cout<<std::endl; 
	if(m_user_request(mptr_username, mptr_password) == true){
		log("Valid User Found and Password correct")<<std::endl;
		return m_loggin_flag = true;
	} //validate input -> call user class and check for user
	//is not -> ask to create new
    //if new -> ask for password create new
    //if not -> ask for username again
	
    return m_loggin_flag = false;
};