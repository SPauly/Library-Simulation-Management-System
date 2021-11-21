#pragma once
#include "CSVParser.h"
#include <iostream>
#include <limits>
#include <regex>
#include <random>

namespace user
{
    #define log(x) std::cout << x

    class Error : public std::runtime_error
    {
    public:
        Error(const std::string &msg) : std::runtime_error(std::string("User: ").append(msg)) {}
    };

    enum _Openmode {
        _Openmask = 0x4C
    };

    static constexpr _Openmode user = (_Openmode)0x55; //U
    static constexpr _Openmode publisher = (_Openmode)0x50; //P
    static constexpr _Openmode admin = (_Openmode)0x41; //A
    static constexpr _Openmode logout = (_Openmode)0x4C; //L
    static constexpr _Openmode failure = (_Openmode)0x0; //NULL

    enum _userstate
    { 
        _Statmask = 0x17
    };

    static constexpr _userstate goodbit = (_userstate)0x0; //everything's fine
	static constexpr _userstate incobit = (_userstate)0x1; //incosistency bit
	static constexpr _userstate failbit = (_userstate)0x2; //failure
	static constexpr _userstate badbit = (_userstate)0x4;  //Input error

    struct _ID {
        unsigned int _ui_id = 0;
        _Openmode _token = user;
        std::string _string_id = "" + _token;
    };
    _ID& _create_ID(_ID&, int, int);

    struct _dimensions {
        unsigned int beg = 0;
        unsigned int end = 0;
        unsigned int space = end - beg;
        unsigned int freespace = 0;
    };

    struct name {
        std::string firstname = "";
        std::string lastname = "";
        std::string_view fullname = firstname + " " + lastname;
    };

    _Openmode& _login(const User&);
    _Openmode& _logout(const User&);
    //bool m_user_request(); //deals with authentification of the user
    //bool m_create_user();  //deals with creating a new user
    
    class User
    { //holds the user and is responsible for login, logout and activity log -> gets automatically deleted with logout
    private:
        _userstate m_state = goodbit;
        _Openmode m_mode = failure; //flag to indicate wheather user is logged in or not
        
        _dimensions m_dimensions;

        _ID m_ID; //User ID UXXXXXX
        name m_name_user;

        std::string m_path_userinfo{fm::init_workingdir() + "Data\\Userinfo.txt"};
        csv::Header m_bookheader{"BID,DATE,POSITION"};

        std::vector<csv::Row> mvec_books;
        std::vector<csv::Row> mvec_owned;
        std::vector<csv::Row> mvec_published; 

        std::fstream m_userinfo_txt;

    public:
        User();
        ~User();

        _Openmode& get_mode();
        _userstate& get_state();
        std::string_view get_name();
        _ID& get_id();

        bool create_user_info(std::string_view);
        bool load_userinfo(std::string_view);
    };



}