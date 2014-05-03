#include "qsong.h"

QSong::QSong(QObject *parent) :
    QObject(parent),
    mModel(nullptr)
{
}

QSong::QSong(SongModel::Ptr model, QObject *parent) :
    QObject(parent),
    mModel(model)
{

}

QSong::QSong(const QSong &rhs) :
    QObject(rhs.parent())
{
    mModel = rhs.model()->shared_from_this();
}

QSong& QSong::operator=(const QSong& rhs)
{
    mModel = rhs.model()->shared_from_this();
    return *this;
}


void QSong::setModel(SongModel::Ptr model)
{
    mModel = model;
    emit modelChanged();
}

SongModel::Ptr QSong::model() const
{
    return mModel;
}
