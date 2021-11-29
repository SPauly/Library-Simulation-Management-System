#pragma once
#include <filesystem>
#include <fstream>

#ifdef _WIN32
    #include <windows.h>
#elif __linux__
    #include <limits.h>
    #include <unistd.h>
#endif

namespace fm
{
    using pos = unsigned int;

    //Workspace info
    static std::filesystem::path _fm_path;
    static std::string work_dir;
    std::string& init_workingdir();

    //CLRF safe getline
    std::fstream &_getline(std::fstream&, std::string&);
    //insert in file functions
    bool fast_insert(std::fstream&, std::string_view, const size_t&, const size_t&, const size_t&, const char);
        //opened file, insert, pos, beg, end, spaceholder
};