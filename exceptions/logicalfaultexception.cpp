#include "logicalfaultexception.h"

LogicalFaultException::LogicalFaultException(const std::string& what)
    : std::logic_error(what)
{
}


LogicalFaultException::~LogicalFaultException() throw()
{

}
