#include "playlist.h"
#include <QVariant>
#include "../database/sqlmodelfactory.h"




/*****************************************************
 *
 *             PLAYLIST METHODS
 *
 ****************************************************/





Playlist::Playlist(QSqlDatabase* db) : SqlModel(db, "playlists"), QMap(), mName("Playlist Sans Titre")
{
}

Playlist::~Playlist()
{

}


void Playlist::save() throw(SqlException)
{
    if(db()->isOpen())
    {
        //////////////////////////////////////////////////////////////////////////////////
        ///////////                  We save playlist first !                  ///////////
        //////////////////////////////////////////////////////////////////////////////////

        /* Firstly, we want to save the playlist information in database.
         * If the playlist has neven been saved yet, we insert it into playlists table.
         * Else we just update it.
         */

        // If the playlist hasn't been saved yet in the database, it needs an INSERT
        if(!SqlModel::key())
        {
            std::cout << "will save playlist" << std::endl;
            // Prepare the query and then bind values
            QSqlQuery query;
            query.prepare("INSERT INTO playlists VALUES(NULL, :name)");
            // We've set the ID to null because AUTOINCREMENT is activated, so let SQLite do its job !
            query.bindValue(":name", mName);
            // Execution
            if(!query.exec()) // In case of error
                throw SqlInsertFailedException("SQL Error : Inserting playlist failed");
            else // If the song has been saved, we get is ID (wich is the last inserted Id)
                setPrimaryKey(query.lastInsertId().toUInt());
        }
        else // else it needs an UPDATE
        {
            // Prepare the query and then bind values
            QSqlQuery query;
            query.prepare("UPDATE playlists SET name=:name WHERE id=:id");
            query.bindValue(":id", SqlModel::key());
            query.bindValue(":name", mName);
            // Execution
            if(!query.exec()) // In case of error
                throw SqlInsertFailedException("SQL Error : Updating playlists failed");
        }


        //////////////////////////////////////////////////////////////////////////////////
        ///////////               We save playlists_songs now                  ///////////
        //////////////////////////////////////////////////////////////////////////////////

        /* We want to save each song that beongs to the playlist in a tabl that keep track of
         * the relations between playlists and songs : playlists_songs.
         * That table associate the id of the playlist with ids of songs that belongs to playlist
         * Note that a song can belongs to multiple playlists but once per playlist.
         */

        auto it = begin(); // Iterator on the begining of the playlist
        // Get the playlist database primary key, to avoid calling that method X times.
        unsigned int this_key = SqlModel::key();

        // Iteration on the playlist to save each song in playlists_songs database
        for(; it != end(); ++it)
        {
            if((*it)->key() == 0)
                (*it)->save();

            // Prepare the query and then bind values
            QSqlQuery query;
            query.prepare("INSERT INTO playlists_songs VALUES(:idP, :idS)");
            query.bindValue(":idP", this_key);
            query.bindValue(":idS", (*it)->key());
            // Exec the query. Note that we do not returns error because we gonna insert data pairs
            // that are UNIQUE in the table, and we do it on purpose.
            query.exec();
        }

    }
    else
        throw SqlDatabaseException("SQL Error : Database is not open");
}

void Playlist::construct() throw(SqlException, LogicalFaultException)
{
    if(!SqlModel::key())
        throw LogicalFaultException("Logical Fault : the key has not been set before construction of playlist");

    if(db()->isOpen())
    {
        //////////////////////////////////////////////////////////////////////////////////
        ///////////               We load playlist data first !                ///////////
        //////////////////////////////////////////////////////////////////////////////////

        // Get the playlist in the database
        QSqlQuery query(QString("SELECT name FROM playlists WHERE id=%1").arg(SqlModel::key()));
        if(query.next()) // Get to the data
        {
            mName = query.value(0).toString();  // Extract the playlist name
        }
        else
            throw SqlDataNotFoundException("SQL Error : the data of the playlist (by id) has not been found"); // Throw an error if one occured



        //////////////////////////////////////////////////////////////////////////////////
        ///////////               We load playlists_songs now                  ///////////
        //////////////////////////////////////////////////////////////////////////////////

        // Get the playlist in the database
        query = QSqlQuery(QString("SELECT id_song, position FROM playlists_songs WHERE id_playlist=%1").arg(SqlModel::key()));

        // Multiple results, we iterate on query.next() while it's true
        unsigned int id_song;
        unsigned int position;
        Song::Ptr   tmp_song;
        while(query.next()) // Get to the data
        {
            id_song = query.value(0).toUInt();
            position = query.value(1).toUInt();
            /**
             * @todo This method is right but surely slower than to do a SELECT with a JOIN on both playlists_songs and songs tables,
             * but that make us rewrite same code than in Song::construct().
             * I've to think about it
             */
            // We are going to construct a Song based on the id we get
            tmp_song.reset();   // Reset the previous Song::Ptr
            tmp_song = SqlModelFactory::instance()->createSong(id_song); // Get a the song corresponding to the primary key (ID)
            insert(position, tmp_song);   // Finally we push it in the list
        }

    }
    else
        throw SqlDatabaseException("SQL Error : Database is not open");
}

