#include "CSVParser.h"
#include <iostream>
#include <string>
#include <fstream>
#include <string_view>

#define log(x) std::cout<<x

class User{
private:
    bool m_loggin_flag = false;
    std::string* mptr_username;
    std::string* mptr_password;
	bool m_user_request(const std::string*, const std::string*);
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
