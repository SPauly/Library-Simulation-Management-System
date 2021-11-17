#pragma once
#include "CSVParser.h"
#include "User.h"

using CONFIG_TYPE = unsigned int;

class Library {
private:
    CONFIG_TYPE m_config = 0; //0 = User, 1 = Publisher, 2 = Admin
    bool m_is_good = true;
    User* mptr_user = nullptr;
    Userinfo* mptr_userinfo = nullptr;
    csv::CSVParser* mptr_csv_parser = nullptr;
    csv::Header m_inventory_structure{"BID,NAME,AUTHOR,COPIES,RENTED,LINK"};
    std::string m_inventory_path{(fm::init_workingdir() + "Data\\Inventory.csv")};
public:
    Library() = delete;
    Library(std::string_view);
    ~Library();

    bool run_library();
};