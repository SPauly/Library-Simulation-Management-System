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
    const size_t npos = -1;

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
        Row(std::string_view, Row*, const size_t&);
        Row(std::vector<std::string>&);

        ~Row();

        size_t size();
        void add_value(std::string_view);
        Row& set_headerptr(Row*); 
        bool change_value_in_to(std::string_view, std::string_view); //new value, Header to change -> returns false if new value is bigger than old
        std::string_view string();

        std::string_view getvalue(unsigned int) const;
        std::string_view getvalue(std::string_view) const;
        unsigned int& get_item_position(std::string_view);
        const size_t& getindex();
        std::string_view operator[] (unsigned int &) const;
        std::string_view operator[] (std::string_view) const;

    protected:
        std::vector<std::string> m_data;
    private:
        Row* mptr_header = nullptr;
        unsigned int m_item_pos = 0;
        size_t m_index = npos;
        std::string m_rowstring = "";
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
        const size_t _index = 0; 
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
        bool updateRow(Row*);
        Row* find_first_of(std::string_view,std::string_view); //later return iterator
        bool is_good();
    #ifdef _DEBUG_CSV
        void print_csv();
    #endif
        Header *_ptr_header = nullptr;

    private:
        bool _csvgood = false;
        std::string m_CURRENT_FILE;
        std::fstream m_DATABASE;
        std::vector<Row> m_content;
        size_t m_index_pos = 0;

        std::fstream& mf_create_database();
    };

} // namespace csv
