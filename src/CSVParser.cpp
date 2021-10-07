#include "CSVParser.h"

namespace csv
{
    Row::Row(){};
    Row::~Row(){};

    Row::add_value(const std::string&);

    csv::CSVParser::CSVParser(const std::string *PATH_ptr)
    {
        try
        {
            m_INPUT_FILE.open(*PATH_ptr);
            m_INPUT_FILE.exceptions(std::ifstream::failbit | std::ifstream::badbit | std::ifstream::eofbit);


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

} // namespace csv
