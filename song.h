#ifndef SONG_H
#define SONG_H

#include "sqlmodel.h"
#include <QSqlDatabase>

class Song : public SqlModel
{
public:
    Song(QSqlDatabase* db);
    virtual ~Song() { }

    virtual void save() throw(Error)
    {

    }

    virtual void construct() throw(Error)
    {

    }

    void truc() { }
};

#endif // SONG_H
