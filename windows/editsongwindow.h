#ifndef EDITSONGWINDOW_H
#define EDITSONGWINDOW_H

#include <QtGui>
#include <QtWidgets/QtWidgets>
#include "models/song.h"
#include "exceptions.h"

class MainWindow;

/**
 * @brief The EditSong Window allow the user to edit informations about a song like title, artist, album, filepath, lyrics, etc..
 */
class EditSongWindow : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief This consructor build the window design
     * @param song The song to edit in the window
     * @param parent The parent MainWindow
     */
    explicit EditSongWindow(Song::Ptr song, MainWindow *parent = 0);

    virtual ~EditSongWindow() { }

    
signals:
    
public slots:
    /**
     * @brief When the "ok" push button is pressed, the data are saved in database and the window is closed
     */
    void saveAndClose();

private:
    // Song
    Song::Ptr       mSong;          ///< The song to edit

    // Widgets
    QLineEdit*      mTitleField;    ///< The title field that contains the song title
    QLineEdit*      mAlbumField;    ///< The album field that contains the song album
    QLineEdit*      mArtistField;   ///< The artist field that contains the song artist
    QLineEdit*      mFilepathField; ///< The filepath field that contains the song filepath
    QComboBox*      mMarkField;     ///< The mark field that contains the song mark
    QTextEdit*      mLyricsField;   ///< The lyrics field that contains the song lyrics

    QPushButton*    mOkButton;      ///< When this button is pressed, the data are saved and the window is closed
    QPushButton*    mCancelButton;  ///< When this button is pressed, the window is closed without saving anything

    // Parent
    MainWindow*         mParent; ///< This is redundant but avoid a cast when we have to use it
    
};

#endif // EDITSONGWINDOW_H
