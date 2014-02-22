
/**
 * @mainpage Lemurien - Jukebox
 * @section Description
 *  Lemurien is a C++ project of jukebox. It allows you to play your favorite playlists in a simple and efficient way !
 *
 * @section Author
 *  Loïc Boutter - main developer (<a href="http://loicboutter.fr/">website</a>)
 *
 * @section Dependences
 *  This jukebox needs Qt5+, libsqlite 3+, SFML(OpenAL), libtag1 to work.
 *
 */



#include <QtWidgets/QApplication>
#include <QtSingleApplication/QtSingleApplication>
#include "windows/mainwindow.h"
#include "models/sqlmodelfactory.h"
#include "models/playlist.h"

#include <cassert>
#include <bandit/bandit.h>
using namespace bandit;

#ifdef DEBUG
#include <iostream>
#endif


int main(int argc, char *argv[])
{
    if(argc > 1 && strcmp(argv[1], "-test") == 0)
    {
        return bandit::run(argc, argv);
    }
    else
    {
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
}
