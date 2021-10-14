#include "CSVParser.h"

namespace csv
{
    //class Row

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

    Row::Row(std::string_view _row, Row* _header): Row(_row) {
        mptr_header = _header;
    }

    Row::~Row(){
        m_data.clear();
    };

    void Row::add_value(std::string_view _value){
        m_data.push_back(std::string(_value));
    };

    std::string_view Row::getvalue(_HEADER_TYPE &_header) const
    {   
        if(_header < m_data.size())
            return m_data.at(_header);
        //throw exception
    };

    _HEADER_TYPE &Row::get_item_position(std::string_view _header) 
    {
        std::vector<std::string>::const_iterator it;

        for (it = m_data.begin(); it != m_data.end(); it++)
        {
            if (_header == *it)
                return m_item_pos;
            m_item_pos++;
        }

        //throw exception
    }

    std::string_view Row::operator[] (_HEADER_TYPE &_header) const {
        return this->getvalue(_header);
    };

    std::string_view Row::operator[] (std::string_view _header) const {
        _HEADER_TYPE pos = mptr_header->get_item_position(_header);
        if(pos < m_data.size())
            return m_data.at(pos);

        return ""; //throw exception 
    };
    //end class Row

    //class Header : public Row

    Header::Header(std::string_view _row) : Row(_row)
    {
        _header_size = m_data.size();
        _header_ptr = m_data.data();
    };

    //end class Header

    //class CSVParser
    csv::CSVParser::CSVParser(const std::string *PATH_ptr)
    {
        try
        {
            //open csv file
            m_INPUT_FILE.open(*PATH_ptr, std::ios::out);
            m_INPUT_FILE.exceptions(std::ifstream::badbit);

            //init header of file
            std::string *tmp_line = new std::string;
            tmp_line->clear();
            std::getline(m_INPUT_FILE, *tmp_line);
            _ptr_header = new Header(*tmp_line);
            tmp_line->clear();

            //init m_content
            while (std::getline(m_INPUT_FILE, *tmp_line))
            {
                m_content.push_back(Row(*tmp_line, _ptr_header));
            }

            //delete temporary values
            delete tmp_line;

            _csvgood = true;
        }
        catch (const std::ifstream::failure &e)
        {
            _csvgood = false;
        }
    }

    CSVParser::~CSVParser()
    {
        _csvgood = false;
        m_INPUT_FILE.close();
        m_content.clear();
        delete _ptr_header;
    }

    const unsigned int CSVParser::size() {
        return m_content.size();
    }

    Row& CSVParser::getRow(unsigned int& _row){
        if(_row < m_content.size())
            return m_content[_row];
        //throw some exception
    }

#ifdef _DEBUG_CSV
    void CSVParser::print_csv()
    {
        for (_HEADER_TYPE i = 0; i < _ptr_header->_header_size; i++)
        {
            std::cout << _ptr_header->getvalue(i);
            std::cout << ",";
        }
        std::cout << std::endl;
        for (_HEADER_TYPE i = 0; i < m_content.size(); i++)
        {
            for (_HEADER_TYPE i2 = 0; i2 < _ptr_header->_header_size; i2++)
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
