#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    mAddfilesWindow(NULL),
    mEditSongWindow(NULL),
    mWebkitWindow(NULL),
    mIndexOfSource(-1)
{

    /*mAudioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    mMediaObject = new Phonon::MediaObject(this);
    mMetaInformationResolver = new Phonon::MediaObject(this);
    Phonon::createPath(mMediaObject, mAudioOutput);

    mMediaObject->setTickInterval(1000);
    mMediaObject->setCurrentSource(Config::cIntroSong);
    mMediaObject->play();
    mMetaInformationResolver->setCurrentSource(Config::cIntroSong);*/

    mMusicPlayer.openFromFile("/home/lo-x/Musique/testsample.flac");
    connect(&mMusicPlayer, SIGNAL(tick(sf::Time)), this, SLOT(tick(sf::Time)));

    mMusicPlayer.play();


    setupPlaylists();
    setupActions();
    setupMenus();
    setupUi();
    setupTray();

    /*connect(mMediaObject, SIGNAL(tick(qint64)), this, SLOT(tick(qint64)));
    connect(mMediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)), this, SLOT(stateChanged(Phonon::State,Phonon::State)));
    connect(mMediaObject, SIGNAL(aboutToFinish()), this, SLOT(aboutToFinish()));
    connect(mMediaObject, SIGNAL(currentSourceChanged(Phonon::MediaSource)), this, SLOT(currentSourceChanged()));*/
}

MainWindow::~MainWindow()
{
    /*delete mAudioOutput;
    delete mMediaObject;
    delete mMetaInformationResolver;
    delete mSeekSlider;
    delete mVolumeSlider;*/

    // Delete Widgets
    delete mSidebar;
    delete mMusicTable;
    delete mMusicModel;
    delete mSongLabel;
    delete mElapsedTimeLabel;
    delete mRemainingTimeLabel;
    delete mSearchField;
    delete mSearchButton;

    // Delete actions
    delete mPlayAction;
    delete mPauseAction;
    delete mStopAction;
    delete mNextAction;
    delete mPreviousAction;
    delete mShuffleAction;
    delete mRepeatOnceAction;
    delete mAddFilesAction;
    delete mExitAction;
    delete mAboutAction;
    delete mAddPlaylistAction;
    delete mRemovePlaylistAction;
    delete mRenamePlaylistAction;
    delete mSortOnTitleAction;
    delete mSortOnAlbumAction;
    delete mSortOnArtistAction;
    delete mSortOnMarkAction;
    delete mSortOnNbPlayAction;
    delete mShrinkAction;
    delete mMaximizeAction;

    // delete windows
    delete mAddfilesWindow;
    delete mEditSongWindow;
    delete mWebkitWindow;
}


void MainWindow::setupPlaylists()
{
    // ***************************************************** //
    //             Special Playlist : all music              //
    // ***************************************************** //

    mMusicLibrairy = SqlModelFactory::instance()->createPlaylist();
    mMusicLibrairy->setPrimaryKey(1);   // Set id to 1 because the fisrt playlist is the global one (cf: DatabaseManager table creation)
    mMusicLibrairy->construct();        // Construct the main playlist (add songs and playlist name)
    mPlaylists.insert(mMusicLibrairy->name(), mMusicLibrairy);


    // ***************************************************** //
    //                 Creation of playlists                 //
    // ***************************************************** //

    if(SqlModelFactory::instance()->dbManager()->db()->isOpen())
    {
        QSqlQuery query(QString("SELECT id FROM playlists WHERE id!=1"));
        while(query.next())
        {
            Playlist::Ptr playlist = SqlModelFactory::instance()->createPlaylist();
            playlist->setPrimaryKey(query.value(0).toUInt());
            playlist->construct();
            mPlaylists.insert(playlist->name(), playlist);
        }
    }

}

