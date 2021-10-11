#pragma once
#include "CSVParser.h"
#include <iostream>

#define log(x) std::cout << x

class User { //holds the user and is responsible for login, logout and activity log -> gets automatically deleted with logout
private:
    csv::CSVParser *mptr_csv_parser;                                    //stores one csv parser for all instances
    bool m_login_flag = false;                                     //flag to indicate wheather user is logged in or not
    bool m_user_request(std::string* , std::string* ); //deals with authentification of the user
public:
    User();
    ~User();
    bool login();               //only called at the beginning to authenticate the user or create a new one
private:                        //private temporary variables
    std::string m_database {"D:/Simon/Documents/Visual Studio 2017/Projects/GetIntoCPPagain/Data/Data.csv"};
    std::string *mptr_username; //username of the current user gets deleted after loggin
    std::string *mptr_password; //password entered by user also gets deleted after authenticating login
};
