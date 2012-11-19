#ifndef SONG_H
#define SONG_H

#include "sqlmodel.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <string>
#include <iostream>

class Song : public SqlModel
{
    friend std::ostream& operator<<(std::ostream&, const Song*);

public:
    Song(QSqlDatabase* db);
    virtual ~Song() { }

    virtual void save() throw(Error);

    /**
     * @brief Construct a Song from a line in the table 'songs'.
     *  Beware: the primary key MUST be set ( with setPrimaryKey() ), else it returns a SqlModel::Error
     * @details This method can throw different SqlModel errors:
     *      - SqlModel::LogicalError if the pimary key isn't set before you try to construct the Song
     *      - SqlModel::SqlError if there is a problem with the database connection
     *      - SqlModel::DataNotFound if the data hasn't been found
     */
    virtual void construct() throw(Error);

private:
    std::string     mTitle;
    std::string     mArtist;
    std::string     mAlbum;
    std::string     mFilepath;
    unsigned int    mNbPlay;
    unsigned int    mMark;
    std::string     mLyrics;
};

extern std::ostream& operator<<(std::ostream&, const Song*);

#endif // SONG_H
