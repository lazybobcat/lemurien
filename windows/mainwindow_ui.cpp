#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    mWebkitWindow(NULL)
{
    setupPlaylists();
    setupActions();
    setupMenus();
    setupUi();
    setupTray();

    SongModel::Ptr model(new SongModel());
    model->setFilepath("/home/lo-x/Musique/testsample2.flac");
    QSong song(model->shared_from_this());
    QPlaylist* playlist = new QPlaylist(this);
    playlist->append(song);

    model.reset(new SongModel());
    model->setFilepath("/home/lo-x/Musique/testsample.flac");
    song.setModel(model);
    playlist->append(song);

    std::cout << "Playlist size : " << playlist->size() << std::endl;

    mMusicPlayer.setPlaylist(playlist);
    mMusicPlayer.play();
}

MainWindow::~MainWindow()
{
    // Delete Widgets


    // Delete actions


    // delete windows
    delete mWebkitWindow;
}


void MainWindow::setupPlaylists()
{

}

void MainWindow::setupActions()
{

}

void MainWindow::setupMenus()
{

}

void MainWindow::setupTray()
{

}

void MainWindow::setupUi()
{

}

void MainWindow::rebuildSidebar()
{

}
