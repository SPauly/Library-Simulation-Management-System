#include "file_management.h"

std::string& fm::working_dir()
{
    std::filesystem::path _temp_path = std::filesystem::current_path();
    if (_temp_path.filename().string() != "GetIntoCPPagain")
    { //Later might want to do that a little bit less specific
       _temp_path.remove_filename();
       _work_dir = _temp_path.string();
       if((_work_dir.find_last_of("\\") + 1) > _work_dir.size()){
           return _work_dir;
       }
       else{
           _work_dir.pop_back();
           return _work_dir;
       }
    }
    _work_dir = _temp_path.string();
    return _work_dir;
}