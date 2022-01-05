#include "User.h"

namespace LSMS
{
    //namespace user
    namespace user
    {
        //private Members
        _Openmode &User::mf_authenticate_user(csv::CSVParser &_parser)
        {
            std::string _username = "";
            std::string _password = "";

            size_t _tries = 0;
            while (_tries < 3)
            {
                log("Username>> ");
                std::getline(std::cin, _username);
                log("Password>> ");
                std::getline(std::cin, _password);

                for (unsigned int i = 0; i < _parser.size(); i++)
                {
                    if (_parser.getRow(i)["USERNAME"] == _username)
                    {
                        if (_parser.getRow(i)["PASSWORD"] == _password)
                        {
                            m_ID.init_id(_parser.getRow(i)["UID"]);
                            return m_ID.mode;
                        }
                    }
                }

                log("Wrong username or password.\n");
                ++_tries;
            }

            return mf_set_mode(failure);
        }

        _Openmode &User::mf_create_user(csv::CSVParser &_parser)
        {
            csv::Row _temp_row;
            std::regex _reg_username{"(?!.*,)(?=[a-zA-Z0-9._]{8,20}$)(?!.*[_.]{2})[^_.].*[^_.]$"};
            std::regex _reg_password{"(?!.*,)(?=.*?[A-Z])(?=.*?[a-z])(?=.*?[0-9])(?=.*?[#?!@$%^&*<>_]).{8,}"};
            std::string _username = "";
            std::string _password = "";
            char _type;
            _Openmode mode;

            //Ask for Type of login
            log("Do you want to create a USER[1] or a PUBLISHER[2] account? (ENTER for default)\n>>");
            std::cin >> _type;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (!std::cin.fail())
            {
                switch (_type)
                {
                case '2':
                    log("Create Publisher Account:\n");
                    mode = publisher;
                    break;
                default:
                    log("Create User Account:\n");
                    mode = user;
                    break;
                }
            }
            else
            {
                log("Create User Account:\n");
                mode = user;
            }
            // set mode to logged in
            m_mode = mode;

            //Get Username
            do
            {
                log("New Username>> ");
                std::getline(std::cin, _username);
                while (!std::regex_match(_username, _reg_username))
                {
                    log("Username must contain 8-20 Characters, start and end with a character,	can contain numbers, '.' and '_'\n");
                    log("New Username>> ");
                    std::getline(std::cin, _username);
                }

                if (_parser.find_first_of(_username, "USERNAME"))
                {
                    log("Username already exists\n");
                    continue;
                }
                else
                {
                    break;
                }
            } while (true);
            _temp_row.add_value(_username);

            //Get Password
            log("New Password>> ");
            std::getline(std::cin, _password);
            while (!std::regex_match(_password, _reg_password))
            {
                log("Password criteria:\n - min. 8 characters ");
                log("\n - One upper one lower case character \n - One number\n - one special character eg. #?!@$%^&*<>_\n");
                log("New Password>> ");
                std::getline(std::cin, _password);
            }
            _temp_row.add_value(_password);

            //Create UID
            _temp_row.add_value(mf_create_ID(_parser, mode, 100000, 999999));

            //write new User to Userfile and save in Parser
            _parser.addRow(_temp_row);

            //create Userinfo
            return mf_create_user_info();
        }

