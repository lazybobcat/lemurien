#include <QApplication>
#include "mainwindow.h"
#include "sqlmodelfactory.h"
#include <iostream>


std::ostream& operator<<(std::ostream& os, const QString& s)
{
    os << s.toStdString();
    return os;
}

std::ostream& operator<<(std::ostream& os, const Song* song)
{
    os << "(" << boost::any_cast<unsigned int>(song->key()) << ") " << song->mTitle << " [" << song->mArtist << " : " << song->mAlbum << "] " << song->mFilepath << " {played:" << song->mNbPlay << "}";
    return os;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Lémurien");




    try {
        SqlModelFactory::instance()->init();
        Song* s = static_cast<Song*>(SqlModelFactory::instance()->create("song"));
        // Set the song data
        s->setTitle("Maybe Tomorrow");
        s->setArtist("Stereophonics");
        s->setAlbum("Pull the Pin");
        s->setFilepath("/home/lo-x/media/SDATA1/musique/Stereophonics/Pull the Pin/maybe_tomorrow.m4a");
        s->setMark(4);
        // Save the song
        s->save();
        s->setMark(1);
        s->save();
        // construc from database
        //s->construct();
        std::cout << s << std::endl;

        delete s;
    }
    catch(DatabaseManager::Error e)
    {
        std::cerr << "Unabled to open the SQLite database !" << std::endl;
        std::cerr << SqlModelFactory::instance()->dbManager()->lastError().text().toStdString() << std::endl;
    }
    catch(SqlModel::Error e)
    {
        std::cerr << "Model error !" << std::endl;
    }
    catch(...)
    {
        std::cerr << "Unknown error with the SQLite database !" << std::endl;
    }


    MainWindow w;
    w.show();


    
    return app.exec();
}
