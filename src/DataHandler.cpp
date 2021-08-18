#include "DataHandler.h"

DataHandler::DataHandler(){}

DataHandler::~DataHandler(){}


user_id& DataHandler::find_user(const std::string& username_ref){
    
    read_data_file.open(data_directory);
    
    if(!read_data_file.good()){
        std::cerr<<"Error accessing database..."<<std::endl;
        exit(-1);
    } else {
       
        while(read_data_file.good()){

            temp_username.clear();
            
            do {
                read_data_file.get(current_ch);
                temp_username += current_ch;
            } while (current_ch != ',');
            
            if(temp_username == username_ref){
                
                do {
                    read_data_file.get(current_ch);
                    temp_pass += current_ch;
                } while (current_ch != '\n');

                user.username = temp_username;
                user.password = temp_pass;
                return user; 
            } else {
                std::getline(read_data_file, temp_username);
            }
            
        }
    }
}