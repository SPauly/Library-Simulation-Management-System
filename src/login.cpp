#include "login.h"

User::User(){
	//temporary allocated values
	mptr_username = new std::string;
	mptr_password = new std::string;

	//longer allocation
	mptr_csv_parser = new csv::CSVParser(&m_database);

};

User::~User(){
	delete mptr_csv_parser;
};

bool User::m_user_request(){
	int _tries = 0;
	while(_tries < 3){
		log("Username>> ");
		std::getline(std::cin, *mptr_username);
		log("Password>> ");
		std::getline(std::cin, *mptr_password);

		for(csv::_HEADER_TYPE i = 0; i < mptr_csv_parser->size(); i++){
			if(mptr_csv_parser->getRow(i)["USERNAME"] == *mptr_username){
				if(mptr_csv_parser->getRow(i)["PASSWORD"] == *mptr_password){
					log("Login successful\n");
					m_login_flag = true;
					return m_login_flag;
				}
			}
		}

		log("Wrong username or password.");
		++_tries;
	}

	return m_login_flag = false;
};

bool User::m_create_user(){
	csv::Row* _temp_rowptr = new csv::Row();
	std::cout<<std::endl;
	log("New Username>> ");
	std::getline(std::cin, *mptr_username);
	//regex check
	
	_temp_rowptr->add_value(*mptr_username);
	log("New Password>> ");
	std::getline(std::cin, *mptr_password);
	//regex check
	_temp_rowptr->add_value(*mptr_password);
	mptr_csv_parser->addRow(*_temp_rowptr);
};

bool User::login(){
	char _yn = 0;
	while (true)
	{
		log("Log into an existing Account? [y/n]\n>>");
		std::cin >> _yn;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		if (!std::cin.fail())
		{
			switch (_yn)
			{
			case 'y':
				if (!m_user_request())
				{
					log("Do you instead want to create a new Account? [y/n]\n>>");
					std::cin>>_yn;
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					if (!std::cin.fail())
					{
						switch (_yn)
						{
						case 'y':
							return m_create_user();
						case 'n':
							log("Login failed.");
							return m_login_flag = false;
						default:
						    log("Login failed.");
							return m_login_flag = false;
						}
					}				
				}
				else {
					//do some logging for user activity
					return m_login_flag = true;
				}
				break;
			case 'n':
				return m_create_user();
				break;
			default:
				log("Wrong input. Enter 'y' or 'n'.");
				break;
			}
		}
		else
		{
			log("Wrong input. Enter 'y' or 'n'.");
			std::cin.clear();
		}
	}

	return m_login_flag = false;
};