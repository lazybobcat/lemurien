#include "sqlmodelfactory.h"
#include <iostream>

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
    else if(type == "unsavable_playlist")
        return new UnsavablePlaylist(mDBManager.db());

    return NULL;
}

Song::Ptr SqlModelFactory::createSong(unsigned int id)
{
    // Check if the song has already been created
    if(mSongs.find(id) != mSongs.end())
        return mSongs[id]; // If yes we return it

    // Else, get a SqlModel* from create() method
    // Cast it in Song* (and we're sure it's a Song* or NULL)
    // Give the ownership to the shared_ptr (Song::Ptr)
    // Init the song
    // return boost::make_shared<Song> on the song
    Song::Ptr song(static_cast<Song*>(create("song")));

    song->setPrimaryKey(id);
    song->construct();
    mSongs[song->key()] = song; // Add the song to the map, for a future usage

    return song;
}

bool SqlModelFactory::getSong(const QString& filepath, Song::Ptr& song)
{
    if(!mDBManager.db()->isOpen())
        mDBManager.db()->open();

    QSqlQuery query(QString("SELECT id FROM songs WHERE filepath='%1'").arg(filepath));
    if(query.next())
    {
        if(query.isValid())
        {
            song = createSong(query.value(0).toUInt());
            return true;
        }
    }

    song = Song::Ptr(static_cast<Song*>(create("song")));
    song->setFilepath(filepath);
    try {
        song->save();
        mSongs[song->key()] = song; // Add the song to the map, for a future usage
    }
    catch(SqlInsertFailedException& e)
    {
        std::cerr << "The saving of a song failed in " << __FILE__ << ". If the song already exists in database, check the logic of the code that leads here." << std::endl;
    }

    return false;
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
