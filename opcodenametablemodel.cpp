#include "opcodenametablemodel.h"

//OpcodeNameTableModel::OpcodeNameTableModel(QObject *parent) : QAbstractTableModel(parent) {
//    mTableHeaders << "opcode" << "name";
//    dataStorage = &DataStorage::instance();

//    connect(dataStorage, &OpcodeParamsStorage::nameAdded, this, &OpcodeNameTableModel::updatePackages);
//}

//QVariant OpcodeNameTableModel::data(const QModelIndex &index, int role) const {
//    if (!index.isValid())
//        return QVariant{};
//    int row = index.row();

//    if (row > opcodeSettingsStorage->getSettingsCount())
//        return QVariant{};

//    auto &opNames = OpcodeParamsStorage::instance();

//    if (role == Qt::DisplayRole) {
//        switch (index.column()) {
//            case 0:
//                return opcodeSettingsStorage->getOpcodeByIdx(row);
//            case 1:
//                return opcodeSettingsStorage->getNameByIdx(row);
//        }
//    }

//    return QVariant{};
//}

//int OpcodeNameTableModel::rowCount(const QModelIndex &parent) const {
//    return opcodeSettingsStorage->getSettingsCount();
//}

//int OpcodeNameTableModel::columnCount(const QModelIndex &parent) const {
//    return mTableHeaders.size();
//}

//QVariant OpcodeNameTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
//    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
//        return mTableHeaders[section];
//    }

//    return QVariant();
//}

//bool OpcodeNameTableModel::insertRows(int row, int count, const QModelIndex &parent) {
//    beginInsertRows(QModelIndex(), row, row+count-1);
////    PackageData *pkg = new PackageData();
////    mPackages.push_back(pkg);
//    endInsertRows();
//    return true;
//}

//void OpcodeNameTableModel::updatePackages() {
//    beginInsertRows(QModelIndex(), opcodeSettingsStorage->getSettingsCount(), opcodeSettingsStorage->getSettingsCount());
//    endInsertRows();
//}
