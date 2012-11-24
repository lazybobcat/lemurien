
/**
 * @mainpage Lemurien - Jukebox
 * @section Description
 *  Lemurien is a C++ project of jukebox for my second year in computing school. It allows you to play your favorite playlists in a simple and efficient way !
 *
 * @section Author
 *  Loïc Boutter - main developer (<a href="http://loicboutter.fr/">website</a>)
 *
 * @section Dependences
 *  This jukebox needs Qt4.6+ (with phonon), libsqlite 3+ to work.
 *
 * @section Implemented features
 *  - DB Handler
 *
 * @section NYI Features
 *  - Main actions (play, pause, stop, next, previous, play mode, sliders, volume, ...)
 *  - Playlist handling
 *  - Shrink mode
 *  - Splash and super jungle screen
 *  - UI
 *  - Import audio files/folders
 *  - Options and preferences
 */





#include <QApplication>
#include "mainwindow.h"
#include "sqlmodelfactory.h"
#include "playlist.h"

#ifdef DEBUG
#include <iostream>
#endif

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QApplication app(argc, argv);
    app.setApplicationName("Lémurien");
    app.setApplicationVersion("0.1");




    MainWindow w;
    w.show();

    return app.exec();
}
