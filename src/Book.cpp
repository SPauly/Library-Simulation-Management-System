#include "Book.h"
namespace LSMS
{
    Book::Book(csv::Row *_ptr_row)
    {
        init(_ptr_row);
    }

    Book::Book(csv::Row *_ptr_row, csv::CSVParser *_ptr_parser) : Book(_ptr_row)
    {
        if (!_ptr_parser)
            throw csv::Error("Book: Parser points to nullptr");
        mptr_parser = _ptr_parser;
    }

    Book::Book(csv::CSVParser *_ptr_parser)
    {
        if (!_ptr_parser)
            throw csv::Error("Book: Parser points to nullptr");
        mptr_parser = _ptr_parser;
    }

    bool Book::init(csv::Row *_ptr_row)
    {
        csv::Row *temp_header = nullptr;
        if (!_ptr_row)
            throw csv::Error("Book: Row points to nullptr");
        try
        {
            temp_header = _ptr_row->get_headerptr(); //try if row is initialized with the right header
            temp_header->getvalue("NAME");
        }
        catch (const csv::Error &e)
        { //must be the BID,DATE,POS
            if (mptr_parser)
            {
                mptr_row = mptr_parser->find_first_of(_ptr_row->getvalue("BID"), "BID");
                mptr_userinfo_style = _ptr_row;
                return true;
            }
            else
            {
                mptr_row = nullptr;
                mptr_userinfo_style = _ptr_row;
                return false;
            }
        }

        mptr_row = _ptr_row;
        return true;
    }

    Book::~Book()
    {
        delete mptr_row;
        delete mptr_userinfo_style;
        m_file.close();
    }

    bool Book::mf_open()
    {
        try
        {
            std::string fullpath = fm::init_workingdir() + mptr_row->getvalue("LINK").data();
            m_file.open(fullpath, std::ios::in | std::ios::out);
            if (m_file.is_open())
                return m_is_open = true;
        }
        catch (const std::fstream::failure &e)
        {
        }
        catch (const csv::Error &e)
        {
        }

        return m_is_open = false;
    }

    size_t Book::increase_rented()
    {
        if (!mptr_parser)
        {
            throw csv::Error("Book: Parser not connected");
        }

        size_t currently_rented = 0;
        int format_length = 4;
        std::string currently_rented_s(format_length--, '0');

        try
        {
            currently_rented = std::stoi(mptr_row->getvalue("RENTED").data());
            ++currently_rented;
            //Format string to 0001
            for (int val = (currently_rented < 0) ? -currently_rented : currently_rented; format_length >= 0 && val != 0; --format_length, val /= 10)
                currently_rented_s[format_length] = '0' + val % 10;
            if (format_length >= 0 && currently_rented < 0)
                currently_rented_s[0] = '-';
        }
        catch (const std::invalid_argument &e)
        {
            return csv::npos;
        }

        if (mptr_row->change_value_in_to("RENTED", currently_rented_s))
            if (mptr_parser->updateRow(mptr_row))
                return currently_rented;
            else
                return csv::npos;
        else
            return csv::npos;
    }

    bool Book::is_available()
    {
        try
        {
            int copies = std::stoi(mptr_row->getvalue("COPIES").data());
            int rented = std::stoi(mptr_row->getvalue("RENTED").data());
            return copies - rented;
        }
        catch (const std::invalid_argument &)
        {
            return false;
        }
        return false;
    }

    std::string_view Book::get_BID()
    {
        try
        {
            return mptr_row->getvalue(0);
        }
        catch (csv::Error &e)
        {
            return "CSVError not found";
        }
    }

    std::string_view Book::get_NAME()
    {
        try
        {
            return mptr_row->getvalue("NAME");
        }
        catch (csv::Error &e)
        {
            return "CSVError Name not found";
        }
    }

    csv::Row &Book::get_Row()
    {
        return *mptr_row;
    }

    std::string Book::get_public_info()
    {
        std::string temp = "";
        csv::Row *row_ptr = mptr_parser->find_first_of(mptr_row->getvalue(0), "BID");
        if (row_ptr)
        {
            temp += row_ptr->getvalue("NAME");
            temp += ", ";
            temp += row_ptr->getvalue("AUTHOR");
        }
        return temp;
    }

    std::string Book::get_insight_info()
    {
        std::string temp = "";
        csv::Row *row_ptr = mptr_parser->find_first_of(mptr_row->getvalue(0), "BID");
        if (row_ptr)
        {
            temp += row_ptr->getvalue("NAME");
            temp += ", ";
            temp += row_ptr->getvalue("AUTHOR");
            temp += ", ";
            temp += row_ptr->getvalue("COPIES");
            temp += ", ";
            temp += row_ptr->getvalue("RENTED");
        }
        return temp;
    }

    std::string Book::get_admin_info()
    {
        std::string temp = "";
        csv::Row *row_ptr = mptr_parser->find_first_of(mptr_row->getvalue(0), "BID");
        if (row_ptr)
        {
            temp += row_ptr->string();
        }
        return temp;
    }

    size_t Book::get_pos()
    {
        if (mptr_userinfo_style)
        {
            try
            {
                size_t pos = std::stoi(mptr_userinfo_style->getvalue("POSITION").data());
                return pos;
            }
            catch (const std::invalid_argument &ia)
            {
                return 0;
            }
        }
        else
            return 0;
    }

    std::string_view Book::get_current_page()
    {
        if (!m_is_open)
        {
            if (!mf_open())
                return "Failed to open Book\n";
        }
        std::string line = "";
        page.clear();
        int line_count = 0;
        m_file.seekg(0, std::ios::beg);
        while(m_file && line_count < (get_pos()*amount_of_lines)){
            fm::_getline(m_file, line);
            ++line_count;
        }
        line_count = 0;
        while (m_file && line_count < amount_of_lines)
        {
            fm::_getline(m_file, line);
            page += line + "\n";
            ++line_count;
        }

        if(line_count < amount_of_lines) {
            page += "END of file\n";
        }

        return page;
    }
}