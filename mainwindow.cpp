#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    mediaObject = new Phonon::MediaObject(this);
    //metaInformationResolver = new Phonon::MediaObject(this);

    mediaObject->setTickInterval(1000);
    mediaObject->setCurrentSource(QString("/home/lo-x/Musique/Ogres_de_Barback.mp3"));
    mediaObject->play();

    Phonon::createPath(mediaObject, audioOutput);
}

MainWindow::~MainWindow()
{

}
