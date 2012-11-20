#ifndef SQLMODEL_H
#define SQLMODEL_H

/**
 * @file
 * @brief This file contains the class to handle Database Models
 * @author BOUTTER Loïc
 * @version 1
 * @date 2012-11
 */

#include <QString>
#include <boost/any.hpp>
#include <QSqlDatabase>
#include <QSqlQuery>

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
    enum Error { LogicalError,  ///< Error that occurs if the program is wrong (a important initialisation has been skiped, ...)
                 SQLError,      ///< Error that occurs if the database isn't open
                 DataNotFound,  ///< Error that occurs if the data for a primary key hasn't been found. This can occur, make sure to catch this !
                 InsertFailed,  ///< Error that occurs if the data hasn't been inserted in the database. This can occur, make sure to catch this !
                 UnknownError   ///< Other errors
               };

    /**
     * @brief SqlModel constructor
     * @param tablename This is the table name, where to look for the Object data
     */
    explicit SqlModel(QSqlDatabase* db, const QString& tablename, const QString &primarykeyname = "id");

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
    void        setTable(const QString& newtable);

    /**
     * @brief Set the name of the primary key field in the table
     * @param keyname The name of the Primary Key Field (eg: "id" or "name")
     */
    void        setPrimaryKeyField(const QString &keyname);

    /**
     * @brief Set the value of the primary key
     * @param key Key to retrieve the correct line in the table, the type can be a string, int values, ...
     */
    void        setPrimaryKey(const boost::any& key);

    /**
     * @brief table accessor
     * @return The (db)table name
     */
    const QString&      table() const;

    /**
     * @brief keyname accessor
     * @return The name of the field containing the primary key
     */
    const QString&      keyfield() const;

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

protected:
    /**
     * @brief Automatically generates the next integer key for the table mTable, in cases where AUTO INCREMENT
     *          cannot be activated (because of SQLIte simplifications)
     * @details Takes the max mPrimarKeyField in the mTable and add 1. Sets the mPrimaryKey with that value.
     *          This method can return an SqlModel::SqlError if the connection to the database isn't established.
     *          This method is protected because you need to be very careful with it. You don't want to alter an already set
     *          primary key for nothing !
     */
    void            autoKey()  throw(Error);

private:
    QSqlDatabase*   mDatabase;          ///< Pointer on the unique QSqlDatabase, handled by the DatabaseManager
    QString         mTable;             ///< The (db)table name containing the needed data
    QString         mPrimaryKeyField;   ///< The name of the Primary Key Field (eg: "id" or "name")
    boost::any      mPrimaryKey;        ///< The value of the Primary Key for this object to access our data in the (db)table
};

#endif // SQLMODEL_H
