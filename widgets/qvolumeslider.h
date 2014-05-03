#ifndef QVOLUMESLIDER_H
#define QVOLUMESLIDER_H

#include <QWidget>
#include <QIcon>
#include <QPushButton>
#include <QSlider>
#include <QHBoxLayout>

class QMusicPlayer;

class QVolumeSlider : public QWidget
{
    Q_OBJECT
public:
    explicit QVolumeSlider(QMusicPlayer* player);
    ~QVolumeSlider();

    void            setVolume(int v);

signals:
    void            volumeChanged(int);

public slots:
    void            sliderValueChanged(int v);
    void            mute();

private:
    QMusicPlayer*   mPlayer;
    QHBoxLayout*    mLayout;
    QPushButton*    mIcon;
    QSlider*        mSlider;
    bool            mMuted;
    int             mMutedValue;

};

#endif // QVOLUMESLIDER_H
