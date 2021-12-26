#pragma once
#include "User.h"
#include "Book.h"

namespace LSMS
{

    class Library
    {
    private:
        bool m_running = false;
        user::User m_user{};
        Book *mptr_book = nullptr;

        csv::Header m_inventory_structure{"BID,NAME,AUTHOR,COPIES,RENTED,LINK"};
        std::string m_inventory_path{(fm::init_workingdir() + "Data/Inventory.csv")};
        csv::CSVParser m_inventory_csv{m_inventory_path, m_inventory_structure};

        bool mf_sync_lib();
        bool mf_rent_book(std::string_view);

    public:
        Library();
        ~Library();

        bool run();
    };

}