#include "mainwindow.h"
#include <taglib/tag.h>
#include <taglib/fileref.h>

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (mTrayIcon && mTrayIcon->isVisible())
    {
        hide();
        mMaximizeAction->setVisible(true);
        event->ignore();
    }
}

void MainWindow::showEvent(QShowEvent *)
{
    mMaximizeAction->setVisible(false);
}

void MainWindow::handleParams(const QStringList& params1)
{
    // Filters files, we want music only !
    QStringList params = params1.filter(QRegExp(".*\\.(mp3|ogg|m4a|wav|flac|mp4|wma|mid)", Qt::CaseInsensitive));

    // If there were no songs, do nothing
    if(params.size() == 0)    return;

    // The first arguments to handle
    if(!mArgumentsPlaylist)
    {
        // We dynamically create a UnsavablePlaylist* that we dynamically cast into a Playlist*
        mArgumentsPlaylist.reset(dynamic_cast<Playlist*>(SqlModelFactory::instance()->create("unsavable_playlist")));
        // Rename it, nicer
        mArgumentsPlaylist->rename(tr("Ouverts"));
        // Insert playlist in the playlists list
        mPlaylists.insert(tr("Ouverts"), mArgumentsPlaylist);
        // Add an action in the sidebar for the playlist
        QAction* playlistAction = mSidebar->addAction(tr("Ouverts"), QIcon(":/data/box.png"));
        playlistAction->setData(tr("Ouverts"));

        connect(playlistAction, SIGNAL(toggled(bool)),
                this,           SLOT(playlistChanged(bool)));

        // Rebuild sidebar to have things in right order, etc..
        rebuildSidebar();
    }

    // Iterates on results and create songs that we will add to the playlist
    for(int i = 0; i < params.size(); ++i)
    {
        Song::Ptr song = importSong(params.at(i));
        mArgumentsPlaylist->append(song);
    }

    // Details : show the playlist, set it as the current playlist and check it in the sidebar
    mMusicModel->setPlaylist(mArgumentsPlaylist);
    mSource = mArgumentsPlaylist;
    mSidebar->checkAction(tr("Ouverts"));
}

void MainWindow::handleMessage(const QString& message)
{
    handleParams(message.split(' ', QString::SkipEmptyParts));
}

/**
 * @brief Change the value of duration labels. Sould be called once per second.
 * @param time The song actual time
 */
void MainWindow::tick(qint64 time)
{
    // Displaying elapsed time with mm:ss format
    QTime elapsedTime(0, (time / 60000) % 60, (time / 1000) % 60);
    mElapsedTimeLabel->setText(elapsedTime.toString("mm:ss"));

    // Displaying elapsed time with mm:ss format
    QTime remainingTime(0, (mMediaObject->remainingTime() / 60000) % 60, (mMediaObject->remainingTime() / 1000) % 60);
    mRemainingTimeLabel->setText(remainingTime.toString("-mm:ss"));
}

/**
 * @brief The song state changed (Phonon states), we need to do some graphical change. States are Play, Pause, Stopped, Error, Buffering, etc..
 * @param newstate The new state
 */
void MainWindow::stateChanged(Phonon::State newstate,Phonon::State /*oldstate*/)
{
    switch(newstate)
    {
    case Phonon::ErrorState:
        if (mMediaObject->errorType() == Phonon::FatalError)
        {
            QMessageBox::warning(this, tr("Fatal Error"), mMediaObject->errorString());
            close();
        }
        else
        {
            QMessageBox::warning(this, tr("Error"), mMediaObject->errorString());
        }
        break;

    case Phonon::PlayingState:
        // Hide the play button (that will be replaced by the pause button)
        mPlayAction->setEnabled(false);
        mPlayAction->setVisible(false);

        // Show the pause button
        mPauseAction->setEnabled(true);
        mPauseAction->setVisible(true);

        // Activate stop button
        mStopAction->setEnabled(true);
        break;

    case Phonon::StoppedState:
        // Show the play button
        mPlayAction->setEnabled(true);
        mPlayAction->setVisible(true);

        // Hide the pause button (replaced by the play button)
        mPauseAction->setEnabled(false);
        mPauseAction->setVisible(false);

        // Deactivate stop button
        mStopAction->setEnabled(false);

        // Reset time labels to zero
        mElapsedTimeLabel->setText("00:00");
        mRemainingTimeLabel->setText("00:00");
        break;

    case Phonon::PausedState:
        // Show the play button
        mPlayAction->setEnabled(true);
        mPlayAction->setVisible(true);

        // Hide the pause button (replaced by the play button)
        mPauseAction->setEnabled(false);
        mPauseAction->setVisible(false);

        // Activate stop button
        mStopAction->setEnabled(true);
        break;

    case Phonon::BufferingState:
    default:

        break;
    }
}

