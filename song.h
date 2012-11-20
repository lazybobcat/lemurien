#ifndef SONG_H
#define SONG_H

/**
 * @file
 * @brief This file implements the Songs class and tools
 * @author BOUTTER Loïc
 * @version 1
 * @date 2012-11
 */

#include "sqlmodel.h"
#include "config.h"
#ifdef DEBUG
#include <iostream>
#endif


/**
 * @class Song
 * @brief The Song class represent a song and its datas, like title, author, filepath, etc...
 * @details The Song class is directly linked to the database (inherits from SqlModel) so you can
 *          construct a song from data in the table 'songs' and/or save a song in database.\n
 *          You shouldn't construct a Song by yourself, use SqlModelFactory instead.
 * @see SqlModelFactory, save(), construct()
 */
class Song : public SqlModel
{
    #ifdef DEBUG
    friend std::ostream& operator<<(std::ostream&, const Song*);
    #endif

public:
    /**
     * @brief Song constructor, you shouldn't construt a Song by yourself, it's done by SqlModelFactory
     * @param db Is a pointer on the QSqlDatabase containing all data needed.
     */
    Song(QSqlDatabase* db);

    /**
     * @brief Song destructor (default)
     */
    virtual ~Song() { }

    /**
     * @brief Save the song in the database.
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


    /**
     * @brief Sets the song title if not empty
     * @param title The new title
     */
    void    setTitle(const QString& title);

    /**
     * @brief Sets the song author if not empty
     * @param the_artist The new artist
     */
    void    setArtist(const QString& the_artist);

    /**
     * @brief Sets the song album if not empty
     * @param album The new album
     */
    void    setAlbum(const QString& album);

    /**
     * @brief Sets the song filepath if not empty
     * @param filepath The new filepath
     */
    void    setFilepath(const QString& filepath);

    /**
     * @brief Sets the song mark if not between 0 and Config::cMaxMark
     * @param mark The new mark
     */
    void    setMark(unsigned int mark);

    /**
     * @brief Sets the song lyrics if not empty
     * @param lyrics The new lyrics
     */
    void    setLyrics(const QString& lyrics);

    /**
     * @brief Add 1 to the number of plays of this song
     */
    void    playOneMoreTime();

private:
    QString         mTitle;     ///< The song title
    QString         mArtist;    ///< The song author/artist
    QString         mAlbum;     ///< The song album
    QString         mFilepath;  ///< The song file path, unique in database.
    unsigned int    mNbPlay;    ///< The number of times the song has been played
    unsigned int    mMark;      ///< A mark between 0 and Config::cMaxMark for that song
    QString         mLyrics;    ///< The song lyrics, can be pretty heavy
};



#endif // SONG_H
