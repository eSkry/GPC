#include "networkdevicelist.h"


#include <QString>
#include <QDebug>

NetworkDeviceListModel::NetworkDeviceListModel(QObject *parent)
    : QAbstractListModel{parent} {

}

QVariant NetworkDeviceListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() > mDeviceList.size())
        return {};

    auto item = mDeviceList[index.row()];
    if (role == Qt::DisplayRole) {
        return QString("%1 %2").arg(item->getName().c_str()).arg(item->getDesc().c_str());
    } else if (role == Qt::UserRole) {
        return item->getIPv4Address().toString().c_str();
    }


    return QVariant{};
}

int NetworkDeviceListModel::rowCount(const QModelIndex &parent) const {
    return mDeviceList.size();
}

void NetworkDeviceListModel::updateList() {
    mDeviceList = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList();
}