/**
 * @brief This slot change the playlist currently in the view and played
 * @param checked Must be true to change the playlist
 */
void MainWindow::playlistChanged(bool checked)
{
    if(!checked) return;
    // Save the previous playlist
    if(mSource)
        mSource->save();

    // Get the new playlist name
    if(mSidebar->checkedAction())
    {
        QString playlistName = mSidebar->checkedAction()->data().toString();
        mMusicModel->setPlaylist(mPlaylists[playlistName]);
        mMusicTable->setModel(mMusicModel);
    }
}

/**
 * @brief When a song end or when the first song is played, the (song) source must be changed as well as the labels.
 */
void MainWindow::currentSourceChanged()
{
    if(!mSource)    return;

    // Select the new line in the table view
    mMusicTable->selectRow(mIndexOfSource);

    // Change the window title and mSongLabel with the new title
    QString message;
    QString title = mSource->at(mIndexOfSource)->title();
    QString artist = mSource->at(mIndexOfSource)->artist();
    setWindowTitle(title + " - Lémurien");
    message = "<b>" + title + "</b> - " + artist;
    mSongLabel->setText(message);

    // Change the system tray informations and popup message
    if(QSystemTrayIcon::isSystemTrayAvailable())
    {
        popTrayMessage(tr("Now Playing"), QString(message + "<br />" + mSource->at(mIndexOfSource)->album() + "<br />" + tr("Noté %1/%2")).arg(mSource->at(mIndexOfSource)->mark()).arg(Config::cMaxMark));
        mTrayIcon->setToolTip(message);
    }

    // Change wikipedia if window opened
    if(mWebkitWindow)
        mWebkitWindow->setWebPage("http://en.wikipedia.org/wiki/" + artist);

    mSource->at(mIndexOfSource)->playOneMoreTime();
    mSource->at(mIndexOfSource)->save();
}

/**
 * @brief Pops up a tray information message containing information for the user
 */
void MainWindow::popTrayMessage(const QString& title, const QString& message, QSystemTrayIcon::MessageIcon icon)
{
    mTrayIcon->showMessage("Lémurien - " + title, message, icon, 5000);
}

/**
 * @brief This slot is automatically called just before the song ends, we need to change the song "source" according to play modes, playlist size, etc..
 */
void MainWindow::aboutToFinish()
{
    if(!mSource)    return;

    // Check the play mode : normal, shuffle, repeat once
    if(mShuffleAction->isChecked()) // Case shuffle
    {
        mIndexOfSource = qrand() % mSource->size(); // Random the next song
    }
    else if(mRepeatOnceAction->isChecked()) // Case repeat once
    {
        // Nothing to do, the next song is the actual song
    }
    else // Case normal mode
    {
        mIndexOfSource++; // Go to the next song
    }

    // If we are too far in the list, come back to the beginning
    if(mIndexOfSource >= (unsigned int)mMusicModel->rowCount())
        mIndexOfSource = 0;

    // Enqueue next song
    mMediaObject->enqueue(mSource->at(mIndexOfSource)->filepath());
}

/**
 * @brief Play next song, according to play modes, playlist size, etc..
 */
void MainWindow::next()
{
    if(!mSource)    return;

    // Check the play mode : normal, shuffle, repeat once
    if(mShuffleAction->isChecked()) // Case shuffle
    {
        mIndexOfSource = qrand() % mSource->size(); // Random the next song
    }
    else if(mRepeatOnceAction->isChecked()) // Case repeat once
    {
        // Nothing to do, the next song is the actual song
    }
    else // Case normal mode
    {
        mIndexOfSource++; // Go to the next song
    }

    // If we are too far in the list, come back to the beginning
    if(mIndexOfSource >= (unsigned int)mMusicModel->rowCount())
        mIndexOfSource = 0;

    mMediaObject->stop();
    mMediaObject->clearQueue();
    mMediaObject->setCurrentSource(mSource->at(mIndexOfSource)->filepath());
    mMediaObject->play();
}

/**
 * @brief Play previous song, according to play modes, playlist size, etc..
 */
