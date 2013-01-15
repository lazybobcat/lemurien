#include "sqlinsertfailedexception.h"

SqlInsertFailedException::SqlInsertFailedException(const std::string &what)
    : SqlException(what)
{
}
