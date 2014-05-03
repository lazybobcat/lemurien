#ifndef QSEEKSLIDER_H
#define QSEEKSLIDER_H

#include <QSlider>
#include <QMouseEvent>
#include <SFML/System/Time.hpp>
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
    void        tick(sf::Time progression);
    void        changeSongProgression(int value);

protected:
    virtual void mousePressEvent(QMouseEvent *ev);

private:
    QMusicPlayer*   mPlayer;

};

#endif // QSEEKSLIDER_H
