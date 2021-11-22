#include "User.h"

//namespace user
namespace user
{
    _Openmode &User::login()
    {
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
                    if (mf_set_mode(mf_authenticate_user()))
                    {
                        log("Successfully logged in\n");
                        log(">>>>>>>>>>>>>>>>>>>>>>  WELCOME BACK ");
                        log(this->get_name());
                        log("  <<<<<<<<<<<<<<<<<<<<<<\n");
                        return this->m_mode;
                    }
                    else
                    {
                        log("Failed to log in.\n");
                    }
                    break;

                case 'n':
                    if (mf_set_mode(mf_create_user()))
                    {
                        log("Registration complete.\n");
                        log("Successfully logged in\n");
                        log(">>>>>>>>>>>>>>>>>>>>>>  WELCOME BACK ");
                        log(this->get_name());
                        log("  <<<<<<<<<<<<<<<<<<<<<<\n");
                        return this->m_mode;
                    }
                    else
                    {
                        log("Failed to register new account.\n");
                    }
                    break;

                case 'e':
                    return this->m_mode = failure;

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

};