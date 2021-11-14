#include "User.h"

//Userinfo

Userinfo::Userinfo(const std::string *ptr_userfile_path){
	try
	{
		//open txtfile
		m_userinfo_txt.open(*ptr_userfile_path, std::ios::in | std::ios::out | std::ios::binary);
		m_userinfo_txt.exceptions(std::ifstream::badbit);


	}
	catch (const std::ifstream::failure &e)
	{
		throw csv::Error(std::string("CTOR: Error accessing Database: ").append(e.what()));
	}
}

bool Userinfo::create_user_info(std::string_view _ID){
	try
	{
		//get necessary info
		m_ID = _ID;
		std::string *ptr_first_name = new std::string;
		std::string *ptr_second_name = new std::string;

		log("Please enter your first name: ");
		std::getline(std::cin,*ptr_first_name);
		log("Please enter your second name: ");
		std::getline(std::cin, *ptr_second_name);

		m_user_name = *ptr_first_name + "," + *ptr_second_name;

		//write data to file
		m_userinfo_txt.seekg(0, std::ios_base::end);

		int position_place = m_userinfo_txt.tellg();
		m_userinfo_txt << "==============" << m_ID << "======\n";
		m_userinfo_txt << "Name:" << m_user_name << "\n";
		m_userinfo_txt << "Books:"
					   << "\n";
		m_userinfo_txt << "Owned:"
					   << "\n";
		if (m_ID.at(0) == 'P')
		{
			m_userinfo_txt << "Published:\n";
		}

		m_next_position = m_userinfo_txt.tellg();
		m_userinfo_txt.seekg(position_place);
		m_userinfo_txt << m_next_position;
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

bool Userinfo::load_userinfo(std::string_view _ID){

	//set userid
	m_ID = _ID;

	//try reading Userinfo.txt
	try
	{
		//set position at beginning and find the next user position while checking for the uid
		m_userinfo_txt.seekg(std::ios::beg);
		m_next_position = std::ios::beg;
		std::string line_tmp;
		std::string sub_tmp;
		std::string::size_type _StartIterator = 0;
		std::string::size_type _ItemIteratorPos = 0;

		try
		{
			do
			{
				m_userinfo_txt.seekg(m_next_position);
				std::getline(m_userinfo_txt, line_tmp);
				_ItemIteratorPos = line_tmp.find_first_of("~", _StartIterator);
				sub_tmp = std::string(line_tmp.substr(_StartIterator, _ItemIteratorPos - _StartIterator));
				m_next_position = std::stoi(sub_tmp);
				_StartIterator = 0;

			} while (line_tmp.find(_ID) == std::string::npos && m_userinfo_txt.is_open());
		}
		catch (const std::ifstream::failure &e)
		{
			log("\nIt seems this User does not yet exist. Please create a new one.\n");
			create_user_info(_ID);
			load_userinfo(_ID);
		};

		//read the name
		std::getline(m_userinfo_txt, line_tmp);
		_ItemIteratorPos = line_tmp.find_first_of(":");
		sub_tmp = std::string(line_tmp.substr(_ItemIteratorPos + 1));
		sub_tmp.at(sub_tmp.find_first_of(",")) = ' ';
		m_user_name = sub_tmp;
		std::getline(m_userinfo_txt, line_tmp);

		//read the books
		std::getline(m_userinfo_txt, line_tmp);
		while(line_tmp.at(0) == 'B'){
			mvec_books.push_back(csv::Row(line_tmp, &m_bookheader));
			std::getline(m_userinfo_txt, line_tmp);
		}

		//read the owned books
		try
		{
			std::getline(m_userinfo_txt, line_tmp);
			while (line_tmp.at(0) == 'B')
			{
				mvec_owned.push_back(csv::Row(line_tmp, &m_bookheader));
				std::getline(m_userinfo_txt, line_tmp);
			}
		}
		catch (const std::ifstream::failure &e)
		{}
		catch (const std::out_of_range &oor){}
	}
	catch (const std::ifstream::failure &e) //means something went wrong with reading
	{
		log("\nError loading userfile\n");
		return false;
	}
	catch (const std::invalid_argument &e){ //means stoi did get an invalid argument
		log("\nError reading next position in Userinfo\n");
		return false;
	}

	return true;

}

std::string_view Userinfo::get_name(){
	if(m_user_name != ""){
		return m_user_name;
	}
	else{
		m_user_name = "Username not loaded.";
		return m_user_name;
	}
}


//User

User::User(){
	//temporary allocated values
	mptr_username = new std::string;
	mptr_password = new std::string;

	//longer allocation
	mptr_csv_parser = new csv::CSVParser(&m_path_userfile);
	mptr_userinfo = new Userinfo(&m_path_userinfo);
	m_ID = "U";
};

User::~User(){
	delete mptr_csv_parser;
	delete mptr_userinfo;

	//temporary deleting them here later got to change that
	delete mptr_username;
	delete mptr_password;
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
					m_login_flag = mptr_userinfo->load_userinfo(mptr_csv_parser->getRow(i)["UID"]);
					return m_login_flag;
				}
			}
		}

		log("Wrong username or password.\n");
		++_tries;
	}

	return m_login_flag = false;
};