void MainWindow::previous()
{
    if(!mSource)    return;

    // Check the play mode : normal, shuffle, repeat once
    if(mShuffleAction->isChecked()) // Case shuffle
    {
        mIndexOfSource = qrand() % mSource->size(); // Random the next song
    }
    else if(mRepeatOnceAction->isChecked()) // Case repeat once
    {
        // Nothing to do, the next song is the actual song
    }
    else // Case normal mode
    {
        --mIndexOfSource; // Go to the previous song
    }

    // If we are too far in the list, come back to the beginning
    if(mIndexOfSource == -1u)
        mIndexOfSource = mMusicModel->rowCount() - 1;

    mMediaObject->stop();
    mMediaObject->clearQueue();
    mMediaObject->setCurrentSource(mSource->at(mIndexOfSource)->filepath());
    mMediaObject->play();
}

/**
 * @brief Repeat Once mode will repeat the same song again and again
 */
void MainWindow::repeatOnce()
{
    mShuffleAction->setChecked(false);
}

/**
 * @brief Shuffle mode with select next (and previous) songs at random
 */
void MainWindow::shuffle()
{
    mRepeatOnceAction->setChecked(false);
}

/**
 * @brief This slot is called when the (playlist) Table View has been double clicked. We change te source for the song that has been double clicked
 * @param index
 */
void MainWindow::tableClicked(const QModelIndex &index)
{
    if(!mSidebar->checkedAction()) return;

    // Get the new playlist name
    QString playlistName = mSidebar->checkedAction()->data().toString();

    if(!mSource)
    {
        mSource = mPlaylists[playlistName];
    }

    if(mSource->name() != playlistName)
    {
        // Save the previous playlist
        if(mSource)
            mSource->save();

        mMusicModel->setPlaylist(mPlaylists[playlistName]);
        mSource.reset();
        mSource = mPlaylists[playlistName];
    }

    if(index.row() < mSource->size() && index.row() >= 0)
        mIndexOfSource = index.row();
    else
        mIndexOfSource = 0;


    mMediaObject->stop();
    mMediaObject->clearQueue();
    mMediaObject->setCurrentSource(mSource->at(mIndexOfSource)->filepath());
    mMediaObject->play();
}

/**
 * @brief Pop the Add Files window
 */
void MainWindow::addFilesToLibrairy()
{
    if(mAddfilesWindow)
    {
        mAddfilesWindow->close();
        delete mAddfilesWindow;
    }

    mAddfilesWindow = new AddfilesWindow(this);
    mAddfilesWindow->show();
}



/**
 * @brief Pop the about window
 */
void MainWindow::about()
{
    QMessageBox::information(this, tr("A propos de Lémurien"),
                                tr("<p align=\"center\"><b>Lémurien</b><br />%1.%2<br /></p>"
                                     "<p align=\"center\">Lémurien est un logiciel d'écoute et de gestion musicale</p>"
                                     "<p align=\"center\">Créé par <a href=\"http://loicboutter.fr\">Loïc Boutter</a> - Licence GPL</p>"
                                     "<p align=\"center\">Les icones \"cartoon\" ont été crées par <a href=\"http://shlyapnikova.deviantart.com/\">shlyapnikova</a> - Licence Creative Commons Attribution 3.0"
                                     "<p align=\"center\">Merci de l'avoir essayé !</p>").arg(Config::cMajorVersion).arg(Config::cMinorVersion));
}

void MainWindow::shrink()
{
    // If we just ask to shrink the player
    if(mShrinkAction->isChecked())
    {
        // Hide uneeded widgets
        mSidebar->hide();
        mMusicTable->hide();
        mSearchField->setEnabled(false);
        mSearchButton->setEnabled(false);
        // Shrink the window
        resize(600, 50);
    }
    // Else we want to maximize the player again
    else
    {
        // Show needed widgets
        mSidebar->show();
        mMusicTable->show();
        mSearchField->setEnabled(true);
        mSearchButton->setEnabled(true);
        // Maximize the window
        resize(960, 600);
    }
}


/**
 * @brief Pop the Add Playlist window and handle the creation of the playlist
 */
