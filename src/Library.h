#pragma once
#include "User.h"
#include "Book.h"
#include <vector>

class Library {
private:
    bool m_is_good = true;
    user::User m_user{};
    Book* mptr_book = nullptr;
    
    csv::Header m_inventory_structure{"BID,NAME,AUTHOR,COPIES,RENTED,LINK"};
    std::string m_inventory_path{(fm::init_workingdir() + "Data/Inventory.csv")};
    csv::CSVParser m_inventory_csv{m_inventory_path, m_inventory_structure};

    bool mf_sync_lib();
    bool mf_rent_or_buy_book(std::string_view, bool); //default: false = rent, true = buy
public:
    Library();
    ~Library();

    bool run_library();
    
};