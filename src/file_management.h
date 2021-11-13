#pragma once
#include <filesystem>

namespace fm
{
    static std::string _work_dir;
    std::string& working_dir();
};