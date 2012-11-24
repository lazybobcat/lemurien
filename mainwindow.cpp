#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    mAudioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    mMediaObject = new Phonon::MediaObject(this);
    mMetaInformationResolver = new Phonon::MediaObject(this);



    Phonon::createPath(mMediaObject, mAudioOutput);

    setupActions();
    setupMenus();
    setupUi();

    mMediaObject->setTickInterval(1000);
    mMediaObject->setCurrentSource(QString("data/intro.mp3"));
    //mMediaObject->enqueue(QString("/home/lo-x/Musique/Ogres_de_Barback.mp3"));
    /*if(Config::cFadeChaining)
        mMediaObject->setTransitionTime(-Config::cFadeTime);*/
    mMediaObject->play();
}

MainWindow::~MainWindow()
{
    delete mAudioOutput;
    delete mMediaObject;
    delete mMetaInformationResolver;

    // Delete actions
    delete mPlayAction;
    delete mPauseAction;
    delete mStopAction;
    delete mNextAction;
    delete mPreviousAction;
    delete mShuffleAction;
    delete mRepeatAction;
    delete mRepeatOnceAction;
    delete mAddFilesAction;
    delete mExitAction;
    delete mAboutAction;
    delete mShrinkAction;
}

void MainWindow::setupActions()
{
    // Play
    mPlayAction = new QAction(style()->standardIcon(QStyle::SP_MediaPlay), tr("Play"), this);
    mPlayAction->setShortcut(tr("Space"));
    mPlayAction->setDisabled(true);

    // Pause
    mPauseAction = new QAction(style()->standardIcon(QStyle::SP_MediaPause), tr("Pause"), this);
    mPauseAction->setShortcut(tr("Space"));
    mPauseAction->setDisabled(true);

    // Stop
    mStopAction = new QAction(style()->standardIcon(QStyle::SP_MediaStop), tr("Stop"), this);
    mStopAction->setShortcut(tr("Ctrl+S"));
    mStopAction->setDisabled(true);

    // Next
    mNextAction = new QAction(style()->standardIcon(QStyle::SP_MediaSkipForward), tr("Suivant"), this);
    mNextAction->setShortcut(tr("Ctrl+N"));

    // Previous
    mPreviousAction = new QAction(style()->standardIcon(QStyle::SP_MediaSkipBackward), tr("Précédent"), this);
    mPreviousAction->setShortcut(tr("Ctrl+R"));

    // Shuffle mode
    mShuffleAction = new QAction(QIcon("data/shuffle.png"), tr("Choisir la chanson suivante aléatoirement"), this);
    mShuffleAction->setCheckable(true);

    // Repeat mode
    mRepeatAction = new QAction(QIcon("data/repeat.png"), tr("Répéter toute la sélection une fois arrivé à la fin"), this);
    mRepeatAction->setCheckable(true);

    // Repeat once mode
    mRepeatOnceAction = new QAction(QIcon("data/repeatOnce.png"), tr("Répéter la chanson actuelle"), this);
    mRepeatOnceAction->setCheckable(true);

    // Add files actin
    mAddFilesAction = new QAction(tr("Ajouter des fichiers audio"), this);
    mAddFilesAction->setShortcut(tr("Ctrl+F"));

    // Exit action
    mExitAction = new QAction(tr("Quitter"), this);
    mExitAction->setShortcut(tr("Ctrl+Q"));
    connect(mExitAction, SIGNAL(triggered()), this, SLOT(close()));

    // About action
    mAboutAction = new QAction(tr("A propos"), this);

    // Shrink action
    mShrinkAction = new QAction(tr("Miniaturiser le lecteur"), this);
    mShrinkAction->setCheckable(true);
    mShrinkAction->setShortcut(tr("Ctrl+D"));




}

void MainWindow::setupMenus()
{
    // Fixes the menbar IN the window, because of ***** unity (or mac) that put the menubar on the desktop toolbar...
    menuBar()->setNativeMenuBar(false);

    QMenu *playlistMenu = menuBar()->addMenu(tr("Playlist"));
        playlistMenu->addAction(mAddFilesAction);
        playlistMenu->addSeparator();

    QMenu *controlMenu = menuBar()->addMenu(tr("Contrôles"));
        controlMenu->addAction(mPlayAction);
        controlMenu->addAction(mPauseAction);
        controlMenu->addAction(mStopAction);
        controlMenu->addAction(mNextAction);
        controlMenu->addAction(mPreviousAction);
        controlMenu->addSeparator();
        controlMenu->addAction(mRepeatAction);
        controlMenu->addAction(mRepeatOnceAction);
        controlMenu->addAction(mShuffleAction);
        controlMenu->addSeparator();
        controlMenu->addAction(mExitAction);

    QMenu *affichageMenu = menuBar()->addMenu(tr("Affichage"));
        affichageMenu->addAction(mShrinkAction);
        affichageMenu->addSeparator();


    QMenu *aboutMenu = menuBar()->addMenu(tr("Aide"));
        /*aboutMenu->addAction(preferencesAction);
        aboutMenu->addSeparator();*/
        aboutMenu->addAction(mAboutAction);
}

