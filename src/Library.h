#pragma once
#include "CSVParser.h"
#include "User.h"


class Library {
private:
    User* mptr_user;
    csv::CSVParser* mptr_csv_parser;
    std::string m_inventory{"E:/Simon/Documents/Visual Studio 2017/Projects/GetIntoCPPagain/Data/Inventory.csv"};
public:
    Library() = delete;
    Library(User*);
    ~Library();

    void run_library();

};