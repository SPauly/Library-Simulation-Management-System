#pragma once
#include "CSVParser.h"
#include <iostream>
#include <limits>
#include <regex>
#include <random>

#define log(x) std::cout << x

class Userinfo{
private:
    unsigned int m_next_position = 0;
    std::string_view m_ID;
    std::string m_user_name = "";
    csv::Row m_bookheader{"BID,DATE,POSITION"};
    std::vector<csv::Row> mvec_books;
    std::vector<csv::Row> mvec_owned;
    std::vector<csv::Row> mvec_published;

    std::fstream m_userinfo_txt;
public:
    Userinfo(const std::string*);

    bool create_user_info(std::string_view);
    bool load_userinfo(std::string_view);

    std::string_view get_name();
};

class User { //holds the user and is responsible for login, logout and activity log -> gets automatically deleted with logout
protected:
    bool m_user_request(); //deals with authentification of the user
    bool m_create_user(); //deals with creating a new user 
    std::string_view m_create_ID(int,int);
public:
    User();
    ~User();
    bool login();               
private: 
    bool m_login_flag;          //flag to indicate wheather user is logged in or not
    std::string *mptr_username; //username of the current user gets deleted after loggin
    std::string *mptr_password; //password entered by user also gets deleted after authenticating login
    std::string m_ID; //User ID UXXXXXX

    std::string m_path_userfile {"E:/Simon/Documents/Visual Studio 2017/Projects/GetIntoCPPagain/Data/Userfile.csv"};
    std::string m_path_userinfo {"E:/Simon/Documents/Visual Studio 2017/Projects/GetIntoCPPagain/Data/Userinfo.txt"};
    csv::CSVParser *mptr_csv_parser; 
    Userinfo* mptr_userinfo;                                   
};
