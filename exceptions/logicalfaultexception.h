#ifndef LOGICALFAULTEXCEPTION_H
#define LOGICALFAULTEXCEPTION_H

#include <stdexcept>

class LogicalFaultException : public std::logic_error
{
public:
    LogicalFaultException(const std::string& what);
    virtual ~LogicalFaultException() throw();
};

#endif // LOGICALFAULTEXCEPTION_H
