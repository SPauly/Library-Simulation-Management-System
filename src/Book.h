#pragma once
#include "CSVParser.h"
#include <string>

namespace LSMS
{
    class Book
    {
    private:
        csv::Row *mptr_row = nullptr;
        csv::CSVParser *mptr_parser = nullptr;
        csv::Row *mptr_userinfo_style = nullptr;

    public:
        Book() = default;
        Book(csv::Row *);
        Book(csv::Row *, csv::CSVParser *);
        Book(csv::CSVParser *);
        ~Book();

        bool init(csv::Row *);

        size_t increase_rented();
        size_t increase_position();

        bool is_available();
        std::string_view get_BID();
        std::string_view get_NAME();
        csv::Row &get_Row();
        std::string_view get_public_info();
        std::string_view get_insight_info();
        std::string_view get_admin_info();
        size_t get_pos();
        std::string_view get_current_page();
    };

    inline Book *g_BookInstance = nullptr;

    inline Book *InstanciateBookInstance(csv::CSVParser *_ptr_parser)
    {
        return g_BookInstance = new Book(_ptr_parser);
    }

    inline Book *GetBookInstance()
    {
        return g_BookInstance;
    }

    inline void DestroyBookInstance()
    {
        delete g_BookInstance;
    }
}