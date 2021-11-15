#pragma once
#include "file_management.h"
#include <string>
#include <string_view>
#include <fstream>
#include <vector>
#include <stdexcept> 

#ifdef _DEBUG_CSV
#include <iostream>
#endif

namespace csv
{
    using _HEADER_TYPE = unsigned int;

    class Error : public std::runtime_error {
    public:
        Error(const std::string &msg) : 
            std::runtime_error(std::string("CSVParser: ").append(msg)){}     
    };

    class Row
    {
    public:
        Row();
        Row(std::string_view);
        Row(std::string_view, Row*);
        Row(std::vector<std::string>&);

        ~Row();

        unsigned int size();
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
    private:
        std::string m_header;
    public:
        Header() = delete;
        Header(std::string_view);
        Header(std::string_view, Row*) = delete;

        std::string_view string();
    public:
        _HEADER_TYPE _header_size;
        std::string *_header_ptr;
    };

    class CSVParser
    {
    public:
        CSVParser() = delete;
        CSVParser(const std::string *, Header&);
        ~CSVParser();

        const unsigned int size();
        Row& getRow(unsigned int&);
        bool addRow(Row&);
        bool find_first_of(std::string_view,std::string_view); //later return iterator
    #ifdef _DEBUG_CSV
        void print_csv();
    #endif

    public:
        Header *_ptr_header;
        bool _csvgood;
    private:
        //void m_check_consistency();
    private:
        std::string m_CURRENT_FILE;
        std::fstream m_DATABASE;
        std::vector<Row> m_content;
    };

} // namespace csv
