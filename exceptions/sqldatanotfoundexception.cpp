#include "sqldatanotfoundexception.h"

SqlDataNotFoundException::SqlDataNotFoundException(const std::string &what)
    : SqlException(what)
{
}
