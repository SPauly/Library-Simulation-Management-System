#pragma once
#include <string>
#include <string_view>
#include <fstream>
#include <vector>
 

#ifdef _DEBUG_CSV
#include <iostream>
#endif

namespace csv
{
    using _HEADER_TYPE = unsigned int;

    class Row
    {
    public:
        Row() = delete;
        Row(std::string_view);
        ~Row();

        void add_value(std::string_view);
        std::string_view getvalue(_HEADER_TYPE &) const;

        std::string_view operator[] (_HEADER_TYPE &) const;
        
    protected:
        std::vector<std::string> m_data;

    private:
        std::string::size_type m_StartIterator;
        std::string::size_type m_ItemIteratorPos;
    };

    class Header : public Row
    {
    public:
        Header() = delete;
        Header(std::string_view);

    public:
        _HEADER_TYPE _header_size;
        std::string *_header_ptr;
    };

    class CSVParser
    {
    public:
        CSVParser() = delete;
        CSVParser(const std::string *);
        ~CSVParser();

        const unsigned int size();
        Row& getRow(unsigned int&);

        Row &operator[] (unsigned int&);  //unfunctional

    #ifdef _DEBUG_CSV
        void print_csv();
    #endif

    public:
        bool _csvgood;
        Header *_ptr_header;

    private:
        std::ifstream m_INPUT_FILE;
        std::vector<Row> m_content;
    };

} // namespace csv
