#ifndef QMUSICPLAYER_H
#define QMUSICPLAYER_H

#include <QObject>
#include "qsfmlmusic.h"
#include "../models/playlist.h"

class QMusicPlayer : public QObject
{
    Q_OBJECT
public:
    explicit QMusicPlayer(QObject *parent = 0);

    void                setPlaylist(Playlist::Ptr playlist);
    void                play();
    void                pause();
    void                stop();
    void                previous();
    void                next();
    void                setVolume(int volume);

    sf::Music::Status   status() const;

signals:
    void                volumeChanged(int new_volume);
    void                statusChanged(sf::Music::Status new_status);
    void                sourceChanged(Song::Ptr new_source);

public slots:
    void                songAboutToFinish();
    void                songFinished();

private:
    QSfmlMusic          mMusic;
    Playlist::Ptr       mPlaylist;
    unsigned int        mPlaylistIndex;
    bool                mLoop;

};

#endif // QMUSICPLAYER_H