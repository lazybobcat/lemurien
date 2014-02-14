#ifndef PLAYLISTTABLEMODEL_H
#define PLAYLISTTABLEMODEL_H

#include <QAbstractTableModel>
#include <QVariant>
#include "../playlists/playlist.h"

/**
 * @brief This is a (Qt) Model used by a view to display a playlist (a list of songs)
 */
class PlaylistTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit PlaylistTableModel(QObject *parent = 0);

    /**
     * @brief Sets the new playlis for the model and emit a signal to inform views that the data has changed
     * @param playlist The new playlist that need to be displayed
     */
    void setPlaylist(Playlist::Ptr playlist);

    /**
     * @brief Return the number of rows the view need to display. Here : the number of song in the playlist
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    /**
     * @brief Return the number of column the view need to have to display the playlist correctly. Here : 5 for our 5 song informations
     */
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    

    /**
     * @brief The all-in-one Qt Models method : can return data, formating informations for the given index
     * @param index The cell index
     * @param role The role is used to know what the view want to display (data, format, ...)
     * @return The asked data or an empty QVariant if non-handled role
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    /**
     * @brief Return header data, header format accordin to the given role
     * @param The line or column (depending on the orientation) of asked header
     * @param orientation Horizontal or Vertical : if the header of a column or a line is asked
     * @param role The role is used to know what the view want to display (data, format, ...)
     * @return The asked data or an empty QVariant if non-handled role/orientation
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

signals:
    
public slots:

private:
    Playlist::Ptr   mPlaylist;
    
};

#endif // PLAYLISTTABLEMODEL_H
