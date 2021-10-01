#pragma once
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>

class DataHandler {
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
    user_id find_user(const std::string&); 
};

class CSVfile {
    std::string m_line;
    std::vector<int> m_data;

public:
    std::string_view operator[](std::size_t index) const {
        return std::string_view(&m_line[m_line[index]+1], m_data[index+1] - (m_data[index] + 1));
    }
    std::size_t size() const{
        return m_data.size() -1;
    }
    void readNextRow(std::istream&);

};

struct user_id{
    std::string username;
    std::string password;
};