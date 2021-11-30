#include "file_management.h"

std::string &fm::init_workingdir()
{

#ifdef _WIN32
    HMODULE module = GetModuleHandleW(NULL);
    WCHAR path[MAX_PATH];
    GetModuleFileNameW(module, path, MAX_PATH);

    std::wstring temp_ws = std::wstring(path);
    std::string str(temp_ws.size(), 0);
    std::transform(temp_ws.begin(), temp_ws.end(), str.begin(), [](wchar_t c)
                   { return (char)c; });
    
    _fm_path = str;
    _fm_path.remove_filename();
    work_dir = _fm_path.string();

    return work_dir;
#elif __linux__
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    work_dir = std::wstring(result, (count > 0) ? count : 0);
    _fm_path = work_dir;
    return work_dir;
#endif
}
 
std::fstream& fm::_getline(std::fstream &stream, std::string& line){
    if ( std::getline( stream, line) ) {
       if ( line.size() && line[line.size()-1] == '\r' ) {
           line.pop_back();
       }
    }
    return stream;
}

bool fm::fast_insert(std::fstream& _file, std::string_view _content, const size_t& _pos, user::_dimensions& _dimensions, const char& _token)
{
    try
    {
        //see if space is enough
        if (_dimensions.freespace < _content.size() + 2)
        {
            //call slow_insert
            return false;
        }

        //insert in file
        _file.seekp(_pos);
        _file >> _content;
        _file >> "\r\n";

        _file.flush();
        _file.clear();
    }
    catch (const std::out_of_range oor)
    {
    }
    catch(const std::ios::failure &e){

    }
}

bool fm::slow_insert(std::fstream& _file, std::string_view _content, const size_t& _pos, user::_dimensions& _dimensions, const char& _token)
{
    //Initialize buffer
            char *buf = new char[_dimensions.space];
            _file.seekg(_dimensions.beg);
            for (size_t i = _file.tellg(); i != _dimensions.end; i++)
            {
                _file.get(buf[i - _dimensions.beg]);
            }
                    delete[] buf;
}