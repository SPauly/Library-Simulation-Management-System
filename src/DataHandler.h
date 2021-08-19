#include "user.h"
#include <fstream>
#include <vector>

class DataHandler{
    std::string data_directory = "E:/Simon/Documents/Visual Studio 2017/Projects/GetIntoCPPagain/Data/Data.csv";
    std::ifstream read_data_file;
    std::ofstream write_data_file;
    user_id user;
    std::string temp_username;
    std::string temp_pass;
    char current_ch;
public:
    DataHandler();
    ~DataHandler();
    user_id& find_user(const std::string&); 
};