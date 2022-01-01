#include "CSVParser.h"

namespace csv
{
    //class Row

    Row::Row() {}

    Row::Row(std::string_view _row)
    {
        std::string::size_type _StartIterator = 0;
        std::string::size_type _ItemIteratorPos = 0;

        do
        {
            _ItemIteratorPos = _row.find_first_of(",", _StartIterator);
            m_data.push_back(std::string(_row.substr(_StartIterator, _ItemIteratorPos - _StartIterator)));
            _StartIterator = _ItemIteratorPos + 1;
        } while (_ItemIteratorPos != std::string::npos);
    };

    Row::Row(std::string_view _row, Row *_header) : Row(_row)
    {
        mptr_header = _header;
    }

    Row::Row(std::string_view _row, Row *_header, const size_t &_index = npos) : Row(_row)
    {
        mptr_header = _header;
        m_index = _index;
    }

    Row::Row(std::vector<std::string> &_row)
    {
        m_data.swap(_row);
        _row.clear();
    }

    Row::~Row()
    {
        m_data.clear();
    };

    size_t Row::size()
    {
        return m_data.size();
    }

    void Row::add_value(std::string_view _value)
    {
        m_data.push_back(std::string(_value));
    };

    bool Row::change_value_in_to(std::string_view _header, std::string_view _newvalue)
    {
        if (mptr_header)
        {
            size_t pos = mptr_header->get_item_position(_header);
            if (_newvalue.size() == m_data.at(pos).size())
            {
                m_data.at(pos) = _newvalue;
            }
            else
            {
                return false;
            }
        }
        else
        {
            throw Error("Row: not linked to valid header");
        }
        return true;
    }

    std::string_view Row::string()
    {
        m_rowstring.clear();
        for (int i = 0; i < m_data.size() - 1; i++)
        {
            m_rowstring += m_data.at(i) + ",";
        }
        m_rowstring += m_data.at(m_data.size() - 1);
        m_rowstring += "\r\n";
        return m_rowstring;
    }

    std::string_view Row::getvalue(unsigned int _header) const
    {
        if (_header < m_data.size())
            return m_data.at(_header);
        else
        {
            throw Error("Row: out of range");
        }
    };

    std::string_view Row::getvalue(std::string_view _header) const
    {
        if (mptr_header)
        {
            size_t pos = mptr_header->get_item_position(_header);
            return m_data.at(pos);
        }
        else
        {
            throw Error("Row: not linked to valid header");
        }
    }

    unsigned int &Row::get_item_position(std::string_view _header)
    {
        std::vector<std::string>::const_iterator it;
        m_item_pos = 0;

        for (it = m_data.begin(); it != m_data.end(); it++)
        {
            if (_header == *it)
                return m_item_pos;
            m_item_pos++;
        }

        throw Error("Row: Item not found");
    }

    const size_t &Row::getindex()
    {
        return m_index;
    }

    Row &Row::set_headerptr(Row *_headerptr)
    {
        mptr_header = _headerptr;
        return *mptr_header;
    }

    std::string_view Row::operator[](unsigned int &_header) const
    {
        return this->getvalue(_header);
    };

    std::string_view Row::operator[](std::string_view _header) const
    {
        if (mptr_header)
        {
            size_t pos = mptr_header->get_item_position(_header);
            return m_data.at(pos);
        }
        else
        {
            throw Error("Row: not linked to valid header");
        }
    };

    Row *Row::get_headerptr() const {
        return mptr_header;
    }
    //end class Row

    //class Header : public Row

    Header::Header(std::string_view _row) : Row(_row)
    {
        _header_size = this->size();
        _header_ptr = m_data.data();
        m_header = _row;
    };

    std::string_view Header::string()
    {
        return m_header;
    }

    //end class Header