bool User::m_create_user(){
	csv::Row* _temp_rowptr = new csv::Row();
	std::regex _reg_username{"(?!.*,)(?=[a-zA-Z0-9._]{8,20}$)(?!.*[_.]{2})[^_.].*[^_.]$"};
	std::regex _reg_password{"(?!.*,)(?=.*?[A-Z])(?=.*?[a-z])(?=.*?[0-9])(?=.*?[#?!@$%^&*<>_]).{8,}"};
	
	//Get Username
	do
	{
		log("New Username>> ");
		std::getline(std::cin, *mptr_username);
		while(!std::regex_match(*mptr_username, _reg_username)){
			log("Username must contain 8-20 Characters, start and end with a character,	can contain numbers, '.' and '_'\n");
			log("New Username>> ");
			std::getline(std::cin, *mptr_username);
		}
		
		if(mptr_csv_parser->find_first_of(*mptr_username, "USERNAME")){
			log("Username already exists\n");
			continue;
		}
		else{
			break;
		}
	} while (true);
	_temp_rowptr->add_value(*mptr_username);

	//Get Password
	log("New Password>> ");
	std::getline(std::cin, *mptr_password);
	while (!std::regex_match(*mptr_password, _reg_password))
	{
		log("Password criteria:\n - min. 8 characters ");
		log("\n - One upper one lower case character \n - One number\n - one special character eg. #?!@$%^&*<>_\n");
		log("New Password>> ");
		std::getline(std::cin, *mptr_password);
	}
	_temp_rowptr->add_value(*mptr_password);

	//Create UID
	_temp_rowptr->add_value(m_create_ID(100000,999999));

	//write new User to Userfile and save in Parser
	mptr_csv_parser->addRow(*_temp_rowptr);

	//create Userinfo in Userinfo.txt and load it 
	if(	mptr_userinfo->create_user_info(m_ID) == true &&
	mptr_userinfo->load_userinfo(m_ID) == true){
		return true;
	}
	else
		return false;

};

std::string_view User::m_create_ID(int min, int max){
		std::uniform_int_distribution<> _distribution(min, max);
		std::random_device _random_dev;
		std::default_random_engine _generator(_random_dev());
		int _rand = _distribution(_generator);
	do
	{
		m_ID += std::to_string(_rand);
		if(mptr_csv_parser->find_first_of(m_ID, "UID")){
			char _token = m_ID.at(0);
			m_ID = _token;
			continue;
		}
		else{
			break;
		}

	} while (true);

	return m_ID;
}

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
							if (m_create_user() == true)
							{
								log("Registration complete.\n");
								log("Successfully logged in\n");
								log(">>>>>>>>>>>>>>>>>>>>>>  WELCOME BACK ");
								log(mptr_userinfo->get_name());
								log("  <<<<<<<<<<<<<<<<<<<<<<\n");
								return m_login_flag = true;
							}
							else {
								log("Failed to register new account.\n");
								return m_login_flag = false;
							}
						case 'n':
							log("Login failed.\n");
							return m_login_flag = false;
						default:
						    log("Login failed.\n");
							return m_login_flag = false;
						}
					}				
				}
				else {
					log("Successfully logged in\n");
					log(">>>>>>>>>>>>>>>>>>>>>>  WELCOME BACK ");
					log(mptr_userinfo->get_name());
					log("  <<<<<<<<<<<<<<<<<<<<<<\n");
					return m_login_flag = true;
				}
				break;
			case 'n':
				if (m_create_user() == true)
				{
					log("Registration complete.\n");
					log("Successfully logged in\n");
					log(">>>>>>>>>>>>>>>>>>>>>>  WELCOME BACK ");
					log(mptr_userinfo->get_name());
					log("  <<<<<<<<<<<<<<<<<<<<<<\n");
					return m_login_flag = true;
				}
				else
				{
					log("Failed to register new account.\n");
					return m_login_flag = false;
				}
			default:
				log("Wrong input. Enter 'y' or 'n'.\n");
				break;
			}
		}
		else
		{
			log("Wrong input. Enter 'y' or 'n'.\n");
			std::cin.clear();
		}
	}

	return m_login_flag = false;
};

bool User::is_logged(){
	return m_login_flag;
};

bool User::logout(){
	return m_login_flag = false;
};