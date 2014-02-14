#ifndef SONG_H
#define SONG_H

/**
 * @file
 * @brief This file implements the Song class and related tools
 * @author BOUTTER Loïc
 * @version 4
 * @date 2013-02
 */

#include "database/sqlmodel.h"
#include "config.h"
#include <boost/shared_ptr.hpp>
//#include <memory> // For C++11
#ifdef DEBUG
#include <iostream>
#endif


/**
 * @class Song
 * @brief The Song class represent a song and its datas, like title, author, filepath, etc...
 * @details The Song class is directly linked to the database (inherits from SqlModel) so you can
 *          construct a song from data in the table 'songs' and/or save a song in database.\n
 *          You shouldn't construct a Song by yourself, use SqlModelFactory instead.
 * @see SqlModelFactory, save(), construct(), Playlist
 */
class Song : public SqlModel
{
    #ifdef DEBUG
    friend std::ostream& operator<<(std::ostream&, const Song*);
    #endif
    friend class SqlModelFactory;

public:
    /**
     * @brief Make our life simple
     */
    typedef boost::shared_ptr<Song> Ptr;

    /**
     * @brief Song constructor, you shouldn't construt a Song by yourself, it's done by SqlModelFactory
     * @param db Is a pointer on the QSqlDatabase containing all data needed.
     */
    Song(QSqlDatabase* db);

    /**
     * @brief Song destructor (default)
     */
    virtual ~Song();

    /**
     * @brief Save the song in the database.
     * @details This add the song in the database if that song is not already in it, else do an update of the song data.
     * This method can return different SqlModel errors :
     *     - LogicalFaultException if the pimary key isn't set before you try to construct the Song
     *     - SqlDatabaseException if there is a problem with the database connection
     *     - SqlInsertFailedException if for some reason, the inserting failed
     */
    virtual void save() throw(SqlException);

    /**
     * @brief Construct a Song from a line in the table 'songs'.
     *  Beware: the primary key MUST be set ( with setPrimaryKey() ), else it returns a SqlModel::Error
     * @details This method can throw different SqlModel errors:
     *      - LogicalFaultException if the pimary key isn't set before you try to construct the Song
     *      - SqlDatabaseException if there is a problem with the database connection
     *      - SqlDataNotFoundException if the data hasn't been found
     */
    virtual void construct() throw(SqlException, LogicalFaultException);

    /**
     * @brief Removes the song from database.
     * @details This removes the song from database if the primary key has been set (the song is saved in database).
     *          Else it does nothing.
     * This method can return different SqlModel errors :
     *      - LogicalFaultException if the pimary key isn't set before you try to construct the Song
     *      - SqlDatabaseException if there is a problem with the database connection
     *      - SqlDataNotFoundException if the data hasn't been found
     */
    virtual void erase() throw(SqlException, LogicalFaultException);


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


    QString title() const;      ///< Title Getter
    QString artist() const;     ///< Artist Getter
    QString album() const;      ///< Album Getter
    QString filepath() const;   ///< Filepath Getter
    QString lyrics() const;     ///< Lyrics Getter

    unsigned int mark() const;  ///< Mark Getter
    unsigned int nbplay() const;///< Number of plays Getter

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
