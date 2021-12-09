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

template <typename T>
T &fm::buf_insert(T& _buf, std::string_view _content, const size_t &_pos, const char &_token) [[deprecated("This function currently overrides all characters and has no oor checks")]]
{
    //check if buf is big enough

    //check if space is actualy free
    for (int i = 0; i < _content.size(); i++)
    {
        if (_buf[i + _pos] != _token)
        {
            //do something here to modify buf and write the characters at the back
            //if there is too many characters to fit set last pos to npos
        }
    }

    //insert _content into buffer
    for (int i = 0; i < _content.size(); i++){
        _buf[i + _pos] = _content.at(i);
    }

    return _buf;
}

size_t& fm::fast_insert(std::fstream &_file, std::string_view _content, const size_t &_pos, const size_t& _beg, const size_t& _end, const char &_token, size_t& _freespace)
{
    std::string _temp_content = _content.data();
    _temp_content += "\r\n";
    size_t space = _end - _beg;
    size_t contentsize = _temp_content.size();

    try
    {
        //Initialize buffer
        char *buf = new char[space + 1];
        _file.seekg(_beg);
        for (size_t i = 0; i < space; i++)
        {
            _file.get(buf[i]);
        }
        buf[space] = '\0';
    
        //see if space is enough
        if (_freespace < contentsize || space < contentsize)
        {
            return slow_insert(_file, _content, _pos, _beg, _end, _token, _freespace);
        }

        buf_insert(buf, _temp_content, (_pos - _beg), _token);

        //insert modified buffer in file
        _file.seekp(_beg);
        _file << buf;

        //update _freespace 
        _freespace -= contentsize;

        _file.flush();
        _file.clear();
        delete[] buf;
    }
    catch (const std::out_of_range oor)
    {
         _freespace = npos;
         return _freespace;
    }
    catch(const std::ios::failure &e){
         _freespace = npos;
        return _freespace;
    }
    return _freespace;
}

size_t& fm::slow_insert(std::fstream &_file, std::string_view _content, const size_t &_pos, const size_t& _beg, const size_t& _end, const char &_token, size_t& _freespace)
{

}