        _Openmode &User::mf_create_user_info()
        {
            try
            {
                //get necessary info
                std::string _first_name = "";
                std::string _last_name = "";

                log("Please enter your FIRST name: ");
                std::getline(std::cin, _first_name);
                log("Please enter your LAST name: ");
                std::getline(std::cin, _last_name);

                //initialize name m_user_name
                m_user_name.init_name(_first_name + "," + _last_name);

                //write data to file
                m_userinfo_txt.seekg(0, std::ios_base::end);

                //initialize _dimensions beg
                m_dimensions.beg = m_userinfo_txt.tellg();

                //write to file
                m_userinfo_txt << "==============" << m_ID.id_string << "======\r\n";
                m_userinfo_txt << "Name:" << m_user_name.fullname_csv << "\r\n";
                m_userinfo_txt << "Books:"
                               << "\r\n";
                for (int i = 0; i < usersize / 2; i++)
                {
                    m_userinfo_txt << "-";
                }
                m_userinfo_txt << "\r\n";
                m_userinfo_txt << "Owned:"
                               << "\r\n";
                for (int i = 0; i < usersize / 2; i++)
                {
                    m_userinfo_txt << "-";
                }
                m_userinfo_txt << "\r\n";
                if (m_ID.mode == publisher)
                {
                    m_userinfo_txt << "Published:\r\n";
                    for (int i = 0; i < publishersize / 3; i++)
                    {
                        m_userinfo_txt << "-";
                    }
                    m_userinfo_txt << "\r\n";
                }

                //initialize _dimensions end
                m_dimensions.end = m_userinfo_txt.tellg();

                //finish writing
                m_userinfo_txt.seekg(m_dimensions.beg);
                m_userinfo_txt << m_dimensions.end;
                m_userinfo_txt << "~";
                m_userinfo_txt.flush();

                //finish initialization of dimensions
                m_dimensions.space = m_dimensions.end - m_dimensions.beg;
            }
            catch (const std::ifstream::failure &e)
            {
                log("Error creating userfile\n");
                mf_set_state(failbit);
                mf_set_mode(failure);
                return m_mode;
            }

            return m_mode;
        }

        _Openmode &User::mf_load_userinfo()
        {
            if (!m_mode)
            {
                return m_mode;
            }

            //try reading Userinfo.txt
            try
            {
                //stack variables
                int start_position = 0;
                int next_position = 0;

                //set position at beginning and find the next user position while checking for the uid
                m_userinfo_txt.seekg(std::ios::beg);
                next_position = std::ios::beg;
                std::string line_tmp;
                std::string sub_tmp;
                std::string::size_type _StartIterator = 0;
                std::string::size_type _ItemIteratorPos = 0;

                try
                {
                    do
                    {
                        m_userinfo_txt.seekg(next_position);
                        fm::_getline(m_userinfo_txt, line_tmp);
                        _ItemIteratorPos = line_tmp.find_first_of("~", _StartIterator);
                        sub_tmp = std::string(line_tmp.substr(_StartIterator, _ItemIteratorPos - _StartIterator));
                        start_position = next_position;
                        next_position = std::stoi(sub_tmp);
                        _StartIterator = 0;

                    } while (line_tmp.find(m_ID.id_string) == std::string::npos && m_userinfo_txt.is_open());
                }
                catch (const std::ifstream::failure &e)
                {
                    log("\nIt seems this User does not yet exist. Please create a new one.\n");
                    return mf_create_user_info();
                };
                //initialize dimensions of user
                m_dimensions.beg = start_position;
                m_dimensions.end = next_position;
                m_dimensions.space = next_position - start_position;

                //read the name
                fm::_getline(m_userinfo_txt, line_tmp);
                _ItemIteratorPos = line_tmp.find_first_of(":");
                sub_tmp = std::string(line_tmp.substr(_ItemIteratorPos + 1));
                m_user_name.init_name(sub_tmp);
                fm::_getline(m_userinfo_txt, line_tmp); //this line can be skiped since it only says Books:

                //read the books
                fm::_getline(m_userinfo_txt, line_tmp);
                while (line_tmp.at(0) == 'B')
                {
                    mvec_books.push_back(csv::Row(line_tmp, &m_bookheader));
                    fm::_getline(m_userinfo_txt, line_tmp);
                }

                //read the owned books
                try
                {
                    if (line_tmp.at(0) == '-')
                        fm::_getline(m_userinfo_txt, line_tmp);
                    fm::_getline(m_userinfo_txt, line_tmp); //skip the line with Owned:
                    while (line_tmp.at(0) == 'B')
                    {
                        mvec_owned.push_back(csv::Row(line_tmp, &m_bookheader));
                        fm::_getline(m_userinfo_txt, line_tmp);
                    }
                }
                catch (const std::ifstream::failure &e)
                {
                    m_userinfo_txt.clear();
                }
                catch (const std::out_of_range &oor)
                {
                    m_userinfo_txt.clear();
                }

                //read published books if necessary
                if (m_mode == publisher)
                {
                    try
                    {
                        if (line_tmp.at(0) == '-')
                            fm::_getline(m_userinfo_txt, line_tmp);
                        fm::_getline(m_userinfo_txt, line_tmp); //skip the line with Published:
                        while (line_tmp.at(0) == 'B')
                        {
                            mvec_published.push_back(csv::Row(line_tmp, &m_bookheader));
                            fm::_getline(m_userinfo_txt, line_tmp);
                        }
                    }
                    catch (const std::ifstream::failure &e)
                    {
                        m_userinfo_txt.clear();
                    }
                    catch (const std::out_of_range &oor)
                    {
                        m_userinfo_txt.clear();
                    }
                }
            }
            catch (const std::ifstream::failure &e) //means something went wrong with reading
            {
                log("Error loading userfile\n");
                mf_set_state(failbit);
                mf_set_mode(failure);
                return m_mode;
            }
            catch (const std::invalid_argument &e)
            { //means stoi did get an invalid argument
                log("Error reading next position in Userinfo\n");
                mf_set_state(failbit);
                mf_set_mode(failure);
                return m_mode;
            }
            catch (const std::out_of_range &e)
            { //means stoi did get an invalid argument
                log("out_of_range reading in Userinfo\n");
                mf_set_state(failbit);
                mf_set_mode(failure);
                return m_mode;
            }

            return m_mode;
        }

