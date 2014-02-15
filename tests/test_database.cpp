#include <bandit/bandit.h>
#include <QStringList>
#include <iostream>

#include "../database/databasemanager.h"
#include "../database/sqlmodel.h"

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
    });
});

