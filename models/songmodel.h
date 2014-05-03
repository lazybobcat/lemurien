#ifndef SONGMODEL_H
#define SONGMODEL_H

#include <QObject>
#include "sqlmodel.h"
#include "config.h"
#include <memory>
#ifdef DEBUG
#include <iostream>
#endif

class SongModel : public QObject, public std::enable_shared_from_this<SongModel>
{
    Q_OBJECT

public:
    typedef std::shared_ptr<SongModel> Ptr;

    #ifdef DEBUG
    friend std::ostream& operator<<(std::ostream&, const SongModel*);
    #endif
    friend class SqlModelFactory;

public:
    explicit    SongModel(QObject *parent = 0);

    void        save();
    void        construct();
    void        erase();

    void        setTitle(const QString& title);
    void        setArtist(const QString& artist);
    void        setAlbum(const QString& album);
    void        setFilepath(const QString& path);
    void        setLyrics(const QString& lyrics);
    void        setMark(unsigned int mark);
    void        playOneMoreTime();

    QString     title() const;      ///< Title Getter
    QString     artist() const;     ///< Artist Getter
    QString     album() const;      ///< Album Getter
    QString     filepath() const;   ///< Filepath Getter
    QString     lyrics() const;     ///< Lyrics Getter
    unsigned int mark() const;      ///< Mark Getter
    unsigned int nbplay() const;    ///< Number of plays Getter

signals:
    void        dataChanged(Ptr);

public slots:

private:
    QString         mTitle;     ///< The song title
    QString         mArtist;    ///< The song author/artist
    QString         mAlbum;     ///< The song album
    QString         mFilepath;  ///< The song file path, unique in database.
    unsigned int    mNbPlay;    ///< The number of times the song has been played
    unsigned int    mMark;      ///< A mark between 0 and Config::cMaxMark for that song
    QString         mLyrics;    ///< The song lyrics, can be pretty heavy

};

#endif // SONGMODEL_H
