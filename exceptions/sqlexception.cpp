#include "sqlexception.h"

SqlException::SqlException(const std::string& what) throw()
    : mWhat(what)
{

}

SqlException::~SqlException() throw()
{

}


SqlException::SqlException(const SqlException &cpy)
{
    mWhat = cpy.mWhat;
}


const char* SqlException::what() const throw()
{
    return mWhat.c_str();
}
