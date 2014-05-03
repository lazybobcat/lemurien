#ifndef QSONG_H
#define QSONG_H

#include <QObject>
#include "../models/songmodel.h"

class QSong : public QObject
{
    Q_OBJECT
public:
    explicit QSong(QObject *parent = 0);
    QSong(SongModel::Ptr model, QObject *parent = 0);
    QSong(const QSong& rhs);
    QSong& operator=(const QSong& rhs);

    void            setModel(SongModel::Ptr model);
    SongModel::Ptr  model() const;

signals:
    void            modelChanged();

public slots:

private:
    SongModel::Ptr  mModel;

};

#endif // QSONG_H