void MainWindow::addPlaylist()
{
    QString oldSelectedPlaylistName;
    if(mSidebar->checkedAction())
        oldSelectedPlaylistName = mSidebar->checkedAction()->data().toString();

    bool ok;
    QString playlist_name = QInputDialog::getText(this, tr("Ajouter une playlist"),
                                                tr("Nom de la playlist :"), QLineEdit::Normal,
                                                "", &ok);
    if (!ok || playlist_name.isEmpty())
        return;

    // Playlist creation
    Playlist::Ptr playlist = SqlModelFactory::instance()->createPlaylist();
    playlist->rename(playlist_name);
    playlist->save();

    // Insert playlist in the playlists list
    mPlaylists.insert(playlist_name, playlist);

    // Add an action in the sidebar for the playlist
    QAction* playlistAction = mSidebar->addAction(playlist_name, QIcon("data/playlist.png"));
    playlistAction->setData(playlist_name);

    connect(playlistAction, SIGNAL(toggled(bool)),
            this,           SLOT(playlistChanged(bool)));

    // Rebuild sidebar to have things in right order, etc..
    rebuildSidebar();

    if(oldSelectedPlaylistName.size() > 0)
        mSidebar->checkAction(oldSelectedPlaylistName);
}


/**
 * @brief Pop the Remove Playlist warning window and handle the removing of the playlist
 */
void MainWindow::removePlaylist()
{
    if(!mSidebar->checkedAction()) return;

    QString playlistName = mSidebar->checkedAction()->data().toString();
    int ret = QMessageBox::warning(this, tr("Supprimer une playlist"),
                                             tr("La playlist %1 sera définitivement supprimée.\n"
                                                "Êtes vous sûr de vouloir continuer ?").arg(playlistName),
                                             QMessageBox::Cancel,
                                             QMessageBox::Ok);
    if(ret != QMessageBox::Ok)  return;

    try {
        mPlaylists[playlistName]->erase();
        mSidebar->removeAction(playlistName);
        mPlaylists.remove(playlistName);
    }
    catch(SqlException& e)
    {
        QMessageBox::warning(this, tr("Erreur"), tr("Une erreur s'est produite lors de la supression... ").append(e.what()));
    }
    catch(LogicalFaultException& e)
    {
        std::cerr << "FError : " << e.what() << std::endl;
    }
}

/**
 * @brief Pop the Rename Playlist window and handle the renaming of the playlist
 */
void MainWindow::renamePlaylist()
{
    if(!mSidebar->checkedAction()) return;

    QString playlistName = mSidebar->checkedAction()->data().toString();

    bool ok;
    QString newname = QInputDialog::getText(this, tr("Renommer une playlist"),
                                                tr("Nom de la playlist :"), QLineEdit::Normal,
                                                playlistName, &ok);
    if (!ok || newname.isEmpty())
        return;

    // Playlist creation
    Playlist::Ptr playlist = mPlaylists[playlistName];
    try {
        playlist->rename(newname);
        playlist->save();
    }
    catch(SqlException& e)
    {
        QMessageBox::warning(this, tr("Erreur"), tr("Une erreur s'est produite lors de la sauvegarde... ").append(e.what()));
    }

    mPlaylists.remove(playlistName);

    // Insert playlist in the playlists list
    mPlaylists.insert(newname, playlist);

    // Remove its action from Sidebar
    mSidebar->removeAction(playlistName);

    // Add an action in the sidebar for the playlist
    QAction* playlistAction = mSidebar->addAction(newname, QIcon("data/playlist.png"));
    playlistAction->setData(newname);

    // Rebuild sidebar to have things in right order, etc..
    rebuildSidebar();

    mSidebar->checkAction(newname);
}

/**
 * @brief Pop the context menu in the (playlis) Table View and handle Menu Actions
 * @param point The local coordinates of click
 */
