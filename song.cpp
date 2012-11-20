#include "song.h"
#include <QVariant>

Song::Song(QSqlDatabase* db)
    : SqlModel(db, "songs", "id"),
      mTitle(),
      mArtist(),
      mFilepath(),
      mNbPlay(0),
      mMark(0),
      mLyrics()
{
}


void Song::save() throw(SqlModel::Error)
{

    if(db()->isOpen())
    {
        // If the song hasn't been saved yet in the database, it needs an INSERT
        if(key().empty())
        {
            // Prepare the query and then bind values
            QSqlQuery query;
            query.prepare("INSERT INTO songs VALUES(NULL, :title, :artist, :album, :filepath, :nbplay, :mark, :lyrics)");
            // We've set the ID to null because AUTOINCREMENT is activated, so let SQLite do its job !
            query.bindValue(":title", QString(mTitle));
            query.bindValue(":artist", mArtist);
            query.bindValue(":album", mAlbum);
            query.bindValue(":filepath", mFilepath);
            query.bindValue(":nbplay", mNbPlay);
            query.bindValue(":mark", mMark);
            query.bindValue(":lyrics", mLyrics);
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
            query.prepare("UPDATE songs SET title=:title, artist=:artist, album=:album, filepath=:filepath, nbplay=:nbplay, mark=:mark, lyrics=:lyrics WHERE id=:id");
            query.bindValue(":id", boost::any_cast<unsigned int>(key()));
            query.bindValue(":title", QString(mTitle));
            query.bindValue(":artist", mArtist);
            query.bindValue(":album", mAlbum);
            query.bindValue(":filepath", mFilepath);
            query.bindValue(":nbplay", mNbPlay);
            query.bindValue(":mark", mMark);
            query.bindValue(":lyrics", mLyrics);
            // Execution
            if(!query.exec()) // In case of error
                throw SqlModel::InsertFailed;
        }
    }
    else
        throw SqlModel::SQLError;
}


void Song::construct() throw(SqlModel::Error)
{
    if(key().empty())
        throw SqlModel::LogicalError;

    if(db()->isOpen())
    {
        // Get the song in the database
        QSqlQuery query(QString("SELECT id, title, artist, album, filepath, nbplay, mark, lyrics FROM songs WHERE id=%1").arg(boost::any_cast<int>(key())));
        if(query.next()) // Get to the data
        {
            mTitle = query.value(1).toString();         // Extract the title
            mArtist = query.value(2).toString();        // Extract the artist
            mAlbum = query.value(3).toString();         // Extrat the album name
            mFilepath = query.value(4).toString();      // Extract te filepath
            mNbPlay = query.value(5).toUInt();          // Extract the numer of times the song has been played
            mMark = query.value(6).toUInt();             // Extract the mark
            mLyrics = query.value(7).toString();        // Extact the lyrics
        }
        else
            throw SqlModel::DataNotFound; // Throw an error if one occured
    }
    else
        throw SqlModel::SQLError;
}


void    Song::setTitle(const QString& title)
{
    if(!title.isEmpty())
        mTitle = title;
}

void    Song::setArtist(const QString &the_artist)
{
    if(!the_artist.isEmpty())
        mArtist = the_artist;
}

void    Song::setAlbum(const QString& album)
{
    if(!album.isEmpty())
        mAlbum = album;
}

void    Song::setFilepath(const QString& filepath)
{
    if(!filepath.isEmpty())
        mFilepath = filepath;
}

void    Song::setMark(unsigned int mark)
{
    if(mark <= Config::cMaxMark)
        mMark = mark;
}

void    Song::setLyrics(const QString& lyrics)
{
    if(!lyrics.isEmpty())
        mLyrics = lyrics;
}

void    Song::playOneMoreTime()
{
    ++mNbPlay;
}



#ifdef DEBUG
std::ostream& operator<<(std::ostream& os, const QString& s)
{
    os << s.toStdString();
    return os;
}

std::ostream& operator<<(std::ostream& os, const Song* song)
{
    os << "(" << boost::any_cast<unsigned int>(song->key()) << ") " << song->mTitle << " [" << song->mArtist << " : " << song->mAlbum << "] " << song->mFilepath << " {played:" << song->mNbPlay << "}";
    return os;
}
#endif
