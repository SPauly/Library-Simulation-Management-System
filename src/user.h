#pragma once
#include <string>
#include <iostream>
#include <cstdlib> //exit(-1)
#include "DataHandler.h"

struct user_id{
    std::string username;
    std::string password;
};

class UserHandler{
    DataHandler datahandler;
public:
    void user_request(const std::string&);
};