#ifndef SQLMODEL_H
#define SQLMODEL_H

/**
 * @file
 * @brief This file contains the class to handle Database Models
 * @author BOUTTER Loïc
 * @version 1
 * @date 2012-11
 */

#include <string>
#include <boost/any.hpp>
#include <QSqlDatabase>

/**
 * @class SqlModel
 * @brief An abstract class to handle object save and construction from database.
 * @details Here, Model means a data container an handler in a MVC point of view.
 *          If you ineherits from this class, you wil be need and be able to save and load your object data from the database
 * @see SqlModelFactory, DatabaseManager
 */
class SqlModel
{
public:
    /**
     * @brief The Error enumeration
     */
    enum Error { LogicalError, SQLError, DataNotFound, CastError, UnknownError };

    /**
     * @brief SqlModel constructor
     * @param tablename This is the table name, where to look for the Object data
     */
    explicit SqlModel(QSqlDatabase* db, const std::string& tablename, const std::string& primarykeyname = "id");

    /**
     * @brief Default destructor
     */
    virtual ~SqlModel();

    /**
     * @brief save should save the children classes data in database
     */
    virtual void save() throw(Error) = 0;

    /**
     * @brief consruct should initialize children classes data from database with the primary key (that should to be set !!)
     */
    virtual void construct() throw(Error) = 0;

    /**
     * @brief consruct should initialize children classes data from database
     * @param fromid This is to retrieve the correct line in the table, the type can be a string, int values, ...
     */
    void construct(const boost::any& fromid) throw(Error);


    /**
     * @brief Set the table name containing the needed data
     * @param newtable The (db)table name
     */
    void        setTable(const std::string& newtable);

    /**
     * @brief Set the name of the primary key field in the table
     * @param keyname The name of the Primary Key Field (eg: "id" or "name")
     */
    void        setPrimaryKeyField(const std::string& keyname);

    /**
     * @brief Set the value of the primary key
     * @param key Key to retrieve the correct line in the table, the type can be a string, int values, ...
     */
    void        setPrimaryKey(const boost::any& key);

    /**
     * @brief table accessor
     * @return The (db)table name
     */
    const std::string&  table() const;

    /**
     * @brief keyname accessor
     * @return The name of the field containing the primary key
     */
    const std::string&  keyfield() const;

    /**
     * @brief key accessor
     * @return The value of the primary key for the object in the (db)table
     */
    const boost::any&   key() const;

    /**
     * @brief Get a pointer on the QSqlDatabase
     * @return a pointer on the unique QSqlDatabase
     */
    QSqlDatabase*   db();

private:
    QSqlDatabase*   mDatabase;          ///< Pointer on the unique QSqlDatabase, handled by the DatabaseManager
    std::string     mTable;             ///< The (db)table name containing the needed data
    std::string     mPrimaryKeyField;   ///< The name of the Primary Key Field (eg: "id" or "name")
    boost::any      mPrimaryKey;        ///< The value of the Primary Key for this object to access our data in the (db)table
};

#endif // SQLMODEL_H
