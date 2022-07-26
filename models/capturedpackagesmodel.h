#ifndef CAPTUREDPACKAGESMODEL_H
#define CAPTUREDPACKAGESMODEL_H

#include <QAbstractTableModel>
#include <QString>

#include "../pkgcap/rawdatastorage.h"
#include "../packetlistrecord.h"

class CapturedPackagesModel : public QAbstractTableModel {
    Q_OBJECT
public:
    CapturedPackagesModel(QObject *parent);

    QVariant data(const QModelIndex& index, int role) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    //добавление строки
    bool insertRows(int row, int count, const QModelIndex &parent);
private slots:

    void addInsertedPackage();

private:
    QStringList mTableHeaders;
    std::vector<GPC::RecordColumns> columns;
    RawDataStorage *mRawDataStorage;
};

#endif // CAPTUREDPACKAGESMODEL_H
