#include "User.h"

//namespace user
namespace user
{
    //private Members
    _Openmode &User::mf_authenticate_user(csv::CSVParser& _parser)
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

            for (csv::_HEADER_TYPE i = 0; i < _parser.size(); i++)
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

        return m_ID.mode = failure;
    }

    _Openmode &User::mf_create_user(csv::CSVParser &_parser)
    {
        csv::Row _temp_row;
        std::regex _reg_username{"(?!.*,)(?=[a-zA-Z0-9._]{8,20}$)(?!.*[_.]{2})[^_.].*[^_.]$"};
        std::regex _reg_password{"(?!.*,)(?=.*?[A-Z])(?=.*?[a-z])(?=.*?[0-9])(?=.*?[#?!@$%^&*<>_]).{8,}"};
        std::string _username = "";
        std::string _password = "";
        int _type;
        _Openmode mode;

        //Ask for Type of login
        log("Do you want to create a USER[1] or a PUBLISHER[2] account? (ENTER for default)\n>>");
        std::cin >> _type;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (!std::cin.fail())
        {
            switch (_type)
            {
            case 2:
                log("Create Publisher Account:\n");
                mode = publisher;
                break;
            default:
                log("Create User Account:\n");
                mode = user;
                break;
            }
        }
        else {
            log("Create User Account:\n");
            mode = user;
        }

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
        _temp_row.add_value(m_ID._create_ID(mode, 100000, 999999));

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
            m_user_name.firstname = _first_name;
            m_user_name.lastname = _last_name;

            //write data to file
            m_userinfo_txt.seekg(0, std::ios_base::end);

            //initialize _dimensions beg
            m_dimensions.beg = m_userinfo_txt.tellg();
            
            //write to file
            m_userinfo_txt << "==============" << m_ID._string_id << "======\r\n";
            m_userinfo_txt << "Name:" << m_user_name.fullname_csv << "\r\n";
            m_userinfo_txt << "Books:"
                           << "\r\n";
            m_userinfo_txt << "Owned:"
                           << "\r\n";
            if (m_ID.mode == publisher)
            {
                m_userinfo_txt << "Published:\r\n";
            }

            //initialize _dimensions end
            m_dimensions.end = m_userinfo_txt.tellg();
            
            //finish writing
            m_userinfo_txt.seekg(m_dimensions.beg);
            m_userinfo_txt << m_dimensions.end;
            m_userinfo_txt << "~";
            m_userinfo_txt.flush();
        }
        catch (const std::ifstream::failure &e)
        {
            log("Error creating userfile\n");
            return false;
        }
        return true;
    }

    //public Members
    _Openmode &User::login()
    {
        try
        { 
            //initialize needed files
            csv::Header _userfile_header{"USERNAME,PASSWORD,UID"};
            std::string _path_userfile {fm::init_workingdir() + "Data\\Userfile.csv"};
            csv::CSVParser _userfile_csv{_path_userfile,_userfile_header};

            //login page
            char _yn = 0;
            while (true)
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
                            return this->m_mode;
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
                            return this->m_mode;
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

};