
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
    QApplication app(argc, argv);
    app.setApplicationName("Lémurien");



#ifdef DEBUG
    try {
        SqlModelFactory::instance()->init();
        /*Song* s = static_cast<Song*>(SqlModelFactory::instance()->create("song"));
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

        delete s;*/
{
        Playlist::Ptr playlist = SqlModelFactory::instance()->createPlaylist();

        for(int i =0; i < 10; ++i)
        {
            Song::Ptr song = SqlModelFactory::instance()->createSong();
            song->setTitle(QString("Title "+i));
            song->setAlbum(QString("Supayr Lapin"));
            song->setArtist(QString("Cawotte"));
            song->setFilepath(QString("/musique/Cawotte/Supayr Lapin/title-"+i));
            playlist->append(song);
        }
        int i = 6;

        playlist->at(i)->setTitle("toto");
        playlist->save();
        playlist->save();
        playlist->rename("Toto Kontratak");
        playlist->save();
}
{
        Playlist::Ptr playlist = SqlModelFactory::instance()->createPlaylist();
        unsigned int id = 1u;
        playlist->setPrimaryKey(id);
        playlist->construct();

        std::cout << "Playlist size: " << playlist->size() << std::endl;
        std::cout << playlist << std::endl;
        playlist->remove(playlist->at(2));
        std::cout << playlist << std::endl;
}
    }
    catch(DatabaseManager::Error e)
    {
        std::cerr << "Unabled to open the SQLite database !" << std::endl;
        std::cerr << SqlModelFactory::instance()->dbManager()->lastError().text().toStdString() << std::endl;
    }
    catch(SqlModel::Error e)
    {
        std::cerr << "Model error !" << std::endl;
        std::cerr << SqlModelFactory::instance()->dbManager()->lastError().text().toStdString() << std::endl;
    }
#endif


    MainWindow w;
    w.show();

    return app.exec();
}
