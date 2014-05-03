#ifndef QSEEKSLIDER_H
#define QSEEKSLIDER_H

#include <QSlider>
#include <QMouseEvent>
#include <SFML/System/Time.hpp>
#include <SFML/Audio/Music.hpp>
#include "qsong.h"

class QMusicPlayer;

class QSeekSlider : public QSlider
{
    Q_OBJECT
public:
    explicit QSeekSlider(QMusicPlayer* player);

signals:

public slots:
    void        sourceChanged(QSong);
    void        statusChaged(sf::Music::Status status);
    void        tick(sf::Time progression);
    void        changeSongProgression(int value);

protected:
    virtual void mouseReleaseEvent(QMouseEvent *ev);

private:
    QMusicPlayer*   mPlayer;

};

#endif // QSEEKSLIDER_H
