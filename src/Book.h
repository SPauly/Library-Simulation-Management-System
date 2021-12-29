#pragma once
#include "CSVParser.h"
#include <string>

namespace LSMS
{
    class Book
    {
    private:
        csv::Row *mptr_info = nullptr;
        csv::CSVParser *mptr_parser = nullptr;

    public:
        Book() = default;
        Book(csv::Row *);
        Book(csv::Row *, csv::CSVParser *);
        Book(csv::CSVParser *);
        ~Book();

        void init(csv::Row *);

        size_t increase_rented();

        bool is_available();
        std::string_view get_BID();
        csv::Row &get_Row();
        std::string_view get_public_info();
        std::string_view get_insight_info();
        std::string_view get_admin_info();
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