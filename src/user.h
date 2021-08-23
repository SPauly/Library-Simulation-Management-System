#pragma once
#include <string>
#include <iostream>
#include <cstdlib> //exit(-1)
#include "DataHandler.h"

class UserHandler{
    DataHandler datahandler;
    user_id user;
    int tries_left = 2;
public:
    bool user_request(const std::string&, const std::string&);
};