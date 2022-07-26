#include "packagecapture.h"

#include <PcapLiveDeviceList.h>

#include <QDebug>
#include <iostream>

namespace {

static std::mutex recieverMutex;
static void onPacketRecieved(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* myStorage) {
    std::scoped_lock<std::mutex> lock(recieverMutex);

    RawDataStorage* storage = static_cast<RawDataStorage*>(myStorage);

    GPC::PacketRecord *record = new GPC::PacketRecord(packet);

    storage->addPacket(record);
}

}

PackageCapture::PackageCapture(QObject *parent)
    : QObject{parent} {

    mRawStorage = &RawDataStorage::instance();

    mPcapDevice = nullptr;
    mIsStarted = false;
}

bool PackageCapture::isStarted() const {
    return mIsStarted;
}

void PackageCapture::startCapture() {
    if (!mPcapDevice)
        mPcapDevice = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByIpOrName(mPcapDeviceName.toStdString());

    if (mPcapDevice) {
        if (!mPcapDevice->open()) {
            qDebug() << "Device open error. IP " << mPcapDevice->getIPv4Address().toString().c_str()
                     << " Name: " << mPcapDevice->getName().c_str();
            return;
        }

        mPcapDevice->setFilter(mPcapFilter.toStdString());
        mPcapDevice->startCapture(onPacketRecieved, mRawStorage);
        mIsStarted = true;
        emit stateChanged(true);
    } else {
        std::cerr << "PackageCapture::startCapture: PcapLiveDevice is null\n";
    }
}

void PackageCapture::stopCapture() {
    if (mPcapDevice) {
        mPcapDevice->stopCapture();
        mPcapDevice->close();
        mIsStarted = false;
        emit stateChanged(false);
    }
}

void PackageCapture::setPcapDeviceByIpOrName(const QString &ipOrName) {
    qInfo() << "PackageCapture::setPcapDeviceByIpOrName: " << ipOrName;

    mPcapDeviceName = ipOrName;

    if (mPcapDevice) {
        mPcapDevice->stopCapture();
        mPcapDevice = nullptr;
    }
}

void PackageCapture::setPcapFilter(const QString &filter) {
    mPcapFilter = filter;
}
