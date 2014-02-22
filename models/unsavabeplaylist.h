#ifndef UNSAVABEPLAYLIST_H
#define UNSAVABEPLAYLIST_H

#include "playlist.h"

/**
 * @brief This is a special playlist that canno be saved in database. The ain use is for searches, as results are put in a temporary playlist that we don't want to save.
 * @see Playlist
 */
class UnsavablePlaylist : public Playlist
{
public:
    UnsavablePlaylist(QSqlDatabase* db) : Playlist(db)
    {

    }

    virtual ~UnsavablePlaylist()
    {

    }

    /**
     * @brief Empty saving method, indeed we do not want to save anything
     */
    virtual void save() throw()
    {

    }
};

#endif // UNSAVABEPLAYLIST_H
