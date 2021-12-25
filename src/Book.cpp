#include "Book.h"

Book::Book(csv::Row* _ptr_row){
	init(_ptr_row);
}

Book::Book(csv::Row* _ptr_row, csv::CSVParser* _ptr_parser) : Book(_ptr_row){
    if (!_ptr_parser)
        throw csv::Error("Book: Parser points to nullptr");
    mptr_parser = _ptr_parser;
}

void Book::init(csv::Row *_ptr_row)
{
    if (!_ptr_row)
        throw csv::Error("Book: Row points to nullptr");
    mptr_info = _ptr_row;
}

Book::~Book(){
    delete mptr_info;
}

int Book::increase_rented(){
    size_t currently_rented = 0;
    int rented_length = 4; 
    std::string currently_rented_s(rented_length--, '0');

    try
    {
        currently_rented = std::stoi(mptr_info->getvalue("RENTED").data());
        ++currently_rented;
        //Format string to 0001
        for (int val = (currently_rented < 0) ? -currently_rented : currently_rented; rented_length >= 0 && val != 0; --rented_length, val /= 10)
            currently_rented_s[rented_length] = '0' + val % 10;
        if (rented_length >= 0 && currently_rented < 0)
            currently_rented_s[0] = '-';

    }
    catch (const std::invalid_argument &e)
    {
        return csv::npos;
    }

    if(mptr_info->change_value_in_to("RENTED", currently_rented_s))
        if(mptr_parser->updateRow(mptr_info))
            return currently_rented;
        else
            return csv::npos;
    else
        return csv::npos;
}

bool Book::is_available(){
    try{
        int copies = std::stoi(mptr_info->getvalue("COPIES").data());
        int rented = std::stoi(mptr_info->getvalue("RENTED").data());
        return copies - rented;
    }
    catch(const std::invalid_argument &){
        return false;
    }
    return false;
}

std::string_view Book::get_BID(){
    return mptr_info->getvalue(0);
}