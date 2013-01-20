#include "addfileswindow.h"
#include "mainwindow.h"

AddfilesWindow::AddfilesWindow(MainWindow *parent) : QDialog(parent), mNbFilepaths(0), mParent(parent)
{
    // Window properties
    setWindowTitle(tr("Ajouter un dossier musical"));
    resize(600, 100);

    // Window main layout
    QVBoxLayout* mainlayout = new QVBoxLayout;

    // Chose directory zone
    QHBoxLayout* choseDirLayout = new QHBoxLayout;
        mDirectoryLabel = new QLabel;
        mDirectoryLabel->setText(tr("Répertoire :"));
        choseDirLayout->addWidget(mDirectoryLabel);

        mDirectoryField = new QLineEdit;
        choseDirLayout->addWidget(mDirectoryField);

        mChoseDirectoryButton = new QPushButton(tr("Parcourir..."));
        choseDirLayout->addWidget(mChoseDirectoryButton);
        connect(mChoseDirectoryButton, SIGNAL(clicked()), this, SLOT(choseDirectory()));

    // Recursive inclusion line
    QHBoxLayout* recursiveLineLayout = new QHBoxLayout;
        mRecursiveField = new QCheckBox;
        mRecursiveField->setText(tr("Inclure les sous dossier récursivement ?"));
        recursiveLineLayout->addWidget(mRecursiveField);

    // Progress bar
        mProgressBar = new QProgressBar;
        mProgressBar->setRange(0, 100);

    // Buttons zone
    QHBoxLayout* buttonsLayout = new QHBoxLayout;
        mCancelButton = new QPushButton(tr("Annuler"));
        mOkButton = new QPushButton(tr("Valider"));
        buttonsLayout->addWidget(mCancelButton);
        buttonsLayout->addStretch();
        buttonsLayout->addWidget(mOkButton);
        connect(mCancelButton, SIGNAL(clicked()), this, SLOT(close()));
        connect(mOkButton, SIGNAL(clicked()), this, SLOT(startImporting()));

    // Window main layout
    mainlayout->addLayout(choseDirLayout);
    mainlayout->addLayout(recursiveLineLayout);
    mainlayout->addWidget(mProgressBar);
    mainlayout->addLayout(buttonsLayout);

    setLayout(mainlayout);
}

void AddfilesWindow::choseDirectory()
{
    QString directory_path = QFileDialog::getExistingDirectory(this,
                                                              tr("Choisissez le dossier musical"),
                                                              QDesktopServices::storageLocation(QDesktopServices::MusicLocation));

    if (directory_path.isEmpty())
        return;

    mDirectoryField->setText(directory_path);
}

void AddfilesWindow::startImporting()
{
    mOkButton->setEnabled(false);
    mCancelButton->setEnabled(false);


    // Folder
    QDir directory(mDirectoryField->text() + "/");

    // Do we include sngs recursively ?
    if(mRecursiveField->isChecked())
    {
        // Recursive inclusion
        prepareQueue(directory);
    }
    else
    {
        // Else we just want to include current directory
        QStringList filters;
            filters << "*.mp3" << "*.ogg" << "*.m4a" << "*.wav" << "*.flac" << "*.mp4" << "*.mid" << "*.wma"
                    << "*.MP3" << "*.OGG" << "*.M4A" << "*.WAV" << "*.FLAC" << "*.MP4" << "*.MID" << "*.WMA";

            directory.setNameFilters(filters);

            foreach(QFileInfo fileInfo, directory.entryInfoList())
            {
                mQueue.push(fileInfo.filePath());
            }
    }



    mNbFilepaths = mQueue.size();

    import();

    mParent->playlistChanged(true);
}

void AddfilesWindow::prepareQueue(const QDir &directory)
{
    QFileInfoList list = directory.entryInfoList(QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    foreach(QFileInfo fileInfo, list)
    {
        if(fileInfo.isDir())
        {
            QDir subdirectory = fileInfo.absoluteFilePath();
            prepareQueue(subdirectory);
        }
    }

    QStringList filters;
    filters << "*.mp3" << "*.ogg" << "*.m4a" << "*.wav" << "*.flac" << "*.mp4" << "*.mid" << "*.wma"
            << "*.MP3" << "*.OGG" << "*.M4A" << "*.WAV" << "*.FLAC" << "*.MP4" << "*.MID" << "*.WMA";

    list = directory.entryInfoList(filters, QDir::Files);
    foreach(QFileInfo fileInfo, list)
    {
        mQueue.push(fileInfo.filePath());
    }
}

void AddfilesWindow::import()
{
    while(!mQueue.empty())
    {
        QString source = mQueue.front();
        TagLib::FileRef f(source.toStdString().c_str());
        TagLib::String artist = f.tag()->artist();


#ifdef DEBUG
        std::cout << "Importing : ";
        std::cout << f.tag()->title() << " / "
                  << f.tag()->artist() << " / "
                  << f.tag()->album();
        std::cout << std::endl;
#endif



        Song::Ptr song;


            bool r = SqlModelFactory::instance()->getSong(source, song);
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
                    std::cout << "Tried to add " << song->filepath().toStdString() << " but it is already in database. Continuing..." << std::endl;
                }
                catch(SqlDatabaseException& e)
                {
                    std::cout << "SQL Database is busy or not connected !" << std::endl;
                }
                catch(...)
                {
                    std::cout << "Unhandled exception !?" << std::endl;
                }
            }

        if(mParent)
        {
            if(!mParent->mMusicLibrairy->contains(song)) {
                mParent->mMusicLibrairy->append(song);
            }

            try {
                mParent->mMusicLibrairy->save();
                mParent->mMusicModel->setPlaylist(mParent->mMusicLibrairy);
            }
            catch(...)
            {
                std::cout << "Unhandled exception !?" << std::endl;
            }
        }

        mProgressBar->setValue(static_cast<int>(100 - (100 * mQueue.size() / mNbFilepaths)));


        mQueue.pop();
    }

    close();
}
