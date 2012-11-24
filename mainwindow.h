#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMainWindow>
#include <phonon/mediaobject.h>
#include <phonon/mediasource.h>
#include <phonon/audiooutput.h>
#include <phonon/mediasource.h>
#include <phonon/volumeslider.h>
#include <phonon/seekslider.h>
#include "sidebar.h"
#include "ui_mainwindow.h"
#include "config.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    void setupActions();
    void setupMenus();
    void setupUi();



    /////////////////////////////////
    //       Audio Properties      //
    /////////////////////////////////
    Phonon::AudioOutput* mAudioOutput;
    Phonon::MediaObject* mMediaObject;
    Phonon::MediaObject *mMetaInformationResolver;
    Phonon::SeekSlider  *mSeekSlider;
    Phonon::VolumeSlider *mVolumeSlider;


    /////////////////////////////////
    //         UI Properties       //
    /////////////////////////////////
    // Widgets
    SideBar     *mSidebar;
    QToolBar    *mControlBar;
    QTableWidget *mMusicTable;
    QLabel      *mSongLabel;
    QLabel      *mElapsedTimeLabel;
    QLabel      *mRemainingTimeLabel;

    // Actions
    QAction     *mPlayAction;
    QAction     *mPauseAction;
    QAction     *mStopAction;
    QAction     *mNextAction;
    QAction     *mPreviousAction;
    QAction     *mShuffleAction;
    QAction     *mRepeatAction;
    QAction     *mRepeatOnceAction;
    QAction     *mAddFilesAction;
    QAction     *mExitAction;
    QAction     *mAboutAction;
    QAction     *mShrinkAction;

};

#endif // MAINWINDOW_H
