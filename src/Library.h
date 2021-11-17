#pragma once
#include "CSVParser.h"
#include "User.h"

class Library {
private:
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