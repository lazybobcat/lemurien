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
    QSqlQuery query;
    query.exec("insert into songs ('id', 'title', 'artist', 'album', 'filepath', 'nbplay', 'mark', 'lyrics') values ('1', 'Attaba', 'M', 'En tête à tête disk 1', '/media/lo-x/SDATA1/Musique/M/En Tête A Tête [Disc 1]/1-03 Attaba.m4a', '0', '0', NULL)");
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
            mTitle = query.value(1).toString().toStdString();   // Extract the title
            mArtist = query.value(2).toString().toStdString();  // Extract the artist
            mAlbum = query.value(3).toString().toStdString();   // Extrat the album name
            mFilepath = query.value(4).toString().toStdString();// Extract te filepath
            mNbPlay = query.value(5).toUInt();                  // Extract the numer of times the song has been played
            mMark = query.value(6).toUInt();                    // Extract the mark
            mLyrics = query.value(7).toString().toStdString();  // Extact the lyrics
        }
        else
            throw SqlModel::DataNotFound; // Throw an error if one occured
    }
    else
        throw SqlModel::SQLError;
}





/*
std::ostream& operator<<(std::ostream& os, const Song* song)
{
    os << song->mTitle << " [" << song->mArtist << " : " << song->mAlbum << "] " << song->mFilepath << " {played:" << song->mNbPlay << "}";
    return os;
}
*/
