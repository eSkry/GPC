#include "capturedpackagesmodel.h"

#include <QDebug>

#include <EthLayer.h>
#include <IPv4Layer.h>

namespace {

QString getProtocolTypeAsString(pcpp::ProtocolType protocolType) {
    switch (protocolType) {
    case pcpp::Ethernet:
        return "Ethernet";
    case pcpp::IPv4:
        return "IPv4";
    case pcpp::IPv6:
        return "IPv6";
    case pcpp::TCP:
        return "TCP";
    case pcpp::UDP:
        return "UDP";
    case pcpp::HTTPRequest:
    case pcpp::HTTPResponse:
        return "HTTP";
    case pcpp::DNS:
        return "DNS";
    case pcpp::SSL:
        return "SSL/TLS";
    case pcpp::SSH:
        return "SSH";
    case pcpp::AuthenticationHeader:
        return "AuthenticationHeader";
    default:
        return "Unknown";
    }
}

}

CapturedPackagesModel::CapturedPackagesModel(QObject *parent) : QAbstractTableModel(parent) {
    using namespace GPC;

    mTableHeaders << "No." << "Time" << "Source" << "Destination" << "Length" << "Protocol";
    columns.push_back(RecordColumns::No);
    columns.push_back(RecordColumns::DeltaTime);
    columns.push_back(RecordColumns::SourceIp);
    columns.push_back(RecordColumns::DestinationIp);
    columns.push_back(RecordColumns::LengthPackage);
    columns.push_back(RecordColumns::Protocol);

    mRawDataStorage = &RawDataStorage::instance();

    connect(mRawDataStorage, &RawDataStorage::packageAdded, this, &CapturedPackagesModel::addInsertedPackage);
}

QVariant CapturedPackagesModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant{};
    int row = index.row();

    if (row > mRawDataStorage->getRecordsCount())
        return QVariant{};

    if (role == Qt::DisplayRole) {
        auto packet = mRawDataStorage->getRecord(row);
        auto columnType = columns[index.column()];
//        auto protocolType = parsedPacket.getLastLayer()->getProtocol();
        switch (index.column()) {
        case 0:
            return row;
        case 1:
            return {};
        case 2:
        case 3:
        case 4:
            return packet->getColumnData(columnType);
        case 5: {
            return getProtocolTypeAsString(static_cast<pcpp::ProtocolType>(packet->getColumnData(GPC::RecordColumns::Protocol).toULongLong()));
        }
//            return getProtocolTypeAsString(parsedPacket.getLastLayer()->getProtocol());
        }
    }

    return QVariant{};
}

int CapturedPackagesModel::rowCount(const QModelIndex &parent) const {
    return mRawDataStorage->getRecordsCount();
}

int CapturedPackagesModel::columnCount(const QModelIndex &parent) const {
    return mTableHeaders.size();
}

QVariant CapturedPackagesModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return mTableHeaders[section];
    }

    return QVariant{};
}

bool CapturedPackagesModel::insertRows(int row, int count, const QModelIndex &parent) {
    beginInsertRows(QModelIndex(), row, row+count-1);
//    PackageData *pkg = new PackageData();
//    mPackages.push_back(pkg);
    endInsertRows();
    return true;
}

void CapturedPackagesModel::addInsertedPackage() {
    beginInsertRows(QModelIndex(), mRawDataStorage->getRecordsCount(), mRawDataStorage->getRecordsCount());
    endInsertRows();
}
