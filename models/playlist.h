#ifndef PLAYLIST_H
#define PLAYLIST_H

/**
 * @file
 * @brief This file implements the Playlist class and related tools
 * @author BOUTTER Loïc
 * @version 2
 * @date 2012-11
 */

#include "song.h"
#include <QMap>
#include <QtAlgorithms>


/**
 * @class Playlist
 * @brief The Playlist class is a list of Songs with some more informations (a name).
 * @details The Playlist class is directly linked to the database (inherits from SqlModel) so you can
 *          construct a playlist from data in the table 'playlists' and 'playlists_songs' and/or save
 *          the playlist in database.\n
 * @see SqlModelFactory, Song, save(), construct()
 */
class Playlist : public SqlModel, public QMap<unsigned int,Song::Ptr>
{
    #ifdef DEBUG
    friend std::ostream& operator<<(std::ostream&, const Playlist*); ///< Allow stream operator to access our attributes
    #endif

public:
    typedef std::shared_ptr<Playlist> Ptr; ///< Make our life simple.

    /**
     * @brief The SortField enum to set on wich field we want to sort the playlist
     */
    /*enum SortField {
        SortOnTitle,    ///< Sort field = title
        SortOnArtist,   ///< Sort field = artist
        SortOnAlbum,    ///< Sort field = album
        SortOnMark,     ///< Sort field = mark
        SortOnNbplay    ///< Sort field = number of times the song has been played
    };*/

    /**
     * @brief Playlist constructor, you shouldn't construt a Playlist by yourself, it's done by SqlModelFactory
     * @param db Is a pointer on the QSqlDatabase containing all data needed.
     */
    Playlist(QSqlDatabase* db);

    /**
     * @brief ~Playlist default destructor
     */
    virtual ~Playlist();

    /**
     * @brief Save the playlist in the database.
     * @details This add the playlist in the database if that playlist is not already in it, else do an update of the playlist data.\n
     *          Furthermore, every song that are in the playlist are added (saved/updated) to the database.\n
     * This method can return different SqlModel errors :
     *     - LogicalFaultException if the pimary key isn't set before you try to construct the Playlist
     *     - SqlDatabaseException if there is a problem with the database connection
     *     - SqlInsertFailedException if for some reason, the inserting failed
     */
    virtual void    save() throw(SqlException);

    /**
     * @brief Construct a Playlist from a line in the table 'playlists' and lines in 'playlists_songs'.
     *  Beware: the primary key MUST be set ( with setPrimaryKey() ), else it returns a SqlModel::Error
     * @details This method can throw different SqlModel errors:
     *      - LogicalFaultException if the pimary key isn't set before you try to construct the Playlist
     *      - SqlDatabaseException if there is a problem with the database connection
     *      - SqlDataNotFoundException if the data hasn't been found
     */
    virtual void    construct() throw(SqlException, LogicalFaultException);

    /**
     * @brief Removes the Playlist from database and clear() it.
     * @details This removes the playlist from database if the primary key has been set (saved in database).
     *          Else it does nothing. This remove all datain table 'playlists_songs', then in 'playlists' and
     *          clear() the list of songs.
     * This method can return different SqlModel errors :
     *      - LogicalFaultException if the pimary key isn't set before you try to construct the Playlist
     *      - SqlDatabaseException if there is a problem with the database connection
     *      - SqlDataNotFoundException if the data hasn't been found
     */
    virtual void    erase() throw(SqlException, LogicalFaultException);



    /**
     * @brief Sorts the playlist on the given parameter
     * @param type The Sorting Type / the field on wich you want to sort the playlist
     */
    //void            sort(SortField  type);


    void            append(const Song::Ptr song);


    /**
     * @brief Renames the playlist
     * @param name The new name
     */
    void            rename(const QString& name);

    /**
     * @brief This is an overload of QList::removeOne, this remove the song from the list
     *        and from table playlists_songs. In fact this method calls eraseSongFromPlaylist().
     * @param song Song to remove
     * @see eraseSongFromPlaylist()
     */
    void            removeOne(const Song::Ptr song);

    /**
     * @brief This remove the song from the list and from table playlists_songs. In fact this
     * method calls eraseSongFromPlaylist().
     * @param song Song to remove
     * @see eraseSongFromPlaylist()
     */
    void            remove(const Song::Ptr song);

    /**
     * @brief Returns the playlist name
     * @return The Playlist name
     */
    QString         name() const;

private:
    /**
     * @brief If the song exists and database is open, this will remove the song from
     *        the playlist and the database (in the table playlists_songs)
     * @param song Song to remove
     */
    void            eraseSongFromPlaylist(Song::Ptr song);

    QString         mName;  ///< The Playlist name, by default : "Playlist Sans Titre"
};


/**
 * @brief Compare two string without taking the case in account
 * @param s1 The first string to compare
 * @param s2 The second string to compare
 * @return true if s1 < s2
 */
bool caseInsensitiveLessThan(const QString& s1, const QString& s2);
/**
 * @brief Compare two unsined ints
 * @param number1
 * @param number2
 * @return true if number1 > number2
 */
bool uintMoreThan(const unsigned int number1, const unsigned int number2);

bool compareTitles(const Song::Ptr song1, const Song::Ptr song2);   ///< Compare songs titles
bool compareArtists(const Song::Ptr song1, const Song::Ptr song2);  ///< Compare songs artists
bool compareAlbums(const Song::Ptr song1, const Song::Ptr song2);   ///< Compare songs albums
bool compareMarks(const Song::Ptr song1, const Song::Ptr song2);    ///< Compare songs marks
bool compareNbplays(const Song::Ptr song1, const Song::Ptr song2);  ///< Compare songs number of plays

#endif // PLAYLIST_H
