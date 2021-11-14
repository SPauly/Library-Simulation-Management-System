#pragma once
#include "CSVParser.h"
#include "User.h"

class Library {
private:
    User* mptr_user;
    csv::CSVParser* mptr_csv_parser;
    csv::Header m_inventory_structure{"BID,NAME,AUTHOR,COPIES,RENTED,LINK"};
    std::string m_inventory_path{(fm::init_workingdir() + "Data\\Inventory.csv")};
public:
    Library() = delete;
    Library(User*);
    ~Library();

    void run_library();

};