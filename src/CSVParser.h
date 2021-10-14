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
        Row(std::string_view, Row*);

        ~Row();

        void add_value(std::string_view);
        std::string_view getvalue(_HEADER_TYPE &) const;
        _HEADER_TYPE& get_item_position(std::string_view);


        std::string_view operator[] (_HEADER_TYPE &) const;
        std::string_view operator[] (std::string_view) const;

    protected:
        std::vector<std::string> m_data;
    private:
        Row* mptr_header;
        _HEADER_TYPE m_item_pos = 0;
    };

    class Header : public Row
    {
    public:
        Header() = delete;
        Header(std::string_view);
        Header(std::string_view, Row*) = delete;

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
