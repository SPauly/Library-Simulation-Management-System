#include "CSVParser.h"

namespace csv
{
    //class Row

    Row::Row(){}

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

    Row::Row(std::vector<std::string> &_row){
        m_data.swap(_row);
        _row.clear();
    }

    Row::~Row(){
        m_data.clear();
    };

    unsigned int Row::size(){
        return m_data.size();
    }

    void Row::add_value(std::string_view _value){
        m_data.push_back(std::string(_value));
    };

    std::string_view Row::getvalue(_HEADER_TYPE &_header) const
    {   
        if(_header < m_data.size())
            return m_data.at(_header);
        else{
            throw Error("Row: out of range");
        }
    };

    _HEADER_TYPE &Row::get_item_position(std::string_view _header) 
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

    std::string_view Row::operator[] (_HEADER_TYPE &_header) const {
        return this->getvalue(_header);
    };

    std::string_view Row::operator[] (std::string_view _header) const {
        _HEADER_TYPE pos = mptr_header->get_item_position(_header);
        return m_data.at(pos);
    };
    //end class Row

    //class Header : public Row

    Header::Header(std::string_view _row) : Row(_row)
    {
        _header_size = this->size();
        _header_ptr = m_data.data();
    };

    //end class Header

    //class CSVParser
    CSVParser::CSVParser(const std::string *PATH_ptr)
    {
        try
        {
            m_CURRENT_FILE = *PATH_ptr;
            //open csv file
            m_DATABASE.open(*PATH_ptr, std::ios::in | std::ios::out | std::ios::binary);
            m_DATABASE.exceptions(std::fstream::badbit);

            //init header of file
            std::string *tmp_line = new std::string;
            tmp_line->clear();
            std::getline(m_DATABASE, *tmp_line);
            _ptr_header = new Header(*tmp_line);
            tmp_line->clear();
            //check m_DATABASE for consistency
            
            //init m_content
            while (std::getline(m_DATABASE, *tmp_line))
            {
                m_content.push_back(Row(*tmp_line, _ptr_header));
            }

            //delete temporary values
            delete tmp_line;
            m_DATABASE.clear();
            m_DATABASE.flush();
            _csvgood = true;
        }
        catch (const std::fstream::failure &e)
        {
            _csvgood = false;
            throw Error(std::string("CTOR: Error accessing Database: ").append(e.what()));
        }
    }

    CSVParser::~CSVParser()
    {
        _csvgood = false;
        m_DATABASE.close();
        m_content.clear();
        delete _ptr_header;
    }

    const unsigned int CSVParser::size() {
        return m_content.size();
    }

    Row& CSVParser::getRow(unsigned int& _row){
        if(_row < m_content.size())
            return m_content[_row];
        
        throw Error("CSV: out of range");
    }

    bool CSVParser::addRow(Row& _row) {
        try
        {
            if (m_DATABASE.is_open())
            {
                unsigned int i = 0;
                m_DATABASE << '\n';
                for (; i < _row.size() - 1; i++)
                {
                    m_DATABASE << _row[i].data();
                    m_DATABASE << ",";
                }
                m_DATABASE << _row[i].data();
                m_DATABASE.flush();
                m_content.push_back(_row);
                return true;
            }
            else{
                m_DATABASE.flush();
                m_DATABASE.close();
                try {
                    m_DATABASE.open(m_CURRENT_FILE, std::ios::in | std::ios::out);
                    throw Error("CSV: Error while writing database");
                }
                catch (std::ios::failure &e){
                    throw Error("CSV: failed to open database");
                }
            }
        }
        catch(std::out_of_range &oor){
            throw Error("CSV: out of range");
        }
        catch(csv::Error &e){
            throw Error("CSV: out of range");
        }
    }

    bool CSVParser::find_first_of(std::string_view _str, std::string_view _pos){
        _HEADER_TYPE _tmp_pos = _ptr_header->get_item_position(_pos);

        for(int i = 0; i < m_content.size(); i++){
            if(m_content.at(i).getvalue(_tmp_pos) == _str){
                return true;
            }
        }

        return false;
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
