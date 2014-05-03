#include "qmusicplayer.h"

#include <iostream>

QMusicPlayer::QMusicPlayer(QWidget *parent) :
    QWidget(parent),
    mPlaylist(nullptr),
    mPlaylistIndex(0),
    mPlaymode(QMusicPlayer::Normal)
{
    connect(&mMusic, SIGNAL(aboutToFinish()), this, SLOT(songAboutToFinish()));
    connect(&mMusic, SIGNAL(finished()), this, SLOT(songFinished()));
    connect(&mMusic, SIGNAL(tick(sf::Time)), this, SLOT(songTick(sf::Time)));
}

void QMusicPlayer::setPlaylist(QPlaylist *playlist)
{
    if(mPlaylist)
    {
        mMusic.stop();
        mPlaylistIndex = 0;
    }

    mPlaylist = playlist;
}

void QMusicPlayer::play()
{
    if(mPlaylist && !mMusic.getStatus() != sf::Music::Playing)
    {
        // If something was already playing
        if(mMusic.getPlayingOffset() > sf::Time::Zero)
        {
            mMusic.play();

            emit statusChanged(sf::Music::Playing);
        }
        else
        {
            if(mPlaylist->find(mPlaylistIndex) != mPlaylist->end())
            {
                QSong song = (*mPlaylist)[mPlaylistIndex];
                mMusic.openFromFile(song.model()->filepath().toStdString());
                mMusic.play();

                std::cout << "Start playing : " << song.model()->filepath().toStdString() << std::endl;

                emit sourceChanged(song);
                emit statusChanged(sf::Music::Playing);
            }
        }
    }
}

void QMusicPlayer::pause()
{
    mMusic.pause();

    emit statusChanged(sf::Music::Paused);
}

void QMusicPlayer::stop()
{
    mMusic.stop();
    mPlaylistIndex = 0;

    emit statusChanged(sf::Music::Stopped);
}

void QMusicPlayer::previous()
{
    mMusic.stop();

    switch(mPlaymode)
    {
        case Normal:
            if(mPlaylist && mPlaylistIndex > 0)
            {
                mPlaylistIndex -= 1;
                play();
            }
            break;

        case Loop:
            if(mPlaylist && mPlaylistIndex > 0)
            {
                mPlaylistIndex -= 1;
                play();
            }
            else if(mPlaylist && mPlaylistIndex == 0)
            {
                mPlaylistIndex = mPlaylist->size() - 1;
                play();
            }
            break;

        case LoopSingle:
            play();
            break;
    }
}

void QMusicPlayer::next()
{
    mMusic.stop();

    switch(mPlaymode)
    {
        case Normal:
            if(mPlaylist && mPlaylistIndex < (mPlaylist->size()-1u))
            {
                mPlaylistIndex += 1;
                play();
                std::cout << "next() -> normal mode will play index " << mPlaylistIndex << std::endl;
            }
            break;

        case Loop:
            if(mPlaylist && mPlaylistIndex < (mPlaylist->size()-1u))
            {
                mPlaylistIndex += 1;
                play();
                std::cout << "next() -> loop mode 1 will play index " << mPlaylistIndex << std::endl;
            }
            else if(mPlaylist)
            {
                mPlaylistIndex = 0;
                play();
                std::cout << "next() -> loop mode 2 will play index " << mPlaylistIndex << std::endl;
            }
            break;

        case LoopSingle:
            play();
            break;
    }
}

void QMusicPlayer::setVolume(int volume)
{
    if(volume >= 0 && volume <= 100)
    {
        mMusic.setVolume((float)volume);

        emit volumeChanged(volume);
    }
}


void QMusicPlayer::setPlaymode(PlayMode mode)
{
    if(mPlaymode != mode)
    {
        mPlaymode = mode;
        emit playmodeChanged(mPlaymode);
    }
}

QMusicPlayer::PlayMode QMusicPlayer::playmode() const
{
    return mPlaymode;
}

sf::Time QMusicPlayer::songDuration() const
{
    return mMusic.getDuration();
}

sf::Time QMusicPlayer::songProgression() const
{
    return mMusic.getPlayingOffset();
}

void QMusicPlayer::setSongProgression(sf::Time progression)
{
    mMusic.setPlayingOffset(progression);
}


void QMusicPlayer::songAboutToFinish()
{

}

void QMusicPlayer::songFinished()
{
    std::cout << "songFinished() " << mPlaylistIndex << std::endl;
    next();
}


void QMusicPlayer::songTick(sf::Time progression)
{
    emit tick(progression);
}
