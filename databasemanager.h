#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H


/**
 * @file
 * @brief Class to manage SQLite database and provide a pointer on QSqlDatabase object
 * @author BOUTTER Loïc
 * @version 1
 * @date 2012-11
 */


#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QFile>

/**
 * @class DatabaseManager
 * @brief This class handle the connection with the SQLite database
 */
class DatabaseManager
{
public:
    /**
     * @brief The Error enumeration.
     */
    enum Error { OpenDbError, AccessError, UnknownError };

    /**
     * @brief DatabaseManager constructor is private, for Singleton
     */
    explicit DatabaseManager();


    /**
     * @brief Destructor. Closes the database.
     */
    virtual ~DatabaseManager();

    /**
     * @brief Getter on the QSqlDatabase object. Use it to exec your SQL queries.
     * @return Pointer on a unique QSqlDatabase object.
     */
    QSqlDatabase*   db();

    /**
     * @brief Open the database
     */
    void        open()      throw(Error);

    /**
     * @brief Close the database.
     */
    void        close();

    /**
     * @brief If an error occured, use this to have more informations
     * @return Return a QSqlError containing informations about the error
     */
    QSqlError   lastError();


private:
    /**
     * @brief createTables
     * @throw In case of error while creating databases, throw a DatabaseManager::AccessError.
     */
    virtual void    createTables()      throw(Error);

    QSqlDatabase     mDatabase; ///< Contains database informations and useful methods (open, close, sql commands)
    
};

#endif // DATABASEMANAGER_H
