#pragma once
#include "User.h"
struct BID : public user::_ID {
};

class Library {
private:
    bool m_is_good = true;
    user::User m_user{};
    
    csv::Header m_inventory_structure{"BID,NAME,AUTHOR,COPIES,RENTED,LINK"};
    std::string m_inventory_path{(fm::init_workingdir() + "Data\\Inventory.csv")};
    csv::CSVParser m_csv_parser{m_inventory_path, m_inventory_structure};
public:
    Library();
    ~Library();

    bool run_library();
};