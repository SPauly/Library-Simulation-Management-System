#include "Library.h"

Library::Library(std::string_view _config){
    mptr_user = new User{};
    try{
        mptr_csv_parser = new csv::CSVParser(m_inventory_path, m_inventory_structure);
    }
    catch(csv::Error &e){
            log(e.what());
    }
}

Library::~Library(){
    delete mptr_csv_parser;
}

bool Library::run_library() {
    mptr_user->login();
    char input;
    while(mptr_user->is_logged()){
        log("\n                           Menu\n");
        log(" [1] Rent a book (unavailable)     [2] Read a book (unavailable)\n");
        log(" [4] Show my books (unavailable)   [5] List books (unavailable)\n");
        log(" [6] Buy a book (unavailable)      [7] Return a book (unavailable)\n");
        log(" [8] Log out                       [9] Exit (unavailable)\n");
        log("\nWhat do you want to do (1-9)>>");

        std::cin >> input;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		if (!std::cin.fail())
		{
			switch (input)
			{
                case '8':
                    log("Logging out\n");
                    if(!mptr_user->logout()){
                        log("Logout successful. Press 'Enter'\n");
                        return m_is_good = true;
                    }
                    else{
                        log("Failed to logout\n");
                    }   
                    break;
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
    return m_is_good = true;
}