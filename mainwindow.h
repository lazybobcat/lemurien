#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMainWindow>
#include <phonon/mediaobject.h>
#include <phonon/mediasource.h>
#include <phonon/audiooutput.h>
#include <phonon/mediasource.h>

#include "ui_mainwindow.h"


class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Phonon::AudioOutput* audioOutput;
    Phonon::MediaObject* mediaObject;

};

#endif // MAINWINDOW_H
