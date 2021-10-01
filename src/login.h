#include "CSVParser.h"
#include <iostream>
#include <string>


class User{
    std::string m_username;
    std::string m_password;
    
public:
    bool login();
};

class UserHandler{
    DataHandler datahandler;
    user_id user;
    int tries_left = 2;
public:
    bool user_request(const std::string&, const std::string&);
};
