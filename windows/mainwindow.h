#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QtWidgets/QMainWindow>
#include <QMap>
#include <QVariant>
#include "../interface/sidebar.h"
#include "config.h"
#include "QSfmlMusic/qsfmlmusic.h"
#include "../database/sqlmodelfactory.h"
#include "../models/playlisttablemodel.h"
#include "addfileswindow.h"
#include "editsongwindow.h"
#include "webkitwindow.h"


/**
 * @brief The Jukebox Main Window. Contains graphic items, other windows, audio components and playlists
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

    friend class AddfilesWindow;    ///< The AddFilesWindow need to access some of our private attributes
    
public:
    explicit MainWindow(QWidget *parent = 0);

    /**
     * @brief Destroys all graphic and audio components. Some destructions are redundant.
     */
    ~MainWindow();

signals:
    void needToShow();

public slots:
    void handleParams(const QStringList& params);
    void handleMessage(const QString& message);

    void playlistChanged(bool checked);
    void tick(qint64);
    void tick(sf::Time);
    //void stateChanged(Phonon::State,Phonon::State);
    void currentSourceChanged();
    void aboutToFinish();
    void next();
    void previous();
    void repeatOnce();
    void shuffle();
    void tableClicked(const QModelIndex &);
    void addFilesToLibrairy();
    void about();
    void shrink();

    void addPlaylist();
    void removePlaylist();
    void renamePlaylist();

    /*void sortOnTitle();
    void sortOnAlbum();
    void sortOnArtist();
    void sortOnMark();
    void sortOnNbPlay();*/

    void popContextMenuOnSong(const QPoint& point);
    void popTrayMessage(const QString &title, const QString &message, QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information);

    void search();
    
protected:
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *);

private:
    void setupActions();
    void setupMenus();
    void setupUi();
    void setupTray();
    void setupPlaylists();
    void rebuildSidebar();

    Song::Ptr importSong(const QString& filepath);

    /////////////////////////////////
    //       Audio Properties      //
    /////////////////////////////////
    //Phonon::AudioOutput* mAudioOutput;      ///< The Audio Output for Phonon, that we will link to our MediaObject
    //Phonon::MediaObject* mMediaObject;      ///< The media object (the song that we are playing + songs in waiting queue)
    //Phonon::MediaObject *mMetaInformationResolver;  ///< The meta information resolver extract informations from songs, such as duration, title, etc..
    QSfmlMusic  mMusicPlayer;

    /////////////////////////////////
    //         UI Properties       //
    /////////////////////////////////
    // Widgets
    //Phonon::SeekSlider  *mSeekSlider;       ///< The seek slider widget, that is used to show where we are in the playing
    //Phonon::VolumeSlider *mVolumeSlider;    ///< The volume widget to mute/unmute and set volume
    SideBar     *mSidebar;              ///< The window sidebar containing shortcut to playlists. This class has been writen by Benjamin Le Cam.
    QToolBar    *mControlBar;           ///< This pointer will refer to the window toolbar
    QTableView  *mMusicTable;           ///< The table containing the songs information for a given playlist
    PlaylistTableModel *mMusicModel;    ///< The model used by mMusicTable to display playlist informations
    QLabel      *mSongLabel;            ///< A label to display the song title
    QLabel      *mElapsedTimeLabel;     ///< A label to display the elapsed time (the time since the current playing song has started)
    QLabel      *mRemainingTimeLabel;   ///< A label to display the remaining time
    QLineEdit   *mSearchField;          ///< The field for searches
    QPushButton *mSearchButton;         ///< The "FETCH" button

    // Actions
    QAction     *mPlayAction;           ///< Play the current source song
    QAction     *mPauseAction;          ///< Pause the current source song
    QAction     *mStopAction;           ///< Stop the current source song
    QAction     *mNextAction;           ///< Play the next song (taking account of playing mode)
    QAction     *mPreviousAction;       ///< Play the previous song (taking account of playing mode)
    QAction     *mShuffleAction;        ///< Set the playing mode to shuffle
    QAction     *mRepeatOnceAction;     ///< Set the playing mode to repeat once
    QAction     *mAddFilesAction;       ///< Open the "add files" window
    QAction     *mExitAction;           ///< Close the window
    QAction     *mAboutAction;          ///< Open about window
    QAction     *mAddPlaylistAction;    ///< Open the "add playlist" window
    QAction     *mRemovePlaylistAction; ///< Remove the current playlist
    QAction     *mRenamePlaylistAction; ///< Rename the current playlist
    QAction     *mSortOnTitleAction;    ///< Sort the current playlist by title
    QAction     *mSortOnAlbumAction;    ///< Sort the current playlist by album
    QAction     *mSortOnArtistAction;   ///< Sort the current playlist by artist
    QAction     *mSortOnMarkAction;     ///< Sort the current playlist by mark
    QAction     *mSortOnNbPlayAction;   ///< Sort the current playlist by nb play
    QAction     *mShrinkAction;
    QAction     *mMaximizeAction;

    // System Tray
    QSystemTrayIcon*    mTrayIcon;
    QMenu*              mTrayMenu;

    // Other windows
    AddfilesWindow* mAddfilesWindow;    ///< A pointer on an "add files" window
    EditSongWindow* mEditSongWindow;    ///< A pointer on an "edit song" window
    WebkitWindow*   mWebkitWindow;      ///< A pointer on an "wikipedia" window

    /////////////////////////////////
    //          Playlists          //
    /////////////////////////////////
    QMap<QString, Playlist::Ptr> mPlaylists;        ///< A map contaning playlists by their name
    Playlist::Ptr                mMusicLibrairy;    ///< Redundant (contained in the map) : the main playlist, music librairy, containing all songs
    unsigned int                 mIndexOfSource;    ///< The index in playlist of the actual playing song
    Playlist::Ptr                mSource;           ///< Redundant : the actual playing playlist
    Playlist::Ptr                mSearchedPlaylist; ///< A special (temporary) playlist that contains the results of a search
    Playlist::Ptr                mArgumentsPlaylist;///< A special (temporary) playlist that contains songs given in application arguments

};

#endif // MAINWINDOW_H