void MainWindow::popContextMenuOnSong(const QPoint& point)
{
    // Map coordinates to global coordinates
    QPoint globalPos = mMusicTable->viewport()->mapToGlobal(point);

    // Get a list of selected items in the table
    QModelIndexList list = mMusicTable->selectionModel()->selectedIndexes();
    if(list.empty() || !mSidebar->checkedAction())    return;
    // We know there can only be one selected item, furthermore the list contains N times the same row where N is the number of column int the TableView
    QModelIndex index = list.at(1);

    // Get the actual playlist in he table
    QString playlistName = mSidebar->checkedAction()->data().toString();
    Playlist::Ptr playlist = mPlaylists[playlistName];

    if(index.row() >= 0 && index.row() < playlist->size())
    {
        // Get the song corresponding to the row
        Song::Ptr song = playlist->at(index.row());

        // Creation of contextual menu
        QMenu playlistMenu;
        playlistMenu.setTitle(tr("Ajouter à la playlist..."));
        for(QMap<QString, Playlist::Ptr>::iterator it = mPlaylists.begin(); it!=mPlaylists.end(); ++it)
        {
            if(it.key() != mMusicLibrairy->name())
                playlistMenu.addAction(it.key());
        }

        QMenu contextMenu;
        contextMenu.addAction(tr("Informations"));
        contextMenu.addAction(tr("Wikipedia"));
        contextMenu.addMenu(&playlistMenu);
        contextMenu.addAction(tr("Supprimer"));


        // Execution of the context menu
        QAction* action = contextMenu.exec(globalPos);
        if(action)
        {
            // We want to edit the informations about the song
            if(action->text() == tr("Informations"))
            {
                if(mEditSongWindow)
                {
                    mEditSongWindow->close();
                    delete mEditSongWindow;
                }

                mEditSongWindow = new EditSongWindow(song, this);
                mEditSongWindow->show();
            }
            // We want to show the wikipedia page for the band
            else if(action->text() == tr("Wikipedia"))
            {
                if(mWebkitWindow)
                    delete mWebkitWindow;

                QString url("http://en.wikipedia.org/wiki/" + song->artist());

                mWebkitWindow = new WebkitWindow(url, this);
                mWebkitWindow->show();
            }
            // We want to remove the song from the playlist
            else if(action->text() == tr("Supprimer"))
            {
                // If the playlist is the music librairy, we remove the song from database and all other playlists
                if(playlist->name() == mMusicLibrairy->name())
                {
                    for(QMap<QString, Playlist::Ptr>::iterator it = mPlaylists.begin(); it!=mPlaylists.end(); ++it)
                        (*it)->remove(song);
                    song->erase();
                }

                playlist->remove(song);
            }
            // We want to put the song into a playlist
            else
            {
                for(QMap<QString, Playlist::Ptr>::iterator it = mPlaylists.begin(); it!=mPlaylists.end(); ++it)
                {
                    if((*it)->name() == action->text())
                    {
                        if(!(*it)->contains(song))
                        {
                            (*it)->append(song);
                            (*it)->save();
                        }
                        break;
                    }
                }
            }
        }
    }
}

/**
 * @brief Search songs in the Database on different column (title, artist, album) and create a special unsavable playlist to display results
 */
void MainWindow::search()
{
    // First we get the string on wich the search must be done
    QString search = mSearchField->text();

    // If the field is empty, we do not waste time and CPU for SQL queries...
    if(search.size() == 0)  return;

    // Get a pointer on the database via a playlist (avoid to construct a new Databaseanager object -> heavy operation)
    QSqlDatabase* db = mMusicLibrairy->db();
    if(db->isOpen())
    {
        // Get the songs in the database
        QSqlQuery query;
        query.prepare(QString("SELECT id FROM songs "
                              "WHERE title LIKE '%"+search+"%' OR album LIKE '%"+search+"%' OR artist LIKE '%"+search+"%'"));
        if(query.exec())
        {
            // The fist search, the pointer isn't set yet
            if(!mSearchedPlaylist)
            {
                // We dynamically create a UnsavablePlaylist* that we dynamically cast into a Playlist*
                mSearchedPlaylist.reset(dynamic_cast<Playlist*>(SqlModelFactory::instance()->create("unsavable_playlist")));
                // Rename it, nicer
                mSearchedPlaylist->rename(tr("Recherche"));
                // Insert playlist in the playlists list
                mPlaylists.insert(tr("Recherche"), mSearchedPlaylist);
                // Add an action in the sidebar for the playlist
                QAction* playlistAction = mSidebar->addAction(tr("Recherche"), QIcon(":/data/zoom.png"));
                playlistAction->setData(tr("Recherche"));

                connect(playlistAction, SIGNAL(toggled(bool)),
                        this,           SLOT(playlistChanged(bool)));

                // Rebuild sidebar to have things in right order, etc..
                rebuildSidebar();
            }
            // Clear it to erase last search results
            mSearchedPlaylist->clear();

            // Iterates on results and create songs that we will add to the playlist
            while(query.next())
            {
                Song::Ptr song = SqlModelFactory::instance()->createSong(query.value(0).toUInt());
                if(song)
                    mSearchedPlaylist->append(song);
            }

            // Details : show the playlist, set it as the current playlist and check it in the sidebar
            mMusicModel->setPlaylist(mSearchedPlaylist);
            mSource = mSearchedPlaylist;
            mSidebar->checkAction(tr("Recherche"));
        }
        else
        {
            std::cerr << "Erreur dans la requete !" << std::endl;
            std::cerr << query.executedQuery().toStdString() << std::endl;
            std::cerr << query.lastError().text().toStdString() << std::endl;
        }
    }
    else
    {
        QMessageBox::warning(this, tr("Erreur"), tr("Une erreur s'est produite lors de l'accès à la base de données... "));
    }
}

