#pragma once
#include <ctime>

namespace LSMS
{
    namespace libtime
    {
        constexpr size_t MAXDATELENGTH = 80;

        char *getdate_mmddyyyy(char *);

    }

};