        _Userstate &User::mf_set_state(const _Userstate &_state)
        {
            switch (_state)
            {
            case goodbit:
                this->m_state = goodbit;
                break;
            case incobit:
                this->m_state = incobit;
                break;
            case badbit:
                this->m_state = badbit;
                break;
            default:
                this->m_state = failbit;
                break;
            }

            return m_state;
        }

        _Openmode &User::mf_set_mode(const _Openmode &_mode)
        {
            switch (_mode)
            {
            case user:
                this->m_mode = user;
                break;
            case publisher:
                this->m_mode = publisher;
                break;
            case admin:
                this->m_mode = admin;
                break;
            case notlogged:
                this->m_mode = notlogged;
                break;
            default:
                this->m_mode = failure;
                break;
            }

            return m_mode;
        }

        std::string_view User::mf_create_ID(csv::CSVParser &_parser, _Openmode &_mode, size_t _min, size_t _max)
        {
            std::uniform_int_distribution<> _distribution(_min, _max);
            std::random_device _random_dev;
            std::default_random_engine _generator(_random_dev());
            int _rand = _distribution(_generator);
            std::string temp = "";

            switch (_mode)
            {
            case failure:
                throw Error("User: Create ID: usermode set to 'failure'");
                break;
            case notlogged:
                throw Error("User: Create ID: usermode set to 'notlogged'");
                break;
            default:
                temp += _mode;
                break;
            }

            do
            {
                temp += std::to_string(_rand);
                if (_parser.find_first_of(temp, "UID"))
                {
                    char _token = temp.at(0);
                    temp = _token;
                    continue;
                }
                else
                {
                    break;
                }

            } while (true);

            m_ID.init_id(temp);

            return m_ID.id_string;
        }

        //public Members
        User::User()
        {
            m_userinfo_txt.exceptions(std::ifstream::failbit);
            try
            {
                //open txtfile
                m_userinfo_txt.open(m_path_userinfo, std::ios::in | std::ios::out | std::ios::binary);
            }
            catch (const std::ifstream::failure &e)
            {
                try
                {
                    m_userinfo_txt.open(m_path_userinfo, std::ios::in | std::ios::out | std::ios::binary | std::ios::app);
                    m_userinfo_txt.close();
                    m_userinfo_txt.open(m_path_userinfo, std::ios::in | std::ios::out | std::ios::binary);
                }
                catch (const std::ifstream::failure &e)
                {
                    throw Error(std::string("User: Error creating new Database: ").append(e.what()));
                }
                log("User: Initilized new database\n");
            }
        }

