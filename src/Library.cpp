#include "Library.h"

namespace LSMS
{
    Library::Library() {}

    Library::~Library() {}

    bool Library::mf_startup()
    {
        return InstanciateBookInstance(&m_inventory_csv);
    }

    bool Library::run()
    {
        if (!mf_startup())
            return false;
        m_running = true;

        char input;
        std::string bookname = "";
        while (m_running)
        {
            while (m_user.login() == user::notlogged)
            {
                if (!m_user.get_mode())
                    return m_running = false;
            }
            log("================== WELCOME BACK ");
            log(m_user.get_name());
            log(" ===================\n");

            while (m_user.get_mode() && m_user.get_mode() != user::notlogged)
            {
                log("\n                           Menu\n");
                log(" [1] Rent a book (beta)            [2] Read a book (unavailable)\n");
                log(" [3] Show my books (beta)          [4] List books (unavailable)\n");
                log(" [5] Buy a book (beta)             [6] Return a book (unavailable)\n");
                log(" [7] Log out                       [8] Exit\n");
                log("\nWhat do you want to do (1-8)>>");

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
                    case '3':
                    {
                        std::string temp = "";
                        log(this->mf_get_my_books(temp));
                        temp.clear();
                    }
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
                        m_running = true;
                        break;
                    case '8':
                        m_user.logout();
                        m_running = false;
                        break;
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
            system("cls");
        }
        return m_running;
    }

    bool Library::mf_rent_or_buy_book(std::string_view _bookname, bool _buy = false)
    {
        try
        {
            BOOK_PTR()->init(m_inventory_csv.find_first_of(_bookname, "NAME"));
        }
        catch (csv::Error &e)
        {
            return false;
        }

        if (BOOK_PTR()->is_available())
        {
            BOOK_PTR()->increase_rented();
            return m_user.add_book(*BOOK_PTR(), _buy);
        }

        return false;
    }

    std::string &Library::mf_get_my_books(std::string &_books)
    {
        _books += "\n\nRented Books: (Name, Author, Date)\n";
        _books += m_user.get_rented();
        _books += "\nOwned Books: (Name, Author)\n";
        _books += m_user.get_owned();
        if (m_user.get_mode() == user::publisher)
        {
            _books += "\nPublished Books: (Name, Date, Copies, Rented)\n";
            _books += m_user.get_published();
        }
        return _books;
    }

}