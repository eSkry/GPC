#include "database.h"

#include <QDebug>
#include <QFile>

DataBase::DataBase(QObject *parent)
    : QObject{parent} {
}

void DataBase::createNew(const QString &dbFile) {
    if (mDb.isOpen())
        mDb.close();

    mDb = QSqlDatabase::addDatabase("QSQLITE");
    mDb.setDatabaseName(dbFile);

    if (!mDb.open()) {
        qDebug() << mDb.lastError().text();
        return;
    }

    createTables();
}

void DataBase::openDataBase(const QString &dbFile) {
    if (mDb.isOpen())
        mDb.close();

    mDb = QSqlDatabase::addDatabase("QSQLITE");
    mDb.setDatabaseName(dbFile);

    if (!mDb.open()) {
        qDebug() << mDb.lastError().text();
    }
}

DataBase &DataBase::instance() {
    static DataBase db(nullptr);
    return db;
}

void DataBase::createTables() {
    if (!mDb.isOpen()) {
        qInfo() << "Database not opened";
        return;
    }

    QSqlQuery createTablesQuery(mDb);

    std::vector<QString> tableList = {
        readFileAsText(":/resources/package_info.sql"),
        readFileAsText(":/resources/packages.sql")
    };

    for (auto &it : tableList) {
        createTablesQuery.prepare(it);

        if (!createTablesQuery.exec()) {
            qWarning() << createTablesQuery.lastError().text();
        }
    }

}


QSqlDatabase &DataBase::getDataBase() {
    return mDb;
}


void DataBase::addPackage(const QString &opcode, const QString &data, uint pkgLen, const QDateTime &timestamp) {
    QSqlQuery query(mDb);
    query.prepare("INSERT INTO packages (opcode, pkgLen, data, timestamp) VALUES (:opcode, :pkgLen, :data, :timestamp)");
    query.bindValue(":opcode", opcode);
    query.bindValue(":pkgLen", pkgLen);
    query.bindValue(":data", data);
    query.bindValue(":timestamp", timestamp.toSecsSinceEpoch());

    if (!query.exec()) {
        qWarning() << query.lastError().text();
    }

    emit addedNewPackage();
}













