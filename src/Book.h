#pragma once
#include "CSVParser.h"
#include <string>

class Book{
private:
    csv::Row* mptr_info = nullptr;
    csv::CSVParser* mptr_parser = nullptr;
public:
    Book() = delete;
    Book(csv::Row*);
    Book(csv::Row*, csv::CSVParser*);
    ~Book();
    
    void init(csv::Row*);

    size_t increase_rented();

    bool is_available();
    std::string_view get_BID();
    csv::Row& get_Row();
};