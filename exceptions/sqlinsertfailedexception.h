#ifndef SQLINSERTFAILEDEXCEPTION_H
#define SQLINSERTFAILEDEXCEPTION_H

#include "sqlexception.h"

class SqlInsertFailedException : public SqlException
{
public:
    SqlInsertFailedException(const std::string& what);
};

#endif // SQLINSERTFAILEDEXCEPTION_H
