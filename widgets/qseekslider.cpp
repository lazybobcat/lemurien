#include "qseekslider.h"
#include "qmusicplayer.h"

#include <iostream>

QSeekSlider::QSeekSlider(QMusicPlayer *player) :
    QSlider(player),
    mPlayer(player)
{
    setOrientation(Qt::Horizontal);
    setTracking(false);

    connect(mPlayer, SIGNAL(sourceChanged(QSong)), this, SLOT(sourceChanged(QSong)));
    connect(mPlayer, SIGNAL(tick(sf::Time)), this, SLOT(tick(sf::Time)));
    connect(mPlayer, SIGNAL(statusChanged(sf::Music::Status)), this, SLOT(statusChaged(sf::Music::Status)));
}


void QSeekSlider::sourceChanged(QSong)
{
    setValue(0);
    setMaximum(mPlayer->songDuration().asMilliseconds());
}

void QSeekSlider::statusChaged(sf::SoundSource::Status status)
{
    switch(status)
    {
        case sf::Music::Stopped:
            setValue(0);
            break;

        default:
            break;
    }
}

void QSeekSlider::tick(sf::Time progression)
{
    if(!isSliderDown())
    {
        setValue(progression.asMilliseconds());
    }
}

void QSeekSlider::changeSongProgression(int value)
{
    sf::Time p = sf::milliseconds(value);
    mPlayer->setSongProgression(p);
}

void QSeekSlider::mouseReleaseEvent(QMouseEvent *ev)
{
    if(mPlayer->status() != sf::Music::Playing)
        return;

    if(ev->button() == Qt::LeftButton)
    {
        int value = 0;
        if (orientation() == Qt::Vertical)
        {
            value = minimum() + ((maximum()-minimum()) * (height()-ev->y())) / height();
            setValue(value) ;
            changeSongProgression(value);
        }
        else
        {
            value = minimum() + ((maximum()-minimum()) * ev->x()) / width();
            setValue(value);
            changeSongProgression(value);
        }

        setSliderDown(false);
        ev->accept();
    }
    QSlider::mousePressEvent(ev);
}
