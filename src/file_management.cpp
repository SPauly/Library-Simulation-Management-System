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

std::fstream &fm::_getline(std::fstream &stream, std::string &line)
{
    if (std::getline(stream, line))
    {
        if (line.size() && line[line.size() - 1] == '\r')
        {
            line.pop_back();
        }
    }
    return stream;
}

template <typename T>
T &fm::buf_insert(T &_buf, std::string_view _content, const size_t &_pos, const char &_token) [[deprecated("This function currently has no oor checks")]]
{
    //check if buf is big enough

    //check if space is actualy free
    for (int i = 0; i < _content.size(); i++)
    {
        if (_buf[i + _pos] != _token && _buf[i + _pos] != '\r' && _buf[i + _pos] != '\n')
        {
            //do something here to modify buf and write the characters at the back
            file_pos = npos;
            return _buf;
        }
    }

    //insert _content into buffer
    for (int i = 0; i < _content.size(); i++)
    {
        _buf[i + _pos] = _content.at(i);
    }

    return _buf;
}

size_t &fm::fast_insert(std::fstream &_file, std::string_view _content, const size_t &_pos, const size_t &_beg, const size_t &_end, const char &_token)
{
    std::string _temp_content = _content.data();
    _temp_content += "\r\n";
    size_t space = _end - _beg;
    size_t contentsize = _temp_content.size();
    size_t pos_minus_one = _pos - 1;

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
        if (space < contentsize)
        {
            return slow_insert(_file, _content, pos_minus_one, _beg, _end, _token);
        }

        buf_insert(buf, _temp_content, (pos_minus_one - _beg), _token);
        //check if space was to short -> file_pos = npos
        if (file_pos == npos)
            return file_pos;

        //insert modified buffer in file
        _file.seekp(_beg);
        _file << buf;
        file_pos = _file.tellp();

        _file.flush();
        _file.clear();
        delete[] buf;
    }
    catch (const std::out_of_range oor)
    {
        return file_pos = npos;
    }
    catch (const std::ios::failure &e)
    {
        return file_pos = npos;
    }
    return file_pos;
}

size_t &fm::slow_insert(std::fstream &_file, std::string_view _content, const size_t &_pos, const size_t &_beg, const size_t &_end, const char &_token)
{
}
