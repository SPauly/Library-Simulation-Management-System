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

        size_t size();
        void add_value(std::string_view);
        Row& set_headerptr(Row*); 
        bool change_value_in(std::string_view, std::string_view); //new value, Header to change -> returns false if new value is bigger than old

        std::string_view getvalue(unsigned int) const;
        std::string_view getvalue(std::string_view) const;
        unsigned int& get_item_position(std::string_view);
        std::string_view operator[] (unsigned int &) const;
        std::string_view operator[] (std::string_view) const;

    protected:
        std::vector<std::string> m_data;
    private:
        Row* mptr_header = nullptr;
        unsigned int m_item_pos = 0;
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
        size_t _header_size;
        std::string *_header_ptr;
    };

    class CSVParser
    {
    public:
        CSVParser() = delete;
        CSVParser(const std::string &, Header&);
        ~CSVParser();

        const size_t size();
        Row& getRow(unsigned int&);
        bool addRow(Row&);
        bool updateRow(Row&);
        Row* find_first_of(std::string_view,std::string_view); //later return iterator
    #ifdef _DEBUG_CSV
        void print_csv();
    #endif

    public:
        Header *_ptr_header = nullptr;
        bool _csvgood = false;

    private:
        std::string m_CURRENT_FILE;
        std::fstream m_DATABASE;
        std::vector<Row> m_content;
    private:
        std::fstream& m_create_database();
    };

} // namespace csv
