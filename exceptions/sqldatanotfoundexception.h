#ifndef SQLDATANOTFOUNDEXCEPTION_H
#define SQLDATANOTFOUNDEXCEPTION_H

#include "sqlexception.h"

class SqlDataNotFoundException : public SqlException
{
public:
    SqlDataNotFoundException(const std::string& what);
};

#endif // SQLDATANOTFOUNDEXCEPTION_H
