#include "qsfmlmusic.h"

QSfmlMusic::QSfmlMusic(QWidget *parent) :
    QWidget(parent),
    mTimer(nullptr)
{
    setVolume(100.f);
    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(tick()));
    mTimer->setInterval(1000);
    mTimer->setSingleShot(false);
    //mTimer->start();
}

void QSfmlMusic::tick()
{
    if(getStatus() != sf::Music::Stopped)
    {
        emit tick(getPlayingOffset());
        if((getDuration() - getPlayingOffset()) < sf::seconds(1.5))
        {
            emit aboutToFinish();
        }
    }
}

void QSfmlMusic::play()
{
    mTimer->start();
    sf::Music::play();
}

void QSfmlMusic::stop()
{
    mTimer->stop();
    sf::Music::stop();
}
