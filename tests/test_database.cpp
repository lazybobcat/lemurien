#include <bandit/bandit.h>
#include <QStringList>
#include <iostream>

#include "../database/databasemanager.h"
#include "../models/sqlmodel.h"
#include "../models/song.h"

using namespace bandit;
using namespace std;


go_bandit([]() {
    describe("[Database] Testing :", []() {
        DatabaseManager DBManager("test");

        it("QSqlDatabase connection", [&]() {
            AssertThat(DBManager.db()->isOpen(), Equals(true));
        });

        it("Get Tables list", [&]() {
            QStringList list = DBManager.db()->tables();
            AssertThat(list.empty(), Equals(false));
            AssertThat(list.count(), IsGreaterThan(3));
        });

        it("Song creation", [&]() {
            Song::Ptr song(new Song(DBManager.db()));
            song->setTitle("Test song");
            song->setArtist("CPP FTW");
            song->setAlbum("Testing Hits");
            song->setFilepath("/path/to/file");
            song->save();

            // Try to gt it back to be sure
            Song::Ptr check_song(new Song(DBManager.db()));
            check_song->setPrimaryKey(song->key());
            check_song->construct();

            AssertThat(check_song->title(), Equals(song->title()));
        });

        // Deet the temp database to not fail next tests
        system("rm test.sqlite");
    });
});

