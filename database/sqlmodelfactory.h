#ifndef SQLMODELFACTORY_H
#define SQLMODELFACTORY_H

/**
 * @file
 * @brief This file contains a Factory used to create SqlModel objects.
 * @author BOUTTER Loïc
 * @version 1
 * @date 2012-11
 */

#include "../utilities/factory.h"
#include "databasemanager.h"
#include "../playlists/playlist.h"
#include "../playlists/unsavabeplaylist.h"
#include <boost/make_shared.hpp>
#include <map>
#include <QVariant>

/**
 * @class SqlModelFactory
 * @brief The SqlModelFactory class inherits from Factory in order to provide SqlModel children classes. It is a Singleton.
 * @details In order to save or load objects drom database, we need our Factory to create and sometimes initialize SqlModel children
 *          and to keep a track of created Songs. Indeed, to avoid multiple creation of the same Song, each created Song remains in
 *          the mSongs map for a future usage
 */
class SqlModelFactory : public Factory< SqlModel >
{
public:
    /**
     * @brief Returns a unique insance on SqlModelFactory. You cannot construct a SqlModelFactory by yourself (singleton).
     * @return
     */
    static SqlModelFactory* instance();

    /**
     * @brief Initialisation method. For now, it opens the Database.
     */
    void                init();

    /**
     * @brief Creates and initialize a Song given its id (passed as parameter) and returns a shared_ptr on it
     * @param id The id, primary key of the song in the table songs
     * @return A boost::shared_ptr<Song> on the song
     */
    Song::Ptr           createSong(unsigned int id);

    /**
     * @brief Search a Song given its filepath (create it if it does not exists) and returns a shared_ptr on it
     *        and a boolean (true if the song already exists, false if it has to be inited)
     * @param filepath The filepath of the song
     * @param song A reference on the song that will be modified by this method
     * @return True if the song was already in database and is constructed, false if there were no data in database
     */
    bool                getSong(const QString& filepath, Song::Ptr& song);

    /**
     * @brief Creates a Playlist and returns a shared_ptr on it
     * @return A boost::shared_ptr<Playlist> on the playlist
     */
    Playlist::Ptr       createPlaylist();

    /**
     * @brief Getter for DatabaseManager
     * @return Pointer on the unique DatabaseManager
     */
    DatabaseManager*    dbManager();

    /**
     * @brief Creates a SqlModel children depending on the parameter.
     * @param type The type of object you want ("song", "playlist", "unsavable_playlist")
     * @return A SqlModel pointer on a children object
     */
    virtual SqlModel* create(const std::string &type);

private:
    explicit SqlModelFactory(); ///< Explicit private constructor -> singeton implementation
    explicit SqlModelFactory(const SqlModelFactory&); ///< Explicit private copy constructor -> singeton implementation
    SqlModelFactory& operator=(const SqlModelFactory&); ///< Explicit private copy operator -> singeton implementation



    static SqlModelFactory* mInstance;  ///< Static and unique (global variable) instance of SqlModelFactory
    DatabaseManager  mDBManager;        ///< That DatabaseManager should be unique because it is contained by a singleton
    std::map<unsigned int, Song::Ptr> mSongs; ///< Map that holds the songs indexed by their primary keys
};

#endif // SQLMODELFACTORY_H
