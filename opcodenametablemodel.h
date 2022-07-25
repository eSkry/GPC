#ifndef OPCODENAMETABLEMODEL_H
#define OPCODENAMETABLEMODEL_H

#include <QAbstractTableModel>
#include <QModelIndex>
#include <vector>

#include "PackageData.h"

//class OpcodeNameTableModel : public QAbstractTableModel {
//    Q_OBJECT
//public:
//    OpcodeNameTableModel(QObject *parent = nullptr);

//    QVariant data(const QModelIndex& index, int role) const;
//    int rowCount(const QModelIndex &parent = QModelIndex()) const;
//    int columnCount(const QModelIndex &parent) const;

//    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
//    bool insertRows(int row, int count, const QModelIndex &parent);

//public:
//    void updatePackages();
//private:
//    DataStorage *dataStorage;
//    QStringList mTableHeaders;
//};

#endif // OPCODENAMETABLEMODEL_H
