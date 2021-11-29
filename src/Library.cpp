#include "Library.h"

Library::Library(){}

Library::~Library(){}

bool Library::run_library() {
    char input;

    if(m_user.get_mode() == user::notlogged){
        m_user.login();
    }

    while(m_is_good && m_user.get_mode()){
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