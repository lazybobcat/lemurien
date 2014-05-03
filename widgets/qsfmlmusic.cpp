#include "qsfmlmusic.h"

#include <iostream>

QSfmlMusic::QSfmlMusic(QWidget *parent) :
    QWidget(parent),
    mTimer(nullptr),
    mAboutToFinish(false)
{
    setVolume(100.f);
    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(tick()));
    mTimer->setInterval(1000);
    mTimer->setSingleShot(false);
    mTimer->stop();
}

void QSfmlMusic::tick()
{
    if(getStatus() != sf::Music::Stopped)
    {
        emit tick(getPlayingOffset());
        if((getDuration() - getPlayingOffset()) < sf::seconds(1.5) && !mAboutToFinish)
        {
            emit aboutToFinish();
            mAboutToFinish = true;
        }
    }
    else
    {
        emit finished();
        mTimer->stop();
    }
}

void QSfmlMusic::play()
{
    mTimer->start();
    sf::Music::play();
    mAboutToFinish = false;
}

void QSfmlMusic::stop()
{
    mTimer->stop();
    sf::Music::stop();
}
