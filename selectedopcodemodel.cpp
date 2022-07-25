#include "selectedopcodemodel.h"

SelectedOpcodeModel::SelectedOpcodeModel(const QString &opcode, QObject *parent) {
    mTableHeaders << "opcode" << "length" << "rec time" << "data";
    mOpcode = opcode;


    pkgStorage = &DataStorage::instance();
    connect(pkgStorage, &DataStorage::addedBDOPackage2, this, &SelectedOpcodeModel::updatePackages);
}

QVariant SelectedOpcodeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant{};
    int row = index.row();

    auto &keysList = pkgStorage->getPackageKeyListForOpcode(mOpcode);

    if (row > keysList.size())
        return QVariant{};

    auto rowFromDataStorage = keysList[row];
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case 0:
                return pkgStorage->getPackageListItem(rowFromDataStorage).opcode;
            case 1:
                return pkgStorage->getPackageListItem(rowFromDataStorage).pkgLen;
            case 2:
                return pkgStorage->getPackageListItem(rowFromDataStorage).timestamp.toString("HH:mm:ss");
            case 3:
                return pkgStorage->getPackageListItem(rowFromDataStorage).data;

        }
    } /*else if (role == Qt::BackgroundRole) {
        return pkgStorage->getPackageInfoItem(pkgStorage->getOpcodeByIdx(row)).color;
    }*/

    return QVariant{};
}

int SelectedOpcodeModel::rowCount(const QModelIndex &parent) const {
    return pkgStorage->getPackageKeyListForOpcode(mOpcode).size();
}

int SelectedOpcodeModel::columnCount(const QModelIndex &parent) const {
    return mTableHeaders.size();
}

QVariant SelectedOpcodeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return mTableHeaders[section];
    }

    return QVariant();
}

bool SelectedOpcodeModel::insertRows(int row, int count, const QModelIndex &parent) {
    beginInsertRows(QModelIndex(), row, row+count-1);
//    PackageData *pkg = new PackageData();
//    mPackages.push_back(pkg);
    endInsertRows();
    return true;
}

void SelectedOpcodeModel::updatePackages(const QString& opcode) {
    if (mOpcode == opcode) {
        beginInsertRows(QModelIndex(), pkgStorage->getPackageKeyListForOpcode(mOpcode).size(), pkgStorage->getPackageKeyListForOpcode(mOpcode).size());
        endInsertRows();
    }
}
