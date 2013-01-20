#ifndef ADDFILESWINDOW_H
#define ADDFILESWINDOW_H

#include <QtGui>
#include <queue>
#include "sqlmodelfactory.h"
#include <phonon/mediaobject.h>

class MainWindow;

/**
 * @brief The Addfiles Window allow to import a folder containing audio files into the main playlist (= MusicLibrairy)
 */
class AddfilesWindow : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief The constructor, set the design of the window
     */
    explicit AddfilesWindow(MainWindow *parent = 0);


public slots:
    /**
     * @brief This slot opens a dialog window that allow the user to select a directory.
     */
    void choseDirectory();

    /**
     * @brief Everything that is needed for the import method is prepared here : the song filtering and song queueing
     */
    void startImporting();

    void prepareQueue(const QDir& directory);

private:
    /**
     * @brief While the song filepath queue isn't empty : extract informations from the file, construct the song and save it in database, add the song in the Music Librairy
     */
    void import();


    // Widgets
    QLineEdit*      mDirectoryField;        ///< The directory field
    QCheckBox*      mRecursiveField;        ///< If the inclusion of music files should be directory recursive

    QPushButton*    mChoseDirectoryButton;  ///< The push button that will open the chose directory window
    QPushButton*    mOkButton;              ///< The push button to start importing
    QPushButton*    mCancelButton;          ///< Close the window without importing

    QProgressBar*   mProgressBar;           ///< The progress bar of importation

    QLabel*         mDirectoryLabel;        ///< A QLabel to b more user-friendly

    // Process queue
    std::queue<QString>  mQueue;            ///< A queue containing the filepath to songs to import
    int                  mNbFilepaths;      ///< Derived attribute handy in the calculation needed for the progress bar

    // Parent
    MainWindow*         mParent; ///< This is redundant but avoid a cast when we have to use it
};

#endif // ADDFILESWINDOW_H