void MainWindow::setupActions()
{
    // Play
    mPlayAction = new QAction(style()->standardIcon(QStyle::SP_MediaPlay), tr("Play"), this);
    mPlayAction->setShortcut(tr("Space"));
    mPlayAction->setDisabled(true);
    //connect(mPlayAction, SIGNAL(triggered()), mMediaObject, SLOT(play()));

    // Pause
    mPauseAction = new QAction(style()->standardIcon(QStyle::SP_MediaPause), tr("Pause"), this);
    mPauseAction->setShortcut(tr("Space"));
    mPauseAction->setDisabled(true);
    //connect(mPauseAction, SIGNAL(triggered()), mMediaObject, SLOT(pause()));

    // Stop
    mStopAction = new QAction(style()->standardIcon(QStyle::SP_MediaStop), tr("Stop"), this);
    mStopAction->setShortcut(tr("Ctrl+S"));
    mStopAction->setDisabled(true);
    //connect(mStopAction, SIGNAL(triggered()), mMediaObject, SLOT(stop()));

    // Next
    mNextAction = new QAction(style()->standardIcon(QStyle::SP_MediaSkipForward), tr("Suivant"), this);
    mNextAction->setShortcut(tr("Ctrl+N"));
    connect(mNextAction, SIGNAL(triggered()), this, SLOT(next()));

    // Previous
    mPreviousAction = new QAction(style()->standardIcon(QStyle::SP_MediaSkipBackward), tr("Précédent"), this);
    mPreviousAction->setShortcut(tr("Ctrl+P"));
    connect(mPreviousAction, SIGNAL(triggered()), this, SLOT(previous()));

    // Shuffle mode
    mShuffleAction = new QAction(QIcon(":/data/shuffle.png"), tr("Choisir la chanson suivante aléatoirement"), this);
    mShuffleAction->setCheckable(true);
    connect(mShuffleAction, SIGNAL(triggered()), this, SLOT(shuffle()));

    // Repeat once mode
    mRepeatOnceAction = new QAction(QIcon(":/data/repeatOnce.png"), tr("Répéter la chanson actuelle"), this);
    mRepeatOnceAction->setCheckable(true);
    connect(mRepeatOnceAction, SIGNAL(triggered()), this, SLOT(repeatOnce()));

    // Add files action
    mAddFilesAction = new QAction(QIcon(":/data/music.png"), tr("Ajouter des fichiers audio"), this);
    mAddFilesAction->setShortcut(tr("Ctrl+F"));
    connect(mAddFilesAction, SIGNAL(triggered()), this, SLOT(addFilesToLibrairy()));

    // Exit action
    mExitAction = new QAction(QIcon(":/data/cancel.png"), tr("Quitter"), this);
    mExitAction->setShortcut(tr("Ctrl+Q"));
    connect(mExitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    // About action
    mAboutAction = new QAction(QIcon(":/data/idea.png"), tr("A propos"), this);
    connect(mAboutAction, SIGNAL(triggered()), this, SLOT(about()));

    // Shrink Action
    mShrinkAction = new QAction(tr("Rétrécir"), this);
    mShrinkAction->setShortcut(tr("Ctrl+D"));
    mShrinkAction->setCheckable(true);
    connect(mShrinkAction, SIGNAL(triggered()), this, SLOT(shrink()));



    // Add Playlist Action
    mAddPlaylistAction = new QAction(QIcon(":/data/playlist.png"), tr("Ajouter une playlist"), this);
    connect(mAddPlaylistAction, SIGNAL(triggered()), this, SLOT(addPlaylist()));

    // Remove playlist action
    mRemovePlaylistAction = new QAction(QIcon(":/data/trash.png"), tr("Suprimmer la playlist actuelle"), this);
    connect(mRemovePlaylistAction, SIGNAL(triggered()), this, SLOT(removePlaylist()));

    // Remove playlist action
    mRenamePlaylistAction = new QAction(QIcon(":/data/label.png"), tr("Renommer la playlist actuelle"), this);
    connect(mRenamePlaylistAction, SIGNAL(triggered()), this, SLOT(renamePlaylist()));

    mSortOnTitleAction = new QAction(tr("Trier par titre"), this);
    //connect(mSortOnTitleAction, SIGNAL(triggered()), this, SLOT(sortOnTitle()));
    mSortOnAlbumAction = new QAction(tr("Trier par album"), this);
    //connect(mSortOnAlbumAction, SIGNAL(triggered()), this, SLOT(sortOnAlbum()));
    mSortOnArtistAction = new QAction(tr("Trier par artiste"), this);
    //connect(mSortOnArtistAction, SIGNAL(triggered()), this, SLOT(sortOnArtist()));
    mSortOnMarkAction = new QAction(tr("Trier par note"), this);
    //connect(mSortOnMarkAction, SIGNAL(triggered()), this, SLOT(sortOnMark()));
    mSortOnNbPlayAction = new QAction(tr("Trier par nb de lectures"), this);
    //connect(mSortOnNbPlayAction, SIGNAL(triggered()), this, SLOT(sortOnNbPlay()));

    // System Tray specific actions
    mMaximizeAction = new QAction(tr("Agrandir"), this);
    connect(mMaximizeAction, SIGNAL(triggered()), this, SLOT(show()));
    mMaximizeAction->setVisible(false);
}

void MainWindow::setupMenus()
{
    // Fixes the menbar IN the window, because of ***** unity (or mac) that put the menubar on the desktop toolbar...
    menuBar()->setNativeMenuBar(false);

    QMenu *musicMenu = menuBar()->addMenu(tr("Musique"));
        musicMenu->addAction(mAddFilesAction);
        musicMenu->addSeparator();
        musicMenu->addAction(mExitAction);

    QMenu *playlistMenu = menuBar()->addMenu(tr("Playlist"));
        playlistMenu->addAction(mAddPlaylistAction);
        playlistMenu->addAction(mRemovePlaylistAction);
        playlistMenu->addAction(mRenamePlaylistAction);
        playlistMenu->addSeparator();
        QMenu* sortMenu = new QMenu;
            sortMenu->addAction(mSortOnTitleAction);
            sortMenu->addAction(mSortOnAlbumAction);
            sortMenu->addAction(mSortOnArtistAction);
            sortMenu->addAction(mSortOnMarkAction);
            sortMenu->addAction(mSortOnNbPlayAction);
            sortMenu->setTitle(tr("Trier la playlist"));
        playlistMenu->addMenu(sortMenu);


    QMenu *controlMenu = menuBar()->addMenu(tr("Contrôles"));
        controlMenu->addAction(mPlayAction);
        controlMenu->addAction(mPauseAction);
        controlMenu->addAction(mStopAction);
        controlMenu->addAction(mNextAction);
        controlMenu->addAction(mPreviousAction);
        controlMenu->addSeparator();
        controlMenu->addAction(mRepeatOnceAction);
        controlMenu->addAction(mShuffleAction);


    QMenu *affichageMenu = menuBar()->addMenu(tr("Affichage"));
        affichageMenu->addAction(mShrinkAction);


    QMenu *aboutMenu = menuBar()->addMenu(tr("Aide"));
        /*aboutMenu->addAction(preferencesAction);
        aboutMenu->addSeparator();*/
        aboutMenu->addAction(mAboutAction);
}

void MainWindow::setupTray()
{
    mTrayMenu = new QMenu(this);
    if(!QSystemTrayIcon::isSystemTrayAvailable())
    {
        std::cerr << "System Tray unvailable on this system !" << std::endl;
        return;
    }

    // Setup Tray Menu with actions
    mTrayMenu->addAction(mMaximizeAction);
    mTrayMenu->addSeparator();
    mTrayMenu->addAction(mPreviousAction);
    mTrayMenu->addAction(mNextAction);
    mTrayMenu->addAction(mPlayAction);
    mTrayMenu->addAction(mPauseAction);
    mTrayMenu->addAction(mStopAction);
    mTrayMenu->addSeparator();
    mTrayMenu->addAction(mRepeatOnceAction);
    mTrayMenu->addAction(mShuffleAction);
    mTrayMenu->addSeparator();
    mTrayMenu->addAction(mExitAction);

    // Setup tray icon and tooltip
    mTrayIcon = new QSystemTrayIcon(this);
    mTrayIcon->setIcon(QIcon(":/data/lemurien.svg"));
    mTrayIcon->setContextMenu(mTrayMenu);
    mTrayIcon->setToolTip(tr("Lémurien - Jukebox"));
    mTrayIcon->show();
}

void MainWindow::setupUi()
{
    // ***************************************************** //
    //                    Window Properties                  //
    // ***************************************************** //

    setWindowTitle("Lémurien");
    setWindowIcon(QIcon(":/data/lemurien.svg"));
    resize(960, 600);
    //setMinimumSize(600, 300);

    QWidget* mainWidget = new QWidget;
    QVBoxLayout* mainLayout = new QVBoxLayout;


    // ***************************************************** //
    //                        Sidebar                        //
    // ***************************************************** //

    QDockWidget* dock = new QDockWidget;
    dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
        mSidebar = new SideBar;
        mSidebar->setMinimumWidth(150);
        mSidebar->setMaximumWidth(150);
    dock->setWidget(mSidebar);

    // Remove the dock widget title zone (tip from documentation) :
    QWidget *titleBarWidget = new QWidget(0);
    dock->setTitleBarWidget(titleBarWidget);




    // ***************************************************** //
    //                    Control Bar                        //
    // ***************************************************** //
    mSearchField = new QLineEdit;
    mSearchField->setPlaceholderText("Rechercher...");
    mSearchButton = new QPushButton(style()->standardIcon(QStyle::SP_ArrowRight), tr(""));
    connect(mSearchButton, SIGNAL(released()), this, SLOT(search()));
    connect(mSearchField, SIGNAL(editingFinished()), this, SLOT(search()));

    // Control Bar
    mControlBar = addToolBar(tr("Barre de contrôle"));
    mControlBar->addAction(mPreviousAction);
    mControlBar->addAction(mPlayAction);
    mControlBar->addAction(mPauseAction);
    mControlBar->addAction(mStopAction);
    mControlBar->addAction(mNextAction);
    mControlBar->addSeparator();
    mControlBar->addAction(mRepeatOnceAction);
    mControlBar->addAction(mShuffleAction);
    mControlBar->addSeparator();
    mControlBar->addWidget(mSearchField);
    mControlBar->addWidget(mSearchButton);
    mControlBar->addSeparator();

    // Volume
    /*mVolumeSlider = new Phonon::VolumeSlider(this);
    mVolumeSlider->setWindowTitle(tr("Contrôle du volume"));
    mVolumeSlider->setAudioOutput(mAudioOutput);
    mVolumeSlider->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    // Seek Slider
    mSeekSlider = new Phonon::SeekSlider(this);
    mSeekSlider->setMediaObject(mMediaObject);*/

    // Labels
    mSongLabel = new QLabel(tr("<b>Pas de lecture en cours...</b>"));
    mElapsedTimeLabel = new QLabel("00:00");
    mRemainingTimeLabel = new QLabel("00:00");

    // Layout everything
    // Layout containing the control bar and volume slider
    QHBoxLayout* controlbarLayout = new QHBoxLayout;
        controlbarLayout->addWidget(mControlBar);
        controlbarLayout->addStretch();
        //controlbarLayout->addWidget(mVolumeSlider);

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

    // Model
    mMusicModel = new PlaylistTableModel(this);
    //mMusicModel->setPlaylist(mMusicLibrairy);

    // View
    QStringList headers;
    headers << tr("Titre") << tr("Artiste") << tr("Album") << tr("Note") << tr("Nombre Lectures");
    mMusicTable = new QTableView;
    //mMusicTable->setHorizontalHeaderLabels(headers);
    mMusicTable->setSelectionMode(QAbstractItemView::SingleSelection);
    mMusicTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    mMusicTable->setShowGrid(false);
    mMusicTable->setContextMenuPolicy(Qt::CustomContextMenu);
    mMusicTable->setModel(mMusicModel);
    //mMusicTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    //mMusicTable->horizontalHeader()->setResizeMode(3, QHeaderView::Fixed);
    //mMusicTable->horizontalHeader()->setResizeMode(4, QHeaderView::Fixed);
    mMusicTable->horizontalHeader()->resizeSection(3, 80);
    mMusicTable->horizontalHeader()->resizeSection(4, 80);
    connect(mMusicTable, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(tableClicked(QModelIndex)));
    connect(mMusicTable, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(popContextMenuOnSong(QPoint)));



    // ***************************************************** //
    //                      Window                           //
    // ***************************************************** //
    mAddfilesWindow = NULL;

    // Setting up the main layout
    // Global Layout for control section
    mainLayout->addLayout(controlbarLayout);
    mainLayout->addLayout(labelsLayout);
    //mainLayout->addWidget(mSeekSlider);
    mainLayout->addWidget(mMusicTable);


    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);



    // ***************************************************** //
    //          Add Playlists in Sidebar                     //
    // ***************************************************** //

    rebuildSidebar();
}

void MainWindow::rebuildSidebar()
{
    mSidebar->clear();

    QMap<QString, Playlist::Ptr>::const_iterator it = mPlaylists.begin();
    for(; it != mPlaylists.end(); ++it)
    {
        QAction* playlistAction;
        if((*it)->name() == mMusicLibrairy->name())
            playlistAction = mSidebar->addAction((*it)->name(), QIcon(":/data/music.png"));
        else if((*it)->name() == tr("Recherche"))
            playlistAction = mSidebar->addAction((*it)->name(), QIcon(":/data/zoom.png"));
        else
            playlistAction = mSidebar->addAction((*it)->name(), QIcon(":/data/playlist.png"));

        playlistAction->setData((*it)->name());

        connect(playlistAction, SIGNAL(toggled(bool)),
                this,           SLOT(playlistChanged(bool)));
    }
}
