#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql>

#include "helpers.h"

class DataBase : public QObject {
    Q_OBJECT

signals:

    void addedNewPackage();

public:
    DataBase(const DataBase&) = delete;
    DataBase& operator=(const DataBase&) = delete;

    explicit DataBase(QObject *parent = nullptr);

    static DataBase& instance();

    void createNew(const QString& dbFile);
    void openDataBase(const QString& dbFile);

    QSqlDatabase& getDataBase();

    void addPackage(const QString& opcode, const QString& data, uint pkgLen, const QDateTime& timestamp);

private:
    void createTables();

    QSqlDatabase mDb;
};


#endif // DATABASE_H
