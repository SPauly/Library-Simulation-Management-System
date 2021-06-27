#include "DataHandler.h"

DataHandler::DataHandler(){}

DataHandler::~DataHandler(){}

user_id& DataHandler::find_user(const std::string& username_ref){
    read_data_file.open(data_directory);
    if(!read_data_file.good()){
        std::cerr<<"Error accessing database..."<<std::endl;
        exit(-1);
    } else {
        std::string temp_user;
        char current_ch;

        while(read_data_file.good()){
            temp_user.clear();
            do {
                read_data_file.get(current_ch);
                temp_user += current_ch;
            } while (current_ch != ',');
            
        }
    }
}