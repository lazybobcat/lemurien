#include "sqldatabaseexception.h"

SqlDatabaseException::SqlDatabaseException(const std::string &what)
    : SqlException(what)
{
}
