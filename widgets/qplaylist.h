#ifndef QPLAYLIST_H
#define QPLAYLIST_H

#include <QObject>
#include <QMap>
#include "qsong.h"

class QPlaylist : public QObject, public QMap<unsigned int,QSong>
{
    Q_OBJECT
public:
    explicit QPlaylist(QObject *parent = 0);

    void    rename(const QString& name);
    QString name() const;

    void    append(QSong song);


signals:

public slots:

private:
    QString     mName;

};

#endif // QPLAYLIST_H
