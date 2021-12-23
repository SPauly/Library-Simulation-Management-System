#include "Library.h"

Library::Library(){}

Library::~Library(){}

bool Library::run_library() {
    char input;
    std::string bookname = "";
    if(m_user.get_mode() == user::notlogged){
        if(m_user.login()){
            log("================== WELCOME BACK ");
            log(m_user.get_name());
            log(" ===================\n");
        }
    }

    while(m_is_good && m_user.get_mode()){
        log("\n                           Menu\n");
        log(" [1] Rent a book (beta)            [2] Read a book (unavailable)\n");
        log(" [4] Show my books (unavailable)   [5] List books (unavailable)\n");
        log(" [6] Buy a book (unavailable)      [7] Return a book (unavailable)\n");
        log(" [8] Log out                       [9] Exit\n");
        log("\nWhat do you want to do (1-9)>>");

        std::cin >> input;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		if (!std::cin.fail())
		{
			switch (input)
			{
                case '1':
                    log("Enter Bookname: ");
                    std::getline(std::cin, bookname);
                    if(!rent_book(bookname)){
                        log("Book unavailable\n");
                        break;
                    }
                    log("Rented " + bookname);
                    break;
                case '8':
                    log("Logging out\n");
                    m_user.logout();
                    log("Logout successful. Press 'Enter'\n");
                    return m_is_good = true;
                case '9':
                    m_user.logout();
                    return m_is_good = false;
                default:
                    log("Function not supported.\n");
                    break;
            }
        }
        else {
            std::cin.clear();
            log("Please Enter a valid number.\n");
        }
    }
    return m_is_good = false;
}

bool Library::rent_book(std::string_view _bookname){    
    try{
        if(!mptr_book)
            mptr_book = new Book(m_inventory_csv.find_first_of(_bookname, "NAME"));
        mptr_book->init(m_inventory_csv.find_first_of(_bookname, "NAME"));
    }
    catch(csv::Error &e){
        return false;
    }

    if(mptr_book->is_available()){
        mptr_book->increase_rented();
        return m_user.add_book(*mptr_book);
    }

    return false;
}