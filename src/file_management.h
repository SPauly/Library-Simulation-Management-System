#pragma once
#include <filesystem>

#ifdef _WIN32
    #include <windows.h>
#elif __linux__
    #include <limits.h>
    #include <unistd.h>
#endif

namespace fm
{
    static std::filesystem::path _fm_path;
    static std::string work_dir;
    std::string& init_workingdir();
};