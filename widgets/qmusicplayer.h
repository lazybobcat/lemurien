#ifndef QMUSICPLAYER_H
#define QMUSICPLAYER_H

#include <QWidget>
#include "qsfmlmusic.h"
#include "qplaylist.h"

class QMusicPlayer : public QWidget
{
    Q_OBJECT

public:
    enum PlayMode
    {
        Normal,
        Loop,
        LoopSingle,
        Random
    };

public:
    explicit QMusicPlayer(QWidget *parent = 0);

    void                setPlaylist(QPlaylist* playlist);
    void                setPlaymode(PlayMode mode);
    PlayMode            playmode() const;
    sf::Time            songDuration() const;
    sf::Time            songProgression() const;
    bool                hasNext() const;
    bool                hasPrevious() const;

    sf::Music::Status   status() const;

signals:
    void                volumeChanged(int new_volume);
    void                statusChanged(sf::Music::Status new_status);
    void                sourceChanged(QSong new_source);
    void                playmodeChanged(PlayMode);
    void                tick(sf::Time progression);

public slots:
    void                play();
    void                pause();
    void                stop();
    void                previous();
    void                next();
    void                setVolume(int volume);

    void                songAboutToFinish();
    void                songFinished();
    void                setSongProgression(sf::Time progression);
    void                songTick(sf::Time progression);

private:
    QSfmlMusic          mMusic;
    QPlaylist*          mPlaylist;
    unsigned int        mPlaylistIndex;
    PlayMode            mPlaymode;

};

#endif // QMUSICPLAYER_H
