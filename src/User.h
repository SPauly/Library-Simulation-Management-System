#pragma once
#include "CSVParser.h"
#include "Book.h"
#include "Time.h"
#include <iostream>
#include <limits>
#include <regex>
#include <random>

namespace LSMS
{
    namespace user
    {
#define log(x) std::cout << x

        class Error : public std::runtime_error
        {
        public:
            Error(const std::string &msg) : std::runtime_error(std::string("User: ").append(msg)) {}
        };

        enum _Openmode
        {
            _Openmask = 0x4C
        };

        static constexpr _Openmode user = (_Openmode)0x55;      //U
        static constexpr _Openmode publisher = (_Openmode)0x50; //P
        static constexpr _Openmode admin = (_Openmode)0x41;     //A
        static constexpr _Openmode notlogged = (_Openmode)0x4C; //L
        static constexpr _Openmode failure = (_Openmode)0x0;    //NULL

        enum _Userstate
        {
            _Statmask = 0x17
        };

        static constexpr _Userstate goodbit = (_Userstate)0x1; //everything's fine
        static constexpr _Userstate incobit = (_Userstate)0x2; //incosistency bit
        static constexpr _Userstate failbit = (_Userstate)0x0; //failure
        static constexpr _Userstate badbit = (_Userstate)0x3;  //Input error

        enum _Size
        {
            _Usersize = 420
        };

        static constexpr _Size usersize = (_Size)456;
        static constexpr _Size publishersize = (_Size)684;

        struct _ID
        {
            _Openmode mode = notlogged;
            std::string id_string = "";
            _ID &init_id(std::string_view _id)
            {
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
                id_string = _id;
                return *this;
            }
        };

        struct _dimensions
        {
            size_t beg = 0;
            size_t end = 0;
            size_t space = 0;
            const char token = '-';
        };

        struct name
        {
            std::string firstname = "";
            std::string lastname = "";
            std::string fullname = "";
            std::string fullname_csv = "";
            std::string_view init_name(std::string_view name)
            {
                int _iter = name.find_first_of(",");
                firstname = std::string(name.substr(0, _iter));
                lastname = std::string(name.substr(_iter + 1));
                fullname = firstname + " " + lastname;
                fullname_csv = name;
                return fullname;
            }
        };

        static size_t rentable_books = 10;
        static size_t buyable_books = 10;

        class User
        { //holds the user and is responsible for login, logout and activity log -> gets automatically deleted with logout
        private:
            _Userstate m_state = goodbit;
            _Openmode m_mode = notlogged; //flag to indicate wheather user is logged in or not

            _dimensions m_dimensions;

            _ID m_ID; //User ID UXXXXXX
            name m_user_name;

            std::vector<csv::Row> mvec_books;
            std::vector<csv::Row> mvec_owned;
            std::vector<csv::Row> mvec_published;

            std::string m_path_userinfo{fm::init_workingdir() + "Data/Userinfo.txt"};
            csv::Header m_bookheader{"BID,DATE,POSITION"};
            std::fstream m_userinfo_txt;

            _Openmode &mf_authenticate_user(csv::CSVParser &);
            _Openmode &mf_create_user(csv::CSVParser &);
            _Openmode &mf_create_user_info();
            _Openmode &mf_load_userinfo();
            _Userstate &mf_set_state(const _Userstate &);
            _Openmode &mf_set_mode(const _Openmode &);
            std::string_view mf_create_ID(csv::CSVParser &, _Openmode &, size_t, size_t);

        public:
            User();
            ~User();

            _Userstate &add_book(Book &, bool);
            _Openmode &login();
            _Openmode &logout();

            const _Openmode &get_mode();
            const _Userstate &get_state();
            std::string_view get_name();
            const _ID &get_id();
            bool can_rent();
            bool can_buy();
        };

    }

}