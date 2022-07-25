#include "packagedatatablemodel.h"

#include <QDebug>

PackageDataTableModel::PackageDataTableModel(QObject *parent) : QAbstractTableModel(parent){
    mTableHeaders << "opcode" << "name" << "length" << "rec time" << "data";
    pkgStorage = &DataStorage::instance();

    connect(pkgStorage, &DataStorage::addedBDOPackage, this, &PackageDataTableModel::updatePackages);
    connect(pkgStorage, &DataStorage::cleanedBDOPackages, [this]() {
        beginRemoveRows({}, 0, pkgStorage->getPackageListSize());
        endRemoveRows();
    });
}

QVariant PackageDataTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant{};
    int row = index.row();

    if (row > pkgStorage->getPackageListSize())
        return QVariant{};

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case 0:
                return pkgStorage->getPackageListItem(row).opcode;
            case 1:
                return pkgStorage->getPackageInfoItem(pkgStorage->getPackageListItem(row).opcode).name;
            case 2:
                return pkgStorage->getPackageListItem(row).pkgLen;
            case 3:
                return pkgStorage->getPackageListItem(row).timestamp.toString("HH:mm:ss");
            case 4:
                return pkgStorage->getPackageListItem(row).data;
        }
    } else if (role == Qt::BackgroundRole) {
        return pkgStorage->getPackageInfoItem(pkgStorage->getPackageListItem(row).opcode).color;
    }

    return QVariant{};
}

int PackageDataTableModel::rowCount(const QModelIndex &parent) const {
    return DataStorage::instance().getPackageListSize();
}

int PackageDataTableModel::columnCount(const QModelIndex &parent) const {
    return mTableHeaders.size();
}

QVariant PackageDataTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return mTableHeaders[section];
    }

    return QVariant();
}

bool PackageDataTableModel::insertRows(int row, int count, const QModelIndex &parent) {
    beginInsertRows(QModelIndex(), row, row+count-1);
//    PackageData *pkg = new PackageData();
//    mPackages.push_back(pkg);
    endInsertRows();
    return true;
}

void PackageDataTableModel::updatePackages() {
//    emit dataChanged(QModelIndex(), QModelIndex());
    beginInsertRows(QModelIndex(), pkgStorage->getPackageListSize(), pkgStorage->getPackageListSize());
    endInsertRows();
}
