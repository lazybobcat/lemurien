#ifndef PLAYLIST_H
#define PLAYLIST_H

/**
 * @file
 * @brief This file implements the Playlist class and related tools
 * @author BOUTTER Loïc
 * @version 1
 * @date 2012-11
 */

#include "song.h"
#include <QList>


/**
 * @class Playlist
 * @brief The Playlist class is a list of Songs with some more informations (a name).
 * @details The Playlist class is directly linked to the database (inherits from SqlModel) so you can
 *          construct a playlist from data in the table 'playlists' and 'playlists_songs' and/or save
 *          the playlist in database.\n
 * @see SqlModelFactory, Song, save(), construct()
 */
class Playlist : public SqlModel, public QList<Song::Ptr>
{
    #ifdef DEBUG
    friend std::ostream& operator<<(std::ostream&, const Playlist*); ///< Allow stream operator to access our attributes
    #endif

public:
    typedef boost::shared_ptr<Playlist> Ptr; ///< Make our life simple.

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
     *     - SqlModel::LogicalError if the pimary key isn't set before you try to construct the Playlist
     *     - SqlModel::SQLError if there is a problem with the database connection
     *     - SqlModel::InsertFailed if for some reason, the inserting failed
     */
    virtual void    save() throw(SqlModel::Error);

    /**
     * @brief Construct a Playlist from a line in the table 'playlists' and lines in 'playlists_songs'.
     *  Beware: the primary key MUST be set ( with setPrimaryKey() ), else it returns a SqlModel::Error
     * @details This method can throw different SqlModel errors:
     *      - SqlModel::LogicalError if the pimary key isn't set before you try to construct the Song
     *      - SqlModel::SQLError if there is a problem with the database connection
     *      - SqlModel::DataNotFound if the data hasn't been found
     */
    virtual void    construct() throw(SqlModel::Error);

    /**
     * @brief Removes the Playlist from database and clear() it.
     * @details This removes the playlist from database if the primary key has been set (saved in database).
     *          Else it does nothing. This remove all datain table 'playlists_songs', then in 'playlists' and
     *          clear() the list of songs.
     * This method can return different SqlModel errors :
     *     - SqlModel::LogicalError if the pimary key isn't set before you try to construct the Song
     *     - SqlModel::SQLError if there is a problem with the database connection
     *     - SqlModel::DataNotFound the line hasn't been found nor erased for some reason.
     */
    virtual void    erase() throw(SqlModel::Error);



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

#endif // PLAYLIST_H
