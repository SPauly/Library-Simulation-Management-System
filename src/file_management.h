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
    //Workspace info
    static std::filesystem::path _fm_path;
    static std::string work_dir;
    std::string& init_workingdir();

    //CLRF safe getline
    std::fstream &_getline(std::fstream&, std::string&);

    //insert in file functions
    static constexpr size_t npos = -1;
    size_t& fast_insert(std::fstream&, std::string_view, const size_t&, const size_t&, const size_t&, const char&, size_t&); //_file, _content, _pos, _beg, _end, _token, _freespace
    bool slow_insert(std::fstream&, std::string_view, const size_t&, const size_t&, const size_t&, const char&, size_t&);

};