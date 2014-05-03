#ifndef QMUSICPLAYER_H
#define QMUSICPLAYER_H

#include <QWidget>
#include "qsfmlmusic.h"
#include "qplaylist.h"

class QMusicPlayer : public QWidget
{
    Q_OBJECT
public:
    explicit QMusicPlayer(QWidget *parent = 0);

    void                setPlaylist(QPlaylist* playlist);
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
    void                sourceChanged(QSong new_source);

public slots:
    void                songAboutToFinish();
    void                songFinished();

private:
    QSfmlMusic          mMusic;
    QPlaylist*          mPlaylist;
    unsigned int        mPlaylistIndex;
    bool                mLoop;

};

#endif // QMUSICPLAYER_H
