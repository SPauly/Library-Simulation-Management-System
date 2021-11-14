#pragma once
#include "CSVParser.h"
#include "file_management.h"
#include "User.h"

class Library {
private:
    User* mptr_user;
    csv::CSVParser* mptr_csv_parser;
    std::string m_inventory{(fm::init_workingdir() + "Data\\Inventory.csv")};
public:
    Library() = delete;
    Library(User*);
    ~Library();

    void run_library();

};