    //class CSVParser
    CSVParser::CSVParser(const std::string &PATH_ref, Header &_header_structure)
    {
        //initialize important references before exceptions
        m_CURRENT_FILE = PATH_ref;
        _ptr_header = &_header_structure;
        m_DATABASE.exceptions(std::fstream::failbit);

        try
        {

            //open csv file
            m_DATABASE.open(PATH_ref, std::ios::in | std::ios::out | std::ios::binary);

            //init header of file
            std::string *tmp_line = new std::string;
            tmp_line->clear();
            fm::_getline(m_DATABASE, *tmp_line);
            if (*tmp_line != _header_structure.string())
            {
                m_DATABASE.seekp(0, std::ios::beg);
                m_DATABASE << _header_structure.string() << "\r\n"; //temporary solution!!! Use insert in file function here instead
                m_DATABASE.flush();
            }
            tmp_line->clear();

            //check m_DATABASE for consistency

            //init m_content
            try
            {
                while (fm::_getline(m_DATABASE, *tmp_line))
                {
                    ++m_index_pos;
                    m_content.push_back(Row(*tmp_line, _ptr_header, m_index_pos));
                }
            }
            catch (const std::fstream::failure &e)
            {
            }
            //delete temporary values
            delete tmp_line;
            m_DATABASE.clear();
            m_DATABASE.flush();

            _csvgood = true;
        }
        catch (const std::fstream::failure &e)
        {
            try
            {
                mf_create_database();
                _csvgood = true;
            }
            catch (const std::fstream::failure &e)
            {
                _csvgood = false;
                throw Error(std::string("CTOR: Failed to create new Database: ").append(e.what()));
            }
            catch (const csv::Error &e)
            {
                _csvgood = false;
                throw Error(std::string("CTOR: Failed initialize new Database: ").append(e.what()));
            }
        }
    }

    CSVParser::~CSVParser()
    {
        _csvgood = false;
        m_DATABASE.close();
        m_content.clear();
    }

    const size_t CSVParser::size()
    {
        return m_content.size();
    }

    Row &CSVParser::getRow(unsigned int &_row)
    {
        if (_row < m_content.size())
            return m_content[_row];

        throw Error("CSV: out of range");
    }

    bool CSVParser::addRow(Row &_row)
    {
        if (_ptr_header)
            _row.set_headerptr(_ptr_header);
        else
            throw Error("CSV: Cannot add row without linking parser to valid header element first");
        try
        {
            if (m_DATABASE.is_open())
            {
                unsigned int i = 0;
                for (; i < _row.size() - 1; i++)
                {
                    m_DATABASE << _row[i].data();
                    m_DATABASE << ",";
                }
                m_DATABASE << _row[i].data();
                m_DATABASE << "\r\n";
                m_DATABASE.flush();
                m_content.push_back(_row);
                return true;
            }
            else
            {
                m_DATABASE.flush();
                m_DATABASE.close();
                try
                {
                    m_DATABASE.open(m_CURRENT_FILE, std::ios::in | std::ios::out);
                    throw Error("CSV: Error while writing database");
                }
                catch (std::ios::failure &e)
                {
                    throw Error("CSV: failed to open database");
                }
            }
        }
        catch (std::out_of_range &oor)
        {
            throw Error("CSV: out of range");
        }
        catch (csv::Error &e)
        {
            throw Error("CSV: out of range");
        }
    }

    bool CSVParser::updateRow(Row *_row)
    {
        if (!m_DATABASE.is_open())
        {
            return false;
        }
        try
        {
            std::string tmp_line = "";
            m_DATABASE.seekp(0, std::ios::beg);
            for (int i = 0; i < _row->getindex(); i++)
            {
                fm::_getline(m_DATABASE, tmp_line);
            }
            m_DATABASE.seekp(m_DATABASE.tellg());
            m_DATABASE << _row->string();
            m_DATABASE.flush();
            m_DATABASE.clear();
        }
        catch (std::ios::failure &e)
        {
            return false;
        }
        return true;
    }

    Row *CSVParser::find_first_of(std::string_view _str, std::string_view _header)
    {
        unsigned int _tmp_pos = _ptr_header->get_item_position(_header);

        for (int i = 0; i < m_content.size(); i++)
        {
            if (m_content.at(i).getvalue(_tmp_pos) == _str)
            {
                return &m_content.at(i);
            }
        }

        return nullptr;
    }

    std::fstream &CSVParser::mf_create_database()
    {
        m_DATABASE.open(m_CURRENT_FILE, std::ios::in | std::ios::out | std::ios::binary | std::ios::app);
        addRow(*_ptr_header);

        m_DATABASE.clear();
        m_DATABASE.flush();
        return m_DATABASE;
    }

    bool CSVParser::is_good()
    {
        return _csvgood;
    }

#ifdef _DEBUG_CSV
    void CSVParser::print_csv()
    {
        for (size_t i = 0; i < _ptr_header->_header_size; i++)
        {
            std::cout << _ptr_header->getvalue(i);
            std::cout << ",";
        }
        std::cout << std::endl;
        for (size_t i = 0; i < m_content.size(); i++)
        {
            for (size_t i2 = 0; i2 < _ptr_header->_header_size; i2++)
            {
                std::cout << m_content.at(i).getvalue(i2);
                std::cout << ",";
            }
            std::cout << std::endl;
        }
    }
#endif
    //end CSVParser

} // namespace csv
