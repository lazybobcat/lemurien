#ifndef SQLMODEL_H
#define SQLMODEL_H

/**
 * @file
 * @brief This file contains the class to handle Database Models
 * @author BOUTTER Loïc
 * @version 2
 * @date 2012-11
 */

#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "exceptions.h"

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
    virtual void save() throw(SqlException) = 0;

    /**
     * @brief consruct should initialize children classes data from database with the primary key (that should to be set !!)
     */
    virtual void construct() throw(SqlException, LogicalFaultException) = 0;

    /**
     * @brief erase should remove the line corresponding to the object from database, the primary key should be set before !
     */
    virtual void erase() throw(SqlException, LogicalFaultException) = 0;

    /**
     * @brief consruct should initialize children classes data from database
     * @param fromid This is to retrieve the correct line in the table
     */
    void construct(unsigned int fromid) throw(SqlException, LogicalFaultException);


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
     * @param key Key to retrieve the correct line in the table
     */
    void        setPrimaryKey(unsigned int key);

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
    unsigned int        key() const;

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
    void            autoKey()  throw(SqlException);

private:
    QSqlDatabase*   mDatabase;          ///< Pointer on the unique QSqlDatabase, handled by the DatabaseManager
    QString         mTable;             ///< The (db)table name containing the needed data
    QString         mPrimaryKeyField;   ///< The name of the Primary Key Field (eg: "id" or "name")
    unsigned int    mPrimaryKey;        ///< The value of the Primary Key for this object to access our data in the (db)table
};

#endif // SQLMODEL_H