void MainWindow::setupUi()
{
    // ***************************************************** //
    //                    Window Properties                  //
    // ***************************************************** //

    setWindowTitle("Lémurien");
    setWindowIcon(QIcon("data/lemurien.png"));
    resize(800, 700);
    setMinimumSize(100, 50);

    QWidget* mainWidget = new QWidget;
    QVBoxLayout* mainLayout = new QVBoxLayout;


    // ***************************************************** //
    //                        Sidebar                        //
    // ***************************************************** //

    QDockWidget* dock = new QDockWidget;
    dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
        mSidebar = new SideBar;
        /** @todo Insérer les vraies playlists avec des QAction dans la sidebar ici */
        QAction* allmusic = mSidebar->addAction("Toute la musique");
        mSidebar->addAction("Playlist 1");
        mSidebar->addAction("Mon Top");
        mSidebar->setMinimumWidth(150);
        mSidebar->setMaximumWidth(150);
    dock->setWidget(mSidebar);

    // Remove the dock widget title zone (tip from documentation) :
    QWidget *titleBarWidget = new QWidget(0);
    dock->setTitleBarWidget(titleBarWidget);




    // ***************************************************** //
    //                    Control Bar                        //
    // ***************************************************** //

    // Control Bar
    mControlBar = addToolBar(tr("Barre de contrôle"));
    mControlBar->addAction(mPreviousAction);
    mControlBar->addAction(mPlayAction);
    mControlBar->addAction(mPauseAction);
    mControlBar->addAction(mStopAction);
    mControlBar->addAction(mNextAction);
    mControlBar->addSeparator();
    mControlBar->addAction(mRepeatAction);
    mControlBar->addAction(mRepeatOnceAction);
    mControlBar->addAction(mShuffleAction);
    mControlBar->addSeparator();

    // Volume
    mVolumeSlider = new Phonon::VolumeSlider(this);
    mVolumeSlider->setWindowTitle(tr("Contrôle du volume"));
    mVolumeSlider->setAudioOutput(mAudioOutput);
    mVolumeSlider->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    // Seek Slider
    mSeekSlider = new Phonon::SeekSlider(this);
    mSeekSlider->setMediaObject(mMediaObject);

    // Labels
    mSongLabel = new QLabel(tr("<b>Pas de lecture en cours...</b>"));
    mElapsedTimeLabel = new QLabel("00:00");
    mRemainingTimeLabel = new QLabel("00:00");

    // Layout everything
    // Layout containing the control bar and volume slider
    QHBoxLayout* controlbarLayout = new QHBoxLayout;
        controlbarLayout->addWidget(mControlBar);
        controlbarLayout->addStretch();
        controlbarLayout->addWidget(mVolumeSlider);

    // Layout containing the labels
    QHBoxLayout* labelsLayout = new QHBoxLayout;
        labelsLayout->addWidget(mElapsedTimeLabel);
        labelsLayout->addStretch();
        labelsLayout->addWidget(mSongLabel);
        labelsLayout->addStretch();
        labelsLayout->addWidget(mRemainingTimeLabel);




    // ***************************************************** //
    //              Playlist Details Table                   //
    // ***************************************************** //

    QStringList headers;
    headers << tr("Titre") << tr("Artiste") << tr("Album") << tr("Note") << tr("Nombre Lectures");
    mMusicTable = new QTableWidget(0, 5);
    mMusicTable->setHorizontalHeaderLabels(headers);
    mMusicTable->setSelectionMode(QAbstractItemView::SingleSelection);
    mMusicTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    mMusicTable->setShowGrid(false);
    mMusicTable->resizeRowsToContents();
    mMusicTable->setContextMenuPolicy(Qt::CustomContextMenu);




    // ***************************************************** //
    //                      Window                           //
    // ***************************************************** //

    // Setting up the main layout
    // Global Layout for control section
    mainLayout->addLayout(controlbarLayout);
    mainLayout->addLayout(labelsLayout);
    mainLayout->addWidget(mSeekSlider);
    mainLayout->addWidget(mMusicTable);


    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
}