        User::~User()
        {
            mf_set_mode(notlogged);
            m_userinfo_txt.flush();
            m_userinfo_txt.close();
        }

        _Userstate &User::add_book(Book &_book, bool _buy = false)
        {
            if (!this->can_rent() && !_buy || !this->can_buy() && _buy)
            {
                return mf_set_state(failbit);
            }
            std::string bookentry = "";
            bookentry += _book.get_BID();
            bookentry += ',';
            //add date
            char datebuf[libtime::MAXDATELENGTH] = {NULL};
            libtime::getdate_mmddyyyy(datebuf);
            bookentry.append(datebuf);
            bookentry += ',';
            //reading pos = begin
            bookentry += "0000";

            try
            {
                char finder = 0;
                std::string temp_line;
                m_userinfo_txt.seekg(m_dimensions.beg);
                m_userinfo_txt.get(finder);
                //skip lines as long as it doesn't start with - or if we want to find 'Owned' O
                while ((finder != '-' && finder != 'O') || (_buy && finder != 'O'))
                {
                    std::getline(m_userinfo_txt, temp_line);
                    m_userinfo_txt.get(finder);
                }
                //if we want to find the Owned section skip lines till we find - or reach the end
                while ((_buy && finder != '-') && (finder != '~' && finder != 'P'))
                {
                    std::getline(m_userinfo_txt, temp_line);
                    m_userinfo_txt.get(finder);
                }

                if (fm::fast_insert(m_userinfo_txt, bookentry, m_userinfo_txt.tellg(), m_dimensions.beg, m_dimensions.end, '-') != fm::npos)
                {
                    if (!_buy)
                        mvec_books.push_back(csv::Row(_book.get_Row())); //temporaryly only add the row here
                    else
                        mvec_owned.push_back(csv::Row(_book.get_Row()));
                }
                temp_line.clear();
            }
            catch (std::ios::failure &e)
            {
                return mf_set_state(failbit);
            }

            return m_state;
        }

        _Openmode &User::login()
        {
            try
            {
                //initialize needed files
                csv::Header _userfile_header{"USERNAME,PASSWORD,UID"};
                std::string _path_userfile{fm::init_workingdir() + "Data/Userfile.csv"};
                csv::CSVParser _userfile_csv{_path_userfile, _userfile_header};

                //login page
                char _yn = 0;
                while (m_state)
                {
                    log("Log into an existing Account? [y/n/e]\n>>");
                    std::cin >> _yn;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    if (!std::cin.fail())
                    {
                        switch (_yn)
                        {
                        case 'y':
                            if (mf_set_mode(mf_authenticate_user(_userfile_csv)))
                            {
                                log("Successfully logged in\n");
                                log("Read Userinfo...\n");
                                return mf_load_userinfo();
                            }
                            else
                            {
                                log("Failed to log in. Try again\n");
                            }
                            break;

                        case 'n':
                            if (mf_set_mode(mf_create_user(_userfile_csv)))
                            {
                                log("Registration complete\n");
                                log("Successfully logged in\n");
                                return m_mode;
                            }
                            else
                            {
                                log("Failed to register new account. Try again\n");
                            }
                            break;

                        case 'e':
                            return mf_set_mode(failure);

                        default:
                            log("Wrong input. Enter 'y' or 'n'. 'e' for exit\n");
                            break;
                        }
                    }
                    else
                    {
                        log("Wrong input. Enter 'y' or 'n'. 'e' for exit\n");
                        std::cin.clear();
                    }
                }
            }
            catch (const csv::Error &e)
            {
            }
        }

        const _Openmode &User::get_mode()
        {
            return m_mode;
        }

        _Openmode &User::logout()
        {
            mvec_books.clear();
            mvec_owned.clear();
            mvec_published.clear();
            mf_set_mode(notlogged);
            return m_mode;
        }

        const _Userstate &User::get_state()
        {
            return m_state;
        }

        std::string_view User::get_name()
        {
            return m_user_name.fullname;
        }

        const _ID &User::get_id()
        {
            return m_ID;
        }

