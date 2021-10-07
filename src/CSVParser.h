#pragma once
#include <string>
#include <fstream>
#include <vector>

namespace csv
{
    typedef int _HEADER;

    class Row 
    {
    private:
        std::vector<std::string> m_values; 
    public:
        Row();
        ~Row();

        void add_value(const std::string&);
        template<typename T> const T* getvalue(_HEADER&) const;
    };

    class CSVParser
    {
    public:
        bool _csvgood = false;

    private:
        std::ifstream m_INPUT_FILE;

    public:
        CSVParser() = delete;
        CSVParser(const std::string *);
        ~CSVParser();
    };

} // namespace csv
