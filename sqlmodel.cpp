#include "sqlmodel.h"

SqlModel::SqlModel(QSqlDatabase* db, const std::string &tablename, const std::string& primarykeyname)
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


void SqlModel::setTable(const std::string &newtable)
{
    mTable = newtable;
}

void SqlModel::setPrimaryKeyField(const std::string& keyname)
{
    mPrimaryKeyField = keyname;
}

void SqlModel::setPrimaryKey(const boost::any& key)
{
    mPrimaryKey = key;
}

const std::string& SqlModel::table() const
{
    return mTable;
}

const std::string& SqlModel::keyfield() const
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

