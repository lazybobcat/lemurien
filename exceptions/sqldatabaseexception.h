#ifndef SQLDATABASEEXCEPTION_H
#define SQLDATABASEEXCEPTION_H

#include "sqlexception.h"

class SqlDatabaseException : public SqlException
{
public:
    SqlDatabaseException(const std::string& what);
};

#endif // SQLDATABASEEXCEPTION_H
