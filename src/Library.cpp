#include "Library.h"

namespace lsms
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
            while (m_user.login() == user::notlogged && m_user.get_mode())
            {
            }
            if (m_user.get_mode() == user::failure)
            {
                return m_running = false;
            }

            log("================== WELCOME BACK ");
            log(m_user.get_name());
            log(" ===================\n");

            while (m_user.get_mode() && m_user.get_mode() != user::notlogged)
            {
                log("\nPRESS ENTER TO GET TO THE MENU\n");
                std::cin.get();
                log("\n---------------------------- MENU -----------------------------\n");
                log(" [1] Rent a book                   [2] Read a book (beta)\n");
                log(" [3] Show my books                 [4] List books\n");
                log(" [5] Buy a book                    [6] Return a book\n");
                log(" [7] Log out                       [8] Exit\n");
                log("\nWhat do you want to do? (1-8)>>");

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
                    case '2':
                        bookname.clear();
                        log("Enter Bookname: ");
                        std::getline(std::cin, bookname);
                        mf_read_book(bookname);
                        break;
                    case '3':
                    {
                        std::string temp = "";
                        log(this->mf_get_my_books(temp));
                        temp.clear();
                    }
                    break;
                    case '4':
                    {
                        std::string temp{""};
                        log(mf_list_books(temp));
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
                    case '6':
                        log("Enter Book you want to return: ");
                        std::getline(std::cin, bookname);
                        mf_return_book(bookname);
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
        _books += "\n\nRented Books: (Book Name, Author, Date)\n";
        _books += m_user.get_rented();
        _books += "\nOwned Books: (Book Name, Author, Date)\n";
        _books += m_user.get_owned();
        if (m_user.get_mode() == user::publisher)
        {
            _books += "\nPublished Books: (Book Name, Date, Copies, Rented)\n";
            _books += m_user.get_published();
        }
        return _books;
    }

    std::string &Library::mf_list_books(std::string &_list)
    {
        _list = "\n---------All available Books---------\n";
        try
        {
            for (unsigned int i = 0; i < m_inventory_csv.size(); i++)
            {
                _list += m_inventory_csv.getRow(i).getvalue("NAME");
                _list += "\n    Author: ";
                _list += m_inventory_csv.getRow(i).getvalue("AUTHOR");
                _list += "  Copies: ";
                _list += m_inventory_csv.getRow(i).getvalue("COPIES");
                _list += "  Rented: ";
                _list += m_inventory_csv.getRow(i).getvalue("RENTED");
                _list += "\n";
            }
        }
        catch (csv::Error &e)
        {
            _list += "Failed to read Database due to: ";
            _list += e.what();
        }

        return _list;
    }

    bool Library::mf_read_book(std::string_view _bookname)
    {
        std::string tmp = "";
        std::cin.clear();
        try
        {
            BOOK_PTR()->init(m_user.has_book(_bookname));
            log("\n----------- Reading Mode: " << _bookname << " ------------\n");

            do
            {
                tmp.clear();
                log("Reading Position: " << BOOK_PTR()->get_pos() << "\n\n");
                log(BOOK_PTR()->get_current_page());
                log("\nPress ENTER for next page. (e + ENTER for exit):\n");
                std::getline(std::cin, tmp);
                if (std::cin.fail())
                {
                    std::cin.clear();
                    break;
                }
            } while (tmp == "" && m_user.change_position_in_book(BOOK_PTR()->get_pos() + 1, _bookname));

            return true;
        }
        catch (csv::Error &e)
        {
            log("Could not find the book in your inventory.\n");
            return false;
        }
    }

    void Library::mf_return_book(std::string_view _bookname)
    {
        try
        {
            if (m_user.remove_book(_bookname))
            { //remove book initializes BOOK_PTR()
                if (BOOK_PTR()->increase_rented(-1))
                    log("Returned book successfully\n");
                else
                    log("Failed to return to inventory\n");
            }
            else
                log("Failed to remove from Account\n");
        }
        catch (csv::Error &e)
        {
            log("Could not find the book in your inventory.\n");
        }
    }

}