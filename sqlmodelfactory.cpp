#include "sqlmodelfactory.h"

SqlModelFactory* SqlModelFactory::mInstance = NULL;

SqlModelFactory* SqlModelFactory::instance()
{
    if(!mInstance)
        mInstance = new SqlModelFactory();

    return mInstance;
}

SqlModelFactory::SqlModelFactory() : mDBManager()
{

}

void SqlModelFactory::init()
{
    mDBManager.open();
}

SqlModel* SqlModelFactory::create(const std::string &type)
{
    if(type == "song")
        return new Song(mDBManager.db());

    return NULL;
}


DatabaseManager* SqlModelFactory::dbManager()
{
    return &mDBManager;
}
