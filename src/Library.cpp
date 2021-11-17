#include "Library.h"

Library::Library(std::string_view _config){
    //establish login type and create user
    if(_config == "publisher"){
        m_config = 1;
        //mptr_user = new Publisher{}
    }   
    else if (_config == "admin"){
        m_config = 2;
        //mptr_user = new Admin{}
    }
    else{
        mptr_user = new User{};
    }

    //initialize Library files
    try{
        mptr_csv_parser = new csv::CSVParser(m_inventory_path, m_inventory_structure);
    }
    catch(csv::Error &e){
        log(e.what());
    }

    //log in User and get Userinfo ptr for Library
    mptr_userinfo = &mptr_user->login();

}

Library::~Library(){
    delete mptr_user;
    delete mptr_csv_parser;
}

bool Library::run_library() {
    char input;
    while(mptr_userinfo){
        log("\n                           Menu\n");
        log(" [1] Rent a book (unavailable)     [2] Read a book (unavailable)\n");
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
                case '8':
                    log("Logging out\n");
                    mptr_user->logout();
                    log("Logout successful. Press 'Enter'\n");
                    return m_is_good = true;
                case '9':
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