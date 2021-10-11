#include "CSVParser.h"

namespace csv
{
    //class Row

    Row::Row(std::string_view _row)
    {
        m_StartIterator = 0;
        m_ItemIteratorPos = 0;

        do
        {
            m_ItemIteratorPos = _row.find_first_of(",", m_StartIterator);
            m_data.push_back(std::string(_row.substr(m_StartIterator, m_ItemIteratorPos - m_StartIterator)));
            m_StartIterator = m_ItemIteratorPos + 1;
        } while (m_ItemIteratorPos != std::string::npos);
    };

    Row::~Row(){};

    void Row::add_value(std::string_view _value){
        m_data.push_back(std::string(_value));
    };

    std::string_view Row::getvalue(_HEADER_TYPE &_header) const
    {
        return m_data.at(_header);
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
                m_content.push_back(Row(*tmp_line));
            }

            //delete temporary values
            delete tmp_line;
        }
        catch (const std::ifstream::failure &e)
        {
            _csvgood = false;
        }
    }

    CSVParser::~CSVParser()
    {
        _csvgood = false;
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
