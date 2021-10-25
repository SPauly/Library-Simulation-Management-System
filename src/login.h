#pragma once
#include "CSVParser.h"
#include <iostream>
#include <limits>

#define log(x) std::cout << x

class User { //holds the user and is responsible for login, logout and activity log -> gets automatically deleted with logout
private:
    std::string m_database {"E:/Simon/Documents/Visual Studio 2017/Projects/GetIntoCPPagain/Data/Data.csv"};
    csv::CSVParser *mptr_csv_parser;                                    //stores one csv parser for all instances
    bool m_login_flag;                                     //flag to indicate wheather user is logged in or not
    bool m_user_request(); //deals with authentification of the user
    bool m_create_user(); //deals with creating a new user 
public:
    User();
    ~User();
    bool login();               
private:                        //private temporary variables
    std::string *mptr_username; //username of the current user gets deleted after loggin
    std::string *mptr_password; //password entered by user also gets deleted after authenticating login
};
