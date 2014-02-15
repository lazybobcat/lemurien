#include "databasemanager.h"
#include <QVariant>

#include <iostream>

DatabaseManager::DatabaseManager(QString dbname)
{
    // Tell Qt we use an SQLite database
    mDatabase = QSqlDatabase::addDatabase("QSQLITE");
    // Fixed database name
    mDatabase.setDatabaseName(dbname + ".sqlite");
    // Open the database and create the tables if not exists
    open();
    createTables();
}

DatabaseManager::~DatabaseManager()
{
    close();
}

void DatabaseManager::open() throw(SqlDatabaseException)
{
    if(!mDatabase.open())
        throw SqlDatabaseException("SQL Error : the database cannot be open");
}

void DatabaseManager::close()
{
    mDatabase.close();
}

QSqlError DatabaseManager::lastError()
{
    return mDatabase.lastError();
}


QSqlDatabase* DatabaseManager::db()
{
    return &mDatabase;
}


void DatabaseManager::createTables() throw(SqlDatabaseException)
{
    bool ret = false;
    if(mDatabase.isOpen())
    {
        QSqlQuery query;


        /*************************************************************************************************************
         *                                       Creating table songs
         ************************************************************************************************************/

        ret = query.exec("CREATE TABLE IF NOT EXISTS songs("
                         "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                         "title VARCHAR(50), "
                         "artist VARCHAR(50), "
                         "album VARCHAR(50), "
                         "filepath VARCHAR(150) UNIQUE NOT NULL, "
                         "nbplay INTEGER, "
                         "mark INTEGER,"
                         "lyrics TEXT"
                         ")"); // After a lot of different creation, setting the primary key on id to
                               // benefit from auto-increment and unique on filepath to only have one
                               // song corresponding to one file is the best way

        // If there was an error while creating the tables.
        if(!ret)    throw SqlDatabaseException("SQL Error : Database access error"); // Throw an error if one occured





        /*************************************************************************************************************
         *                                       Creating table playlists
         ************************************************************************************************************/
        ret = query.exec("CREATE TABLE IF NOT EXISTS playlists("
                         "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                         "name VARCHAR(50) NOT NULL"
                         ")");

        // If there was an error while creating the tables.
        if(!ret)    throw SqlDatabaseException("SQL Error : Database access error"); // Throw an error if one occured





        /*************************************************************************************************************
         *                            Inserting the main playlist ('Toute la musique')
         ************************************************************************************************************/
        query.exec("INSERT INTO playlists VALUES(1, 'All Music')");






        /*************************************************************************************************************
         *                                Creating table playlists_songs
         ************************************************************************************************************/
        ret = query.exec("CREATE TABLE IF NOT EXISTS playlists_songs("
                         "id_playlist INTEGER, "
                         "id_song INTEGER, "
                         "position INTEGER, "
                         "PRIMARY KEY(id_playlist, id_song),"
                         "FOREIGN KEY(id_playlist) REFERENCES playlists(id),"
                         "FOREIGN KEY(id_song) REFERENCES songs(id)"
                         ")");

        // If there was an error while creating the tables.
        if(!ret)    throw SqlDatabaseException("SQL Error : Database access error"); // Throw an error if one occured
    }
}
