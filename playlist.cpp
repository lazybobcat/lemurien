#include "playlist.h"
#include <QVariant>
#include "sqlmodelfactory.h"

Playlist::Playlist(QSqlDatabase* db) : SqlModel(db, "playlists"), QList(), mName("Playlist Sans Titre")
{
}

Playlist::~Playlist()
{

}


void Playlist::save() throw(SqlModel::Error)
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
        if(key().empty())
        {
            // Prepare the query and then bind values
            QSqlQuery query;
            query.prepare("INSERT INTO playlists VALUES(NULL, :name)");
            // We've set the ID to null because AUTOINCREMENT is activated, so let SQLite do its job !
            query.bindValue(":name", mName);
            // Execution
            if(!query.exec()) // In case of error
                throw SqlModel::InsertFailed;
            else // If the song has been saved, we get is ID (wich is the last inserted Id)
                setPrimaryKey(query.lastInsertId().toUInt());
        }
        else // else it needs an UPDATE
        {
            // Prepare the query and then bind values
            QSqlQuery query;
            query.prepare("UPDATE playlists SET name=:name WHERE id=:id");
            query.bindValue(":id", boost::any_cast<unsigned int>(key()));
            query.bindValue(":name", mName);
            // Execution
            if(!query.exec()) // In case of error
                throw SqlModel::InsertFailed;
        }


        //////////////////////////////////////////////////////////////////////////////////
        ///////////               We save playlists_songs now                  ///////////
        //////////////////////////////////////////////////////////////////////////////////

        /* We want to save each song that beongs to the playlist in a tabl that keep track of
         * the relations between playlists and songs : playlists_songs.
         * That table associate the id of the playlist with ids of songs that belongs to playlist
         * Note that a song can belongs to multiple playlists but once per playlist.
         */

        QList<Song::Ptr>::iterator it = begin(); // Iterator on the begining of the playlist
        // Get the playlist database primary key, to avoid calling that method X times.
        unsigned int this_key = boost::any_cast<unsigned int>(key());

        // Iteration on the playlist to save each song in playlists_songs database
        for(; it != end(); ++it)
        {
            if((*it)->key().empty())
                (*it)->save();

            // Prepare the query and then bind values
            QSqlQuery query;
            query.prepare("INSERT INTO playlists_songs VALUES(:idP, :idS)");
            query.bindValue(":idP", this_key);
            query.bindValue(":idS", boost::any_cast<unsigned int>((*it)->key()));
            // Exec the query. Note that we do not returns error because we gonna insert data pairs
            // that are UNIQUE in the table, and we do it on purpose.
            query.exec();
        }

    }
    else
        throw SqlModel::SQLError;
}

void Playlist::construct() throw(SqlModel::Error)
{
    if(key().empty())
        throw SqlModel::LogicalError;

    if(db()->isOpen())
    {
        //////////////////////////////////////////////////////////////////////////////////
        ///////////               We load playlist data first !                ///////////
        //////////////////////////////////////////////////////////////////////////////////

        // Get the playlist in the database
        QSqlQuery query(QString("SELECT name FROM playlists WHERE id=%1").arg(boost::any_cast<int>(key())));
        if(query.next()) // Get to the data
        {
            mName = query.value(0).toString();  // Extract the playlist name
        }
        else
            throw SqlModel::DataNotFound; // Throw an error if one occured



        //////////////////////////////////////////////////////////////////////////////////
        ///////////               We load playlists_songs now                  ///////////
        //////////////////////////////////////////////////////////////////////////////////

        // Get the playlist in the database
        query = QString("SELECT id_song FROM playlists_songs WHERE id_playlist=%1").arg(boost::any_cast<int>(key()));

        // Multiple results, we iterate on query.next() while it's true
        boost::any id_song;
        Song::Ptr   tmp_song;
        while(query.next()) // Get to the data
        {
            id_song = query.value(0).toUInt();
            /**
             * @todo This method is right but surely slower than to do a SELECT with a JOIN on both playlists_songs and songs tables,
             * but that make us rewrite same code than in Song::construct().
             * I've to think about it
             */
            // We are going to construct a Song based on the id we get
            tmp_song.reset();   // Reset the previous Song::Ptr
            tmp_song = SqlModelFactory::instance()->createSong(); // Get a new Song::Ptr, brand new
            tmp_song->setPrimaryKey(id_song);   // We give the song its primary key (its ID)
            tmp_song->construct();  // With the ID the song is able to construct itself from database
            append(tmp_song);   // Finally we push it in the list
        }

    }
    else
        throw SqlModel::SQLError;
}


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
    if(db()->isOpen() && contains(song))
    {
        // Get the playlist in the database
        QSqlQuery query(QString("DELETE FROM playlists_songs WHERE id_playlist=%1 AND id_song=%2").arg(boost::any_cast<unsigned int>(key())).arg(boost::any_cast<unsigned int>(song->key())));
        // Hop, adios amigos !

        QList<Song::Ptr>::removeOne(song);
    }
}



void Playlist::rename(const QString &name)
{
    mName = name;
}

QString Playlist::name() const
{
    return mName;
}



#ifdef DEBUG
inline std::ostream& operator<<(std::ostream& os, const QString& s)
{
    os << s.toStdString();
    return os;
}

std::ostream& operator<<(std::ostream& os, const Playlist* p)
{
    os << p->mName << ":" << std::endl;

    QList<Song::Ptr>::const_iterator it = p->begin(); // Iterator on the begining of the playlist

    // Iteration on the playlist to save each song in playlists_songs database
    for(; it != p->end(); ++it)
    {
        std::cout << "\t" << (*it) << std::endl;
    }

    return os;
}
#endif
