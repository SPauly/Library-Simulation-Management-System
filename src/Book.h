#pragma once
#include "CSVParser.h"
#include <string>

class Book{
private:
    csv::Row* mrow_info = nullptr;
public:
    Book() = delete;
    Book(csv::Row*);
    ~Book();
    int &increase_rented();

    bool is_available();
    std::string_view get_BID();
};