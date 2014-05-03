#ifndef QALBUM_H
#define QALBUM_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class QAlbum : public QWidget
{
    Q_OBJECT
public:
    QAlbum(const QString &name, const QString &artist, QWidget *parent = 0);

signals:
    void    clicked(const QString& album, const QString& artist);

public slots:
    void    onTrigger();

private:
    QPushButton*    mImage;
    QString         mName;
    QString         mArtist;

};

#endif // QALBUM_H
