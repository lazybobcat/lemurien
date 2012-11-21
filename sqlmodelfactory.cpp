#include "sqlmodelfactory.h"

SqlModelFactory* SqlModelFactory::mInstance = NULL;

SqlModelFactory* SqlModelFactory::instance()
{
    if(!mInstance)
        mInstance = new SqlModelFactory();

    return mInstance;
}

SqlModelFactory::SqlModelFactory() : mDBManager()
{

}

void SqlModelFactory::init()
{
    mDBManager.open();
}

SqlModel* SqlModelFactory::create(const std::string &type)
{
    if(type == "song")
        return new Song(mDBManager.db());
    else if(type == "playlist")
        return new Playlist(mDBManager.db());

    return NULL;
}

Song::Ptr SqlModelFactory::createSong()
{
    // Get a SqlModel* from create() method
    // Cast it in Song* (and we're sure it's a Song* or NULL)
    // Give the ownership to the shared_ptr (Song::Ptr) an return it
    //return boost::make_shared<Song>(mDBManager.db());
    return Song::Ptr(static_cast<Song*>(create("song")));
}

Playlist::Ptr SqlModelFactory::createPlaylist()
{
    // Get a SqlModel* from create() method
    // Cast it in Playlist* (and we're sure it's a Playlist* or NULL)
    // Give the ownership to the shared_ptr (Playlist::Ptr) an return it
    return Playlist::Ptr(static_cast<Playlist*>(create("playlist")));
}


DatabaseManager* SqlModelFactory::dbManager()
{
    return &mDBManager;
}
