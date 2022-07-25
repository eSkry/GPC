#include "packagedatatotalmodel.h"

#include <QDebug>

PackageDataTotalModel::PackageDataTotalModel(QObject *parent) : QAbstractTableModel(parent){
    mTableHeaders << "opcode" << "name" << "count" << "last time";

    pkgStorage = &DataStorage::instance();

    connect(pkgStorage, &DataStorage::pkgInfoUpdated, this, &PackageDataTotalModel::updatePackages);
}

QVariant PackageDataTotalModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant{};
    int row = index.row();
    if (row > pkgStorage->getPackageInfoListSize())
        return QVariant{};

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case 0:
                return pkgStorage->getOpcodeByIdx(row);
            case 1:
                return pkgStorage->getPackageInfoItem(pkgStorage->getOpcodeByIdx(row)).name;
            case 2:
                return pkgStorage->getPackageInfoItem(pkgStorage->getOpcodeByIdx(row)).count;
            case 3:
                return pkgStorage->getPackageInfoItem(pkgStorage->getOpcodeByIdx(row)).lastTimestamp.toString("HH:mm:ss");

        }
    } else if (role == Qt::BackgroundRole) {
        return pkgStorage->getPackageInfoItem(pkgStorage->getOpcodeByIdx(row)).color;
    }

    return QVariant{};
}

int PackageDataTotalModel::rowCount(const QModelIndex &parent) const {
    return pkgStorage->getPackageInfoListSize();
}

int PackageDataTotalModel::columnCount(const QModelIndex &parent) const {
    return mTableHeaders.size();
}

QVariant PackageDataTotalModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return mTableHeaders[section];
    }

    return QVariant();
}

bool PackageDataTotalModel::insertRows(int row, int count, const QModelIndex &parent) {
    beginInsertRows(QModelIndex(), row, row+count-1);
//    PackageData *pkg = new PackageData();
//    mPackages.push_back(pkg);
    endInsertRows();
    return true;
}

void PackageDataTotalModel::updatePackages() {
    qDebug() << "PackageDataTotalModel::updatePackages";
    beginInsertRows(QModelIndex(), pkgStorage->getPackageInfoListSize(), pkgStorage->getPackageInfoListSize());
    endInsertRows();
}




















