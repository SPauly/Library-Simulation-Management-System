#pragma once
#include <string>
#include <string_view>
#include <fstream>
#include <vector>
#include <iostream>

#define _DEBUG

namespace csv
{
    using _HEADER_TYPE = signed int;

    class Row 
    {
    public:
        Row()= delete;
        Row(std::string_view);
        ~Row();

        void add_value(std::string_view);
        std::string_view getvalue(_HEADER_TYPE&) const;
    protected:
        std::vector<std::string_view> m_data; 
    private:
        std::string::size_type m_StartIterator;
        std::string::size_type m_ItemIteratorPos;
    };

    class Header : public Row {
    public:
        Header() = delete;
        Header(std::string_view);

    public: 
        _HEADER_TYPE _header_size;
        std::string_view* _header_ptr; 

    };


    class CSVParser
    {
    public:
        CSVParser() = delete;
        CSVParser(const std::string*);
        ~CSVParser();

        //create debug print function here
    #ifdef _DEBUG
        void print_csv();
    #endif
    
    public:
        bool _csvgood;
        Header* _ptr_header;

    private:
        std::ifstream m_INPUT_FILE;
        std::vector<Row> m_content;
    };

} // namespace csv
