#include <QApplication>
#include "mainwindow.h"
#include "sqlmodelfactory.h"
#include <iostream>

std::ostream& operator<<(std::ostream& os, const Song* song)
{
    os << song->mTitle << " [" << song->mArtist << " : " << song->mAlbum << "] " << song->mFilepath << " {played:" << song->mNbPlay << "}";
    return os;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Lémurien");




    try {
        SqlModelFactory::instance()->init();
        Song* s = static_cast<Song*>(SqlModelFactory::instance()->create("song"));
        s->save();
        s->setPrimaryKey(1);
        s->construct();
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
