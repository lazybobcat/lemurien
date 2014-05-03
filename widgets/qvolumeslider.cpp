#include "qvolumeslider.h"
#include <QStyle>
#include "qmusicplayer.h"

QVolumeSlider::QVolumeSlider(QMusicPlayer *player) :
    QWidget(player),
    mPlayer(player),
    mLayout(nullptr),
    mIcon(nullptr),
    mSlider(nullptr),
    mMuted(false)
{
    mIcon = new QPushButton(style()->standardIcon(QStyle::SP_MediaVolume), "", this);
    connect(mIcon, SIGNAL(pressed()), this, SLOT(mute()));

    mSlider = new QSlider(this);
    mSlider->setTickInterval(1);
    mSlider->setMaximum(100);
    mSlider->setMinimum(0);
    connect(mSlider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));

    mLayout = new QHBoxLayout(this);
    mLayout->addWidget(mIcon);
    mLayout->addWidget(mSlider);
}


QVolumeSlider::~QVolumeSlider()
{
    delete mIcon;
    delete mSlider;
    delete mLayout;
}


void QVolumeSlider::setVolume(int v)
{
    mSlider->setValue(v);
}

void QVolumeSlider::sliderValueChanged(int v)
{
    if(v > 0)
    {
        mMuted = false;
    }

    mPlayer->setVolume(v);

    emit volumeChanged(v);
}


void QVolumeSlider::mute()
{
    if(!mMuted)
    {
        mMuted = true;
        mMutedValue = mSlider->value();
        mSlider->setEnabled(false);
        mIcon->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
        setVolume(0);
    }
    else
    {
        mMuted = false;
        mSlider->setEnabled(true);
        mIcon->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
        setVolume(mMutedValue);
    }
}


