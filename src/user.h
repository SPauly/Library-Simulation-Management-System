#pragma once
#include <string>


class user{
    std::string username;
    std::string password;

public:
    void user_request(const std::string&);
};