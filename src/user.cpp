#include "user.h"

void UserHandler::user_request(const std::string& username_ref){
    //read data 
    try{
        datahandler.find_user(username_ref);
    }
    catch
    //see if username is there 
    //is there -> ask for password 3x 
    //is not -> ask to create new
    //if new -> ask for password create new
    //if not -> ask for username again
}