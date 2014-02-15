#include "playlisttablemodel.h"

PlaylistTableModel::PlaylistTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

void PlaylistTableModel::setPlaylist(Playlist::Ptr playlist)
{
    /*reset();
    emit dataChanged(createIndex(0,0,this), createIndex(playlist->size(), 4, this));*/
    emit layoutAboutToBeChanged();
    mPlaylist = playlist;
    emit layoutChanged();
}


int PlaylistTableModel::rowCount(const QModelIndex &/*parent*/) const
{
    if(mPlaylist)
        return mPlaylist->size();

    return 0;
}

int PlaylistTableModel::columnCount(const QModelIndex &/*parent*/) const
{
    return 5;
}


QVariant PlaylistTableModel::data(const QModelIndex &index, int role) const
{
    if(!mPlaylist)
        return QVariant();

    if(!index.isValid() || index.column() > 4 || index.row() >= mPlaylist->size())
        return QVariant();

    switch(role)
    {
    case Qt::TextAlignmentRole:
        return Qt::AlignCenter;
        break;

    case Qt::DisplayRole:
        switch(index.column())
        {
        case 0:
            return mPlaylist->value(index.row())->title();

        case 1:
            return mPlaylist->value(index.row())->artist();

        case 2:
            return mPlaylist->value(index.row())->album();

        case 3:
            return mPlaylist->value(index.row())->mark();

        case 4:
            return mPlaylist->value(index.row())->nbplay();

        default:
            return QVariant();
        }
        break;

    default:
        return QVariant();
        break;
    }
}

QVariant PlaylistTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();

    switch(section)
    {
    case 0:
        return tr("Titre");

    case 1:
        return tr("Artiste");

    case 2:
        return tr("Album");

    case 3:
        return tr("Note");

    case 4:
        return tr("Nb Lectures");

    default:
        return QVariant();
    }
}
