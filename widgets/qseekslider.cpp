#include "qseekslider.h"
#include "qmusicplayer.h"

QSeekSlider::QSeekSlider(QMusicPlayer *player) :
    QSlider(player),
    mPlayer(player)
{
    setOrientation(Qt::Horizontal);
    setTracking(false);

    connect(mPlayer, SIGNAL(sourceChanged(QSong)), this, SLOT(sourceChanged(QSong)));
    connect(mPlayer, SIGNAL(tick(sf::Time)), this, SLOT(tick(sf::Time)));
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(changeSongProgression(int)));
}


void QSeekSlider::sourceChanged(QSong)
{
    setValue(0);
    setMaximum(mPlayer->songDuration().asMilliseconds());
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

void QSeekSlider::mousePressEvent(QMouseEvent *ev)
{
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
        ev->accept();
    }
    QSlider::mousePressEvent(ev);
}
