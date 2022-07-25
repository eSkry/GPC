#include "ignorelisttablemodel.h"

//IgnoreListTableModel::IgnoreListTableModel(QObject *parent) : QAbstractTableModel(parent) {
//    pkgStorage = &DataStorage::instance();

//    mTableHeaders << "opcode" << "name";

////    connect(pkgIgnoreStorage, &IgnorePackagesStorage::ignoreOpcodeAdded, this, &IgnoreListTableModel::updatePackages);
////    connect(pkgIgnoreStorage, &IgnorePackagesStorage::ignoreOpcodeRemoved, this, &IgnoreListTableModel::updatePackages);
//}

//QVariant IgnoreListTableModel::data(const QModelIndex &index, int role) const {
//    if (!index.isValid())
//        return QVariant{};
//    int row = index.row();

//    if (row > pkgIgnoreStorage->getIgnoreOpcodesCount())
//        return QVariant{};

//    if (role == Qt::DisplayRole) {
//        switch (index.column()) {
//            case 0:
//                return pkgIgnoreStorage->getIgnoreOpcodeByIdx(row);
//            case 1:
//                return pkgNamesStorage->getNameByOpcode(pkgIgnoreStorage->getIgnoreOpcodeByIdx(row));
//        }
//    }

//    return QVariant{};
//}

//int IgnoreListTableModel::rowCount(const QModelIndex &parent) const {
//    return pkgIgnoreStorage->getIgnoreOpcodesCount();
//}

//int IgnoreListTableModel::columnCount(const QModelIndex &parent) const {
//    return mTableHeaders.size();
//}

//QVariant IgnoreListTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
//    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
//        return mTableHeaders[section];
//    }

//    return QVariant();
//}

//bool IgnoreListTableModel::insertRows(int row, int count, const QModelIndex &parent) {
//    beginInsertRows(QModelIndex(), row, row+count-1);
////    PackageData *pkg = new PackageData();
////    mPackages.push_back(pkg);
//    endInsertRows();
//    return true;
//}

//void IgnoreListTableModel::updatePackages() {
//    qDebug() << "IgnoreListTableModel::updatePackages";
//    beginInsertRows(QModelIndex(), pkgIgnoreStorage->getIgnoreOpcodesCount(), pkgIgnoreStorage->getIgnoreOpcodesCount());
//    endInsertRows();
//}