/**
 * @brief Sorting slot, sort on title
 */
void MainWindow::sortOnTitle()
{
    if(!mSidebar->checkedAction()) return;

    QString playlistName = mSidebar->checkedAction()->data().toString();
    Playlist::Ptr playlist = mPlaylists[playlistName];

    playlist->sort(Playlist::SortOnTitle);
    mMusicModel->setPlaylist(mPlaylists[playlistName]);
}

/**
 * @brief Sorting slot, sort on album
 */
void MainWindow::sortOnAlbum()
{
    if(!mSidebar->checkedAction()) return;

    QString playlistName = mSidebar->checkedAction()->data().toString();
    Playlist::Ptr playlist = mPlaylists[playlistName];

    playlist->sort(Playlist::SortOnAlbum);
    mMusicModel->setPlaylist(mPlaylists[playlistName]);
}

/**
 * @brief Sorting slot, sort on artist
 */
void MainWindow::sortOnArtist()
{
    if(!mSidebar->checkedAction()) return;

    QString playlistName = mSidebar->checkedAction()->data().toString();
    Playlist::Ptr playlist = mPlaylists[playlistName];

    playlist->sort(Playlist::SortOnArtist);
    mMusicModel->setPlaylist(mPlaylists[playlistName]);
}

/**
 * @brief Sorting slot, sort on mark
 */
void MainWindow::sortOnMark()
{
    if(!mSidebar->checkedAction()) return;

    QString playlistName = mSidebar->checkedAction()->data().toString();
    Playlist::Ptr playlist = mPlaylists[playlistName];

    playlist->sort(Playlist::SortOnMark);
    mMusicModel->setPlaylist(mPlaylists[playlistName]);
}

/**
 * @brief Sorting slot, sort on nb play
 */
void MainWindow::sortOnNbPlay()
{
    if(!mSidebar->checkedAction()) return;

    QString playlistName = mSidebar->checkedAction()->data().toString();
    Playlist::Ptr playlist = mPlaylists[playlistName];

    playlist->sort(Playlist::SortOnNbplay);
    mMusicModel->setPlaylist(mPlaylists[playlistName]);
}

/**
 * @brief Try to import a song from its filepath. If that song wasn't already imported, try to extract information from file with TagLib and add the song into the MusicLibrary.
 * @param filepath
 * @return
 */
Song::Ptr MainWindow::importSong(const QString &filepath)
{
    TagLib::FileRef f(filepath.toStdString().c_str());

#ifdef DEBUG
    std::cout << "Importing : ";
    std::cout << f.tag()->title() << " / "
              << f.tag()->artist() << " / "
              << f.tag()->album();
    std::cout << std::endl;
#endif

    Song::Ptr song;

    bool r = SqlModelFactory::instance()->getSong(filepath, song);
    if(!r)
    {
        song->setTitle(f.tag()->title().toCString(true));
        song->setAlbum(f.tag()->album().toCString(true));
        song->setArtist(f.tag()->artist().toCString(true));

        try {
            song->save();
        }
        catch(SqlInsertFailedException& e)
        {
            // The user tried to add a song that is already in the database, no prob
            std::cerr << "Tried to add " << song->filepath().toStdString() << " but it is already in database. Continuing..." << std::endl;
        }
        catch(SqlDatabaseException& e)
        {
            std::cerr << "SQL Database is busy or not connected !" << std::endl;
        }
        catch(...)
        {
            std::cerr << "Unhandled exception !?" << std::endl;
        }

        if(!mMusicLibrairy->contains(song)) {
            mMusicLibrairy->append(song);
        }

        try {
            mMusicLibrairy->save();
        }
        catch(SqlInsertFailedException& e)
        {
            // The user tried to add a song that is already in the database, no prob
            std::cerr << "Tried to add " << song->filepath().toStdString() << " but it is already in database. Continuing..." << std::endl;
        }
        catch(SqlDatabaseException& e)
        {
            std::cerr << "SQL Database is busy or not connected !" << std::endl;
        }
        catch(...)
        {
            std::cerr << "Unhandled exception !?" << std::endl;
        }

    }
    return song;
}