        std::string User::get_rented()
        {
            std::string rented = "";
            for (int i = 0; i < mvec_books.size(); i++)
            {
                BOOK_PTR()->init(&mvec_books.at(i));
                rented += BOOK_PTR()->get_public_info();
                rented += ", ";
                try
                {
                    rented += mvec_books.at(i).getvalue("DATE");
                }
                catch (csv::Error &e)
                {
                    rented += "now";
                }
                rented += '\n';
            }
            return rented;
        }

        std::string User::get_owned()
        {
            std::string owned = "";
            for (int i = 0; i < mvec_owned.size(); i++)
            {
                BOOK_PTR()->init(&mvec_owned.at(i));
                owned += BOOK_PTR()->get_public_info();
                owned += ", ";
                try
                {
                    owned += mvec_owned.at(i).getvalue("DATE");
                }
                catch (csv::Error &e)
                {
                    owned += "now";
                }
                owned += '\n';
            }
            return owned;
        }

        std::string User::get_published()
        {
            std::string published = "";
            for (int i = 0; i < mvec_published.size(); i++)
            {
                BOOK_PTR()->init(&mvec_published.at(i));
                published += BOOK_PTR()->get_insight_info();
                published += ", ";
                try
                {
                    published += mvec_published.at(i).getvalue("DATE");
                }
                catch (csv::Error &e)
                {
                    published += "now";
                }
                published += '\n';
            }
            return published;
        }

        bool User::can_rent()
        {
            return mvec_books.size() < rentable_books;
        }
        bool User::can_buy()
        {
            return mvec_owned.size() < buyable_books;
        }

        csv::Row *User::has_book(std::string_view _bookname)
        {
            for (int i = 0; i < mvec_books.size(); i++)
            {
                BOOK_PTR()->init(&mvec_books.at(i));
                if (BOOK_PTR()->get_NAME() == _bookname)
                {
                    return &mvec_books.at(i);
                }
            }
            for (int i = 0; i < mvec_owned.size(); i++)
            {
                BOOK_PTR()->init(&mvec_owned.at(i));
                if (BOOK_PTR()->get_NAME() == _bookname)
                {
                    return &mvec_owned.at(i);
                }
            }
            for (int i = 0; i < mvec_published.size(); i++)
            {
                BOOK_PTR()->init(&mvec_published.at(i));
                if (BOOK_PTR()->get_NAME() == _bookname)
                {
                    return &mvec_published.at(i);
                }
            }

            return nullptr;
        }

        size_t User::change_position_in_book(size_t _pos, std::string_view _bookname)
        {
            size_t _position = _pos;
            int format_length = 4;
            std::string _position_s(format_length--, '0');

            try
            {
                //Format string to 0001
                for (int val = (_position < 0) ? -_position : _position; format_length >= 0 && val != 0; --format_length, val /= 10)
                    _position_s[format_length] = '0' + val % 10;
                if (format_length >= 0 && _position < 0)
                    _position_s[0] = '-';
            }
            catch (const std::invalid_argument &e)
            {
                return csv::npos;
            }

            csv::Row *current_book = this->has_book(_bookname);
            current_book->change_value_in_to("POSITION", _position_s);

            if (m_userinfo_txt.good() && m_userinfo_txt.is_open())
            {
                size_t last_pos = m_dimensions.beg;
                std::string tmp;
                char finder = 0;

                m_userinfo_txt.seekg(m_dimensions.beg);
                std::getline(m_userinfo_txt, tmp); //skip initial line
                std::getline(m_userinfo_txt, tmp); //skip Name: line
                while (tmp != BOOK_PTR()->get_BID())
                {
                    tmp.clear();
                    last_pos = m_userinfo_txt.tellg();
                    while (finder != '\n' )
                    {
                        m_userinfo_txt.get(finder);
                        if(finder == ',')
                            break;
                        tmp += finder;
                    }
                    finder = 0;
                }
                m_userinfo_txt.seekp(last_pos);
                m_userinfo_txt << current_book->string();
                m_userinfo_txt.flush();
            }
            else
            {
                return 0;
            }
            return _pos;
        }
    } //end user

} //end user
