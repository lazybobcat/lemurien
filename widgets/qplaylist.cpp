#include "qplaylist.h"

QPlaylist::QPlaylist(QObject *parent) :
    QObject(parent)
{
}


void QPlaylist::rename(const QString &name)
{
    mName = name;
}

QString QPlaylist::name() const
{
    return mName;
}

void QPlaylist::append(QSong song)
{
    unsigned int index = size();
    insert(index, song);
}
