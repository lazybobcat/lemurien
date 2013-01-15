#ifndef SQLEXCEPTION_H
#define SQLEXCEPTION_H

#include <exception>
#include <string>

/**
* @brief Exceptions class, inherits from std::exceptions
*/
class SqlException : public std::exception
{
public:

    /*enum Error { LogicalError,      ///< Error that occurs if the program is wrong (a important initialisation has been skiped, ...)
                 DatabaseNotOpened, ///< Error that occurs if the database isn't open
                 DataNotFound,      ///< Error that occurs if the data for a primary key hasn't been found. This can occur, make sure to catch this !
                 InsertFailed,      ///< Error that occurs if the data hasn't been inserted in the database. This can occur, make sure to catch this !
                 UnknownError       ///< Other errors
               };*/

    /**
    * @brief Constructor
    * @param what A std::string containing the exception explanation
    */
    SqlException(const std::string& what) throw();

    /**
    * @brief Default destructor
    */
    virtual ~SqlException() throw();

    /**
    * Default copy constructor
    */
    SqlException(const SqlException& cpy);

    /**
    * @brief Returns the explanation in a const char*, to put it in a stream
    * @return The const char* string containing the explanation of the exception
    */
    virtual const char* what() const throw();




private:
    std::string     mWhat; ///< A std::string containing the exception explanation
};

#endif // SQLEXCEPTION_H
