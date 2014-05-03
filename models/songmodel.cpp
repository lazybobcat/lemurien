#include "songmodel.h"
#include <QFile>

SongModel::SongModel(QObject *parent) :
    QObject(parent),
    mNbPlay(0),
    mMark(0)
{
}


void SongModel::save()
{
    // todo
}

void SongModel::construct()
{
    // todo
}

void SongModel::erase()
{
    // todo
}

void SongModel::setTitle(const QString &title)
{
    mTitle = title;
    emit dataChanged(shared_from_this());
}

void SongModel::setArtist(const QString &artist)
{
    mArtist = artist;
    emit dataChanged(shared_from_this());
}

void SongModel::setAlbum(const QString &album)
{
    mAlbum = album;
    emit dataChanged(shared_from_this());
}

void SongModel::setFilepath(const QString &path)
{
    if(!path.isEmpty() && QFile(path).exists())
    {
        mFilepath = path;
        emit dataChanged(shared_from_this());
    }
}

void SongModel::setLyrics(const QString &lyrics)
{
    mLyrics = lyrics;
    emit dataChanged(shared_from_this());
}

void SongModel::setMark(unsigned int mark)
{
    if(mark <= Config::cMaxMark)
    {
        mMark = mark;
        emit dataChanged(shared_from_this());
    }
}

void SongModel::playOneMoreTime()
{
    ++mNbPlay;
    emit dataChanged(shared_from_this());
}



QString SongModel::title() const
{
    return mTitle;
}

QString SongModel::artist() const
{
    return mArtist;
}

QString SongModel::album() const
{
    return mAlbum;
}

QString SongModel::filepath() const
{
    return mFilepath;
}

QString SongModel::lyrics() const
{
    return mLyrics;
}

unsigned int SongModel::mark() const
{
    return mMark;
}

unsigned int SongModel::nbplay() const
{
    return mNbPlay;
}


#ifdef DEBUG
inline std::ostream& operator<<(std::ostream& os, const QString& s)
{
    os << s.toStdString();
    return os;
}

std::ostream& operator<<(std::ostream& os, const SongModel* song)
{
    os << song->mTitle << " [" << song->mArtist << " : " << song->mAlbum << "] " << song->mFilepath << " {played:" << song->mNbPlay << "}";
    return os;
}
#endif
