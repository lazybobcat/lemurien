#include "song.h"

Song::Song(QSqlDatabase* db)
    : SqlModel(db, "songs", "id")
{
}
