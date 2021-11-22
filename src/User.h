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

    enum _Userstate
    { 
        _Statmask = 0x17
    };

    static constexpr _Userstate goodbit = (_Userstate)0x0; //everything's fine
	static constexpr _Userstate incobit = (_Userstate)0x1; //incosistency bit
	static constexpr _Userstate failbit = (_Userstate)0x2; //failure
	static constexpr _Userstate badbit = (_Userstate)0x4;  //Input error

    enum _Size {
        _Usersize = 300
    };

    static constexpr _Size usersize = (_Size)400;
    static constexpr _Size publishersize = (_Size)500;

    struct _ID {
        _Openmode mode = failure;
        std::string _string_id = "";
        _ID& init_id(std::string_view _id){
            switch (_id[0])
            {
            case 'U':
                mode = user;
                break;
            case 'P':
                mode = publisher;
                break;
            case 'A':
                mode = admin;
                break;
            default:
                mode = failure;
                return *this;
            }
            _string_id = _id;
            return *this;
        }
        std::string_view _create_ID(_Openmode& mode, size_t beg, size_t end);
    };
    

    struct _dimensions {
        size_t beg = 0;
        size_t end = 0;
        size_t space = end - beg;
        size_t freespace = 0;
    };

    struct name {
        std::string firstname = "";
        std::string lastname = "";
        std::string_view fullname = firstname + " " + lastname;
        std::string_view fullname_csv = firstname + "," + lastname;
    };


    //bool m_user_request(); //deals with authentification of the user
    //bool m_create_user();  //deals with creating a new user
    
    class User
    { //holds the user and is responsible for login, logout and activity log -> gets automatically deleted with logout
    private:
        _Userstate m_state = goodbit;
        _Openmode m_mode = failure; //flag to indicate wheather user is logged in or not
        
        _dimensions m_dimensions;

        _ID m_ID; //User ID UXXXXXX
        name m_user_name;

        std::vector<csv::Row> mvec_books;
        std::vector<csv::Row> mvec_owned;
        std::vector<csv::Row> mvec_published; 

        std::string m_path_userinfo{fm::init_workingdir() + "Data\\Userinfo.txt"};
        csv::Header m_bookheader{"BID,DATE,POSITION"};
        std::fstream m_userinfo_txt;

        _Openmode& mf_authenticate_user(csv::CSVParser&);
        _Openmode& mf_create_user(csv::CSVParser&);
        _Openmode& mf_create_user_info();
        _Userstate& mf_load_userinfo();
        _Userstate& mf_set_state(const _Userstate&);
        _Openmode& mf_set_mode(const _Openmode&);
    public:
        User() = delete;
        User(_Openmode&);
        ~User();
        
        _Openmode& login();
        _Openmode& logout();
        
        _Openmode& get_mode() const;
        _Userstate& get_state() const;
        std::string_view get_name() const;
        _ID& get_id() const;


    };



}