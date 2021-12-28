#include "Library.h"

Library::Library() {}

Library::~Library() {}

bool Library::run_library()
{
    char input;
    std::string bookname = "";
    m_is_good = true;

    if (m_user.get_mode() == user::notlogged)
    {
        if (m_user.login())
        {
            log("================== WELCOME BACK ");
            log(m_user.get_name());
            log(" ===================\n");
        }
    }

    while (m_is_good && m_user.get_mode())
    {
        log("\n                           Menu\n");
        log(" [1] Rent a book (beta)            [2] Read a book (unavailable)\n");
        log(" [3] Show my books (unavailable)   [4] List books (unavailable)\n");
        log(" [5] Buy a book (beta)             [6] Return a book (unavailable)\n");
        log(" [7] Log out                       [8] Exit\n");
        log("\nWhat do you want to do (1-9)>>");

        std::cin >> input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (!std::cin.fail())
        {
            switch (input)
            {
            case '1':
                if (!m_user.can_rent())
                {
                    log("You can't rent more than ");
                    log(user::rentable_books);
                    log(" Books. Please return one first.\n");
                    break;
                }
                log("Enter Bookname: ");
                std::getline(std::cin, bookname);
                if (!mf_rent_or_buy_book(bookname, false))
                {
                    log("Book unavailable\n");
                    break;
                }
                log("Rented " + bookname);
                bookname.clear();
                break;
            case '5':
                if (!m_user.can_buy())
                {
                    log("You can't buy more than ");
                    log(user::buyable_books);
                    log(" Books. Please hold on untill more Books are supported.\n");
                    break;
                }
                log("Enter Bookname: ");
                std::getline(std::cin, bookname);
                if (!mf_rent_or_buy_book(bookname, true))
                {
                    log("Book unavailable\n");
                    break;
                }
                log("Bought " + bookname);
                bookname.clear();
                break;
            case '7':
                log("Logging out\n");
                m_user.logout();
                log("Logout successful. Press 'Enter'\n");
                return m_is_good = true;
            case '8':
                m_user.logout();
                return m_is_good = false;
            default:
                log("Function not supported.\n");
                break;
            }
        }
        else
        {
            std::cin.clear();
            log("Please Enter a valid number.\n");
        }
    }
    return m_is_good = false;
}

bool Library::mf_rent_or_buy_book(std::string_view _bookname, bool _buy = false)
{
    try
    {
        if (!mptr_book)
            mptr_book = new Book(m_inventory_csv.find_first_of(_bookname, "NAME"), &m_inventory_csv);
        else
            mptr_book->init(m_inventory_csv.find_first_of(_bookname, "NAME"));
    }
    catch (csv::Error &e)
    {
        return false;
    }

    if (mptr_book->is_available())
    {
        mptr_book->increase_rented();
        return m_user.add_book(*mptr_book, _buy);
    }

    return false;
}