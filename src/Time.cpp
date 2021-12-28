#include "Time.h"

char *libtime::getdate_mmddyyyy(char *_str)
{
    std::time_t t = std::time(0);
    strftime(_str, MAXDATELENGTH, "%m%d%Y", localtime(&t));
    return _str;
}
