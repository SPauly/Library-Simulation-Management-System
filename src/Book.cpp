#include "Book.h"

Book::Book(csv::Row* _ptr_row){
    if(!_ptr_row)
        throw csv::Error("");
    mrow_info = _ptr_row;
}

Book::~Book(){
    delete mrow_info;
}

int &Book::increase_rented(){
}

bool Book::is_available(){
    try{
        int copies = std::stoi(mrow_info->getvalue("COPIES").data());
        int rented = std::stoi(mrow_info->getvalue("RENTED").data());
        return copies - rented;
    }
    catch(const std::invalid_argument &){
        return false;
    }
    return false;
}

std::string_view Book::get_BID(){
    return mrow_info->getvalue(0);
}