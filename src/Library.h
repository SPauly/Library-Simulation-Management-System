#pragma once
#include "User.h"
#include "Book.h"

namespace LSMS
{
    
#ifndef BOOK_PTR()
#define BOOK_PTR() GetBookInstance()
#endif
#ifndef log
#define log(x) std::cout << x
#endif

    class Library
    {
    private:
        bool m_running = false;
        user::User m_user{};

        csv::Header m_inventory_structure{"BID,NAME,AUTHOR,COPIES,RENTED,LINK"};
        std::string m_inventory_path{(fm::init_workingdir() + "Data/Inventory.csv")};
        csv::CSVParser m_inventory_csv{m_inventory_path, m_inventory_structure};

        bool mf_sync_lib();
        bool mf_rent_or_buy_book(std::string_view, bool); //default: false = rent, true = buy
        std::string &mf_get_my_books(std::string &);
        bool mf_startup();

    public:
        Library();
        ~Library();

        bool run();
    };

}
