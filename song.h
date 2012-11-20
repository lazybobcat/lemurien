#ifndef SONG_H
#define SONG_H

#include "sqlmodel.h"
#include "config.h"
#include <QSqlDatabase>
#include <QString>
#include <iostream>

class Song : public SqlModel
{
    friend std::ostream& operator<<(std::ostream&, const Song*);

public:
    Song(QSqlDatabase* db);
    virtual ~Song() { }

    /**
     * @brief Save the song in the database. The primary key MUST be set !
     * @details This add the song in the database if that song is not already in it, else do an update of the song data.
     * This method can return different SqlModel errors :
     *     - SqlModel::LogicalError if the pimary key isn't set before you try to construct the Song
     *     - SqlModel::SqlError if there is a problem with the database connection
     *     - SqlModel::InsertFailed if for some reason, the inserting failed (it's often because of the UNIQUE
     *          on the filepath : the song is already in database)
     */
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


    void    setTitle(const QString& title);
    void    setArtist(const QString& the_artist);
    void    setAlbum(const QString& album);
    void    setFilepath(const QString& filepath);
    void    setMark(unsigned int mark);
    void    setLyrics(const QString& lyrics);
    void    playOneMoreTime();

private:
    QString         mTitle;
    QString         mArtist;
    QString         mAlbum;
    QString         mFilepath;
    unsigned int    mNbPlay;
    unsigned int    mMark;
    QString         mLyrics;
};

extern std::ostream& operator<<(std::ostream&, const Song*);

#endif // SONG_H
