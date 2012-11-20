#include "sqlmodel.h"
#include <QVariant>

SqlModel::SqlModel(QSqlDatabase* db, const QString &tablename, const QString& primarykeyname)
    : mDatabase(db),
      mTable(tablename),
      mPrimaryKeyField(primarykeyname),
      mPrimaryKey()
{

}

SqlModel::~SqlModel()
{

}


void SqlModel::construct(const boost::any &fromid)  throw(Error)
{
    // If the param is empty, big mistake ! We throw a logical error
    if(fromid.empty())
        throw SqlModel::LogicalError;

    setPrimaryKey(fromid);
    construct();
}

void SqlModel::autoKey() throw(SqlModel::Error)
{
    // Nothing to do if database is close
    if(db()->isOpen())
    {
        // Set the query : we want the bigger id+1 in the table
        QSqlQuery query(QString("SELECT MAX(%1)+1 FROM %2").arg(mPrimaryKeyField).arg(mTable));

        // If there's a result
        if(query.next())
            mPrimaryKey = query.value(0).toUInt(); // we extract the id and set Primarykey with its value
        else
            mPrimaryKey = 1; // If there the table is empty, we will be number one
    }
    else
        throw SqlModel::SQLError;
}


void SqlModel::setTable(const QString &newtable)
{
    mTable = newtable;
}

void SqlModel::setPrimaryKeyField(const QString& keyname)
{
    mPrimaryKeyField = keyname;
}

void SqlModel::setPrimaryKey(const boost::any& key)
{
    mPrimaryKey = key;
}

const QString& SqlModel::table() const
{
    return mTable;
}

const QString& SqlModel::keyfield() const
{
    return mPrimaryKeyField;
}

const boost::any&  SqlModel::key() const
{
    return mPrimaryKey;
}

QSqlDatabase*  SqlModel::db()
{
    return mDatabase;
}

