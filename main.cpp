
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





#include <QtSingleApplication/QtSingleApplication>
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
    QtSingleApplication app("Lemurien", argc, argv);

    // We check if the app is already launched
    if(app.isRunning())
    {
        std::cout << "Lémurien is already running !" << std::endl;
        QString message;
        for (int a = 0; a < argc; ++a)
        {
            message += argv[a];
            if (a < argc-1)
                message += " ";
        }
        app.sendMessage(message);
        return 0;
    }

    // If not, we set main application datas
    app.setApplicationName("Lémurien");
    app.setApplicationVersion(QString("%1.%2").arg(Config::cMajorVersion).arg(Config::cMinorVersion));
    app.setOrganizationDomain("http://loicboutter.fr/");
    app.setOrganizationName("Boutter Loïc");

    // Creating main window
    MainWindow w;
    w.handleParams(app.arguments());
    w.show();

    // Connecting MainWindow instance to QtSingleApplication
    // This will allow us to get messages if a new instance of the application try to be launched. We will give us the params to
    // read a song if one were given as parameter
    QObject::connect(&app, SIGNAL(messageReceived(const QString&)), &w, SLOT(handleMessage(const QString&)));
    // Sets the active window and ask to be shown when an message is recieved
    app.setActivationWindow(&w, true);
    QObject::connect(&w, SIGNAL(needToShow()), &app, SLOT(activateWindow()));

    return app.exec();
}
