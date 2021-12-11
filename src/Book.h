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

    bool is_available();
    int &increase_rented();
};