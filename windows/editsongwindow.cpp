#include "editsongwindow.h"
#include "mainwindow.h"

EditSongWindow::EditSongWindow(Song::Ptr song, MainWindow *parent) :
    QDialog(parent),
    mSong(song),
    mParent(parent)
{
    if(!mSong)
        throw LogicalFaultException("Try to initialize an EditSongWindow with a NULL song !");

    // Window properties
    setWindowTitle(tr("Propriétés de la chanson"));
    setMinimumSize(500, 600);

    // Window main layout
    QVBoxLayout* mainlayout = new QVBoxLayout;
    QTabWidget* tabs = new QTabWidget;
        QWidget* pageInformations = new QWidget;

    // Informations
    QFormLayout* formLayout = new QFormLayout;

        mTitleField = new QLineEdit;
        mTitleField->setText(mSong->title());
        formLayout->addRow(tr("Titre"), mTitleField);

        mAlbumField = new QLineEdit;
        mAlbumField->setText(mSong->album());
        formLayout->addRow(tr("Album"), mAlbumField);

        mArtistField = new QLineEdit;
        mArtistField->setText(mSong->artist());
        formLayout->addRow(tr("Artiste"), mArtistField);

        mFilepathField = new QLineEdit;
        mFilepathField->setText(mSong->filepath());
        formLayout->addRow(tr("Chemin fichier"), mFilepathField);

        mMarkField = new QComboBox;
        mMarkField->addItem("0/5", 0);
        mMarkField->addItem("1/5", 1);
        mMarkField->addItem("2/5", 2);
        mMarkField->addItem("3/5", 3);
        mMarkField->addItem("4/5", 4);
        mMarkField->addItem("5/5", 5);
        mMarkField->setCurrentIndex(mSong->mark());
        formLayout->addRow(tr("Note"), mMarkField);

    // Lyrics
    mLyricsField = new QTextEdit;
    mLyricsField->setText(mSong->lyrics());


    // Buttons zone
    QHBoxLayout* buttonsLayout = new QHBoxLayout;
        mOkButton = new QPushButton(tr("Valider"));
        mCancelButton = new QPushButton(tr("Annuler"));
        buttonsLayout->addWidget(mCancelButton);
        buttonsLayout->addStretch();
        buttonsLayout->addWidget(mOkButton);
        connect(mCancelButton, SIGNAL(clicked()), this, SLOT(close()));
        connect(mOkButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));

    // Tabs widgets & layouts
    pageInformations->setLayout(formLayout);
    tabs->addTab(pageInformations, tr("Informations"));
    tabs->addTab(mLyricsField, tr("Paroles"));


    // Window main layout
    mainlayout->addWidget(tabs);
    mainlayout->addLayout(buttonsLayout);

    setLayout(mainlayout);
}


void EditSongWindow::saveAndClose()
{
    // We edit the song with the fields data
    mSong->setTitle(mTitleField->text());
    mSong->setArtist(mArtistField->text());
    mSong->setAlbum(mAlbumField->text());
    mSong->setFilepath(mFilepathField->text());
    mSong->setLyrics(mLyricsField->toPlainText());
    mSong->setMark(mMarkField->currentIndex());

    // We try to save the song in database
    try {
        mSong->save();
    }
    catch(SqlInsertFailedException& e)
    {
        QMessageBox::warning(this, tr("Erreur"), tr("Une erreur s'est produite lors de la sauvegarde... ").append(e.what()));
    }

    // Close the window
    close();
}
