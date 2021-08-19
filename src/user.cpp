#include "user.h"

bool UserHandler::user_request(const std::string& username_ref, const std::string& password_ref){
    
    user = datahandler.find_user(username_ref);

    if(user.username != ""){
        if(user.password == password_ref){
            
        }
    } else {
        return false;
    }
     
}