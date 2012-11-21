#ifndef SQLMODELFACTORY_H
#define SQLMODELFACTORY_H

/**
 * @file
 * @brief This file contains a Factory used to create SqlModel objects.
 * @author BOUTTER Loïc
 * @version 1
 * @date 2012-11
 */

#include "factory.h"
#include "databasemanager.h"
#include "playlist.h"
#include <boost/make_shared.hpp>

/**
 * @class SqlModelFactory
 * @brief The SqlModelFactory class inherits from Factory in order to provide SqlModel children classes. It is a Singleton.
 * @details In order to save or load objects drom database, we need our Factory to create and sometimes initialize SqlModel children.
 *
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
     * @brief Creates a Song and returns a shared_ptr on it
     * @return A boost::shared_ptr<Song> on the song
     */
    Song::Ptr           createSong();

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

private:
    explicit SqlModelFactory(); ///< Explicit private constructor -> singeton implementation
    explicit SqlModelFactory(const SqlModelFactory&); ///< Explicit private copy constructor -> singeton implementation
    SqlModelFactory& operator=(const SqlModelFactory&); ///< Explicit private copy operator -> singeton implementation

    /**
     * @brief Creates a SqlModel children depending on the parameter.
     * @param type The type of object you want ("song", "playlist")
     * @return A SqlModel pointer on a children object
     */
    virtual SqlModel* create(const std::string &type);

    static SqlModelFactory* mInstance;  ///< Static and unique (global variable) instance of SqlModelFactory
    DatabaseManager  mDBManager;        ///< That DatabaseManager should be unique because it is contained by a singleton
};

#endif // SQLMODELFACTORY_H
