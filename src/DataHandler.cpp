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
            
            while (current_ch != ','){
                read_data_file.get(current_ch);
                if(current_ch != ','){
                    temp_username += current_ch;
                }
            }
            
            if(temp_username == username_ref){
                
                while (current_ch != '\n') {
                    read_data_file.get(current_ch);
                    if(current_ch != '\n'){
                        temp_pass += current_ch;
                    }
                } 

                user.username = temp_username;
                user.password = temp_pass;
                return user; 
            } else {
                std::getline(read_data_file, temp_username);
            }
            
        }

    }
}