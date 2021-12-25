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

int &Book::increase_rented(){
    if(mptr_info->change_value_in_to("RENTED", "0020"))
        mptr_parser->updateRow(mptr_info);
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