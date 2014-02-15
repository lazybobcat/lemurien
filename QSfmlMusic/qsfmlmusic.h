#ifndef QSFMLMUSIC_H
#define QSFMLMUSIC_H

#include <QWidget>
#include <QTimer>
#include <SFML/Audio/Music.hpp>

class QSfmlMusic : public QWidget, public sf::Music
{
    Q_OBJECT
public:
    explicit QSfmlMusic(QWidget *parent = 0);

    void    play();
    void    stop();

public slots:
    void    tick();

signals:
    void    tick(sf::Time);
    void    aboutToFinish();

private:
    QTimer*     mTimer;

};

#endif // QSFMLMUSIC_H