void Playlist::erase() throw(SqlException, LogicalFaultException)
{
    if(!SqlModel::key())
        throw LogicalFaultException("Logical Fault : the key has not been set before erasing a playlist");

    if(db()->isOpen())
    {
        // Remove all data from table 'playlists_songs' first
        QSqlQuery query(QString("DELETE FROM playlists_songs WHERE id_playlist=%1").arg(SqlModel::key()));

        // In case of error
        if(query.lastError().type() != QSqlError::NoError)
            throw SqlDataNotFoundException("SQL Error : error while deleting playlist songs"); // Throw an error if one occured



        // Then we can remove the playlist from table 'playlits'
        query.exec(QString("DELETE FROM playlists WHERE id=%1").arg(SqlModel::key()));

        // In case of error
        if(query.lastError().type() != QSqlError::NoError)
            throw SqlDataNotFoundException("SQL Error : error while deleting playlist"); // Throw an error if one occured


        // Remove songs from the list
        clear();

    }
    else
        throw SqlDatabaseException("SQL Error : Database is not open");
}


/*void Playlist::sort(SortField type)
{
    switch(type)
    {
    case Playlist::SortOnTitle:
        qSort(begin(), end(), compareTitles);
        break;

    case Playlist::SortOnAlbum:
        qSort(begin(), end(), compareAlbums);
        break;

    case Playlist::SortOnArtist:
        qSort(begin(), end(), compareArtists);
        break;

    case Playlist::SortOnMark:
        qSort(begin(), end(), compareMarks);
        break;

    case Playlist::SortOnNbplay:
        qSort(begin(), end(), compareNbplays);
        break;
    }
}*/


void Playlist::removeOne(const Song::Ptr song)
{
    eraseSongFromPlaylist(song);
}

void Playlist::remove(const Song::Ptr song)
{
    eraseSongFromPlaylist(song);
}


void Playlist::eraseSongFromPlaylist(Song::Ptr song)
{
    unsigned int map_key = QMap::key(song);
    if(db()->isOpen() && contains(map_key))
    {
        // Get the playlist in the database
        QSqlQuery query(QString("DELETE FROM playlists_songs WHERE id_playlist=%1 AND id_song=%2").arg(SqlModel::key()).arg(song->key()));
        // Hop, adios amigos !

        QMap::remove(map_key);
    }
}


void Playlist::append(const Song::Ptr song)
{
    unsigned int index = size() + 1u;
    insert(index, song);
}


void Playlist::rename(const QString &name)
{
    mName = name;
}

QString Playlist::name() const
{
    return mName;
}






/*****************************************************
 *
 *             OTHER FUNCTIONS
 *
 ****************************************************/


/*bool caseInsensitiveLessThan(const QString& s1, const QString& s2)
{
    return (s1.toLower() < s2.toLower());
}

bool uintMoreThan(const unsigned int number1, const unsigned int number2)
{
    return (number1 > number2);
}



bool compareTitles(const Song::Ptr song1, const Song::Ptr song2)
{
    return caseInsensitiveLessThan(song1->title(), song2->title());
}

bool compareArtists(const Song::Ptr song1, const Song::Ptr song2)
{
    return caseInsensitiveLessThan(song1->artist(), song2->artist());
}

bool compareAlbums(const Song::Ptr song1, const Song::Ptr song2)
{
    return caseInsensitiveLessThan(song1->album(), song2->album());
}

bool compareMarks(const Song::Ptr song1, const Song::Ptr song2)
{
    return uintMoreThan(song1->mark(), song2->mark());
}

bool compareNbplays(const Song::Ptr song1, const Song::Ptr song2)
{
    return uintMoreThan(song1->nbplay(), song2->nbplay());
}*/



#ifdef DEBUG
inline std::ostream& operator<<(std::ostream& os, const QString& s)
{
    os << s.toStdString();
    return os;
}

std::ostream& operator<<(std::ostream& os, const Playlist* p)
{
    os << p->mName << ":" << std::endl;

    auto it = p->begin(); // Iterator on the begining of the playlist

    // Iteration on the playlist to save each song in playlists_songs database
    for(; it != p->end(); ++it)
    {
        std::cout << "\t" << (*it) << std::endl;
    }

    return os;
}
#endif
