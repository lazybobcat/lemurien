#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QtWidgets/QMainWindow>
#include <QMap>
#include <QVariant>
#include "config.h"
#include "../widgets/qmusicplayer.h"
#include "webkitwindow.h"


/**
 * @brief The Jukebox Main Window. Contains graphic items, other windows, audio components and playlists
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
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


    /////////////////////////////////
    //       Audio Properties      //
    /////////////////////////////////
    QMusicPlayer  mMusicPlayer;

    /////////////////////////////////
    //         UI Properties       //
    /////////////////////////////////
    // Widgets


    // Actions


    // System Tray


    // Other windows
    WebkitWindow*   mWebkitWindow;      ///< A pointer on an "wikipedia" window

    /////////////////////////////////
    //          Playlists          //
    /////////////////////////////////
    QMap<QString, QPlaylist*>   mPlaylists;        ///< A map contaning playlists by their name
    QPlaylist*                  mMusicLibrairy;    ///< Redundant (contained in the map) : the main playlist, music librairy, containing all songs

};

#endif // MAINWINDOW_H
