#include "qalbum.h"
#include <QPixmap>
#include <QIcon>
#include <QDir>
#include <QFile>
#include <QStringList>
#include <QStandardPaths>
#include <QVBoxLayout>

#include <iostream>

QAlbum::QAlbum(const QString &name, const QString &artist, QWidget *parent) :
    QWidget(parent),
    mImage(nullptr),
    /*mNameLabel(nullptr),
    mArtistLabel(nullptr),*/
    mName(name),
    mArtist(artist)
{
    mName = mName.toLower().replace(' ', '_');
    mArtist = mArtist.toLower().replace(' ', '_');
    QStringList locations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
    QString imagePath = ":/data/music.png";

    for(QString path : locations)
    {
        QString test = path + QDir::separator() + ".Lemurien" + QDir::separator() + mName + "-" + mArtist + ".jpg";
        if(QFile(test).exists())
        {
            imagePath = test;
            break;
        }
    }

    QPixmap pixmap(imagePath);
    QIcon icon(pixmap);

    mName = name;
    mArtist = artist;

    mImage = new QPushButton(icon, "", this);
    mImage->setIconSize(QSize(200,200));
    mImage->setMaximumSize(QSize(200,200));
    mImage->setFlat(true);
    connect(mImage, SIGNAL(clicked()), this, SLOT(onTrigger()));

    QLabel *nameLabel = new QLabel(mName);
    nameLabel->setStyleSheet("QLabel { color : #000; font-size : 14px; font-weight : bold; }");
    nameLabel->setMaximumWidth(200);

    QLabel *artistLabel = new QLabel(mArtist);
    artistLabel->setStyleSheet("QLabel { color : #666; font-size : 13px; }");
    artistLabel->setMaximumWidth(200);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(mImage);
    layout->addWidget(nameLabel);
    layout->addWidget(artistLabel);
    layout->addStretch();
}

void QAlbum::onTrigger()
{
    emit clicked(mName, mArtist);
}
