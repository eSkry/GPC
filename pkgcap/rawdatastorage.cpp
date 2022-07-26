#include "rawdatastorage.h"

#include <QDebug>

#include <iostream>

RawDataStorage::RawDataStorage(QObject *parent)
    : QObject{parent} {

    _records.reserve(4096 * 4);
}

std::size_t RawDataStorage::getRecordsCount() {
    std::scoped_lock<std::mutex> lock(mMutex);
    return _records.size();
}

GPC::PacketRecord *RawDataStorage::getRecord(std::size_t index) {
    return _records[index];
}

void RawDataStorage::addPacket(GPC::PacketRecord *packet) {
    _records.push_back(packet);
    emit packageAdded();
}

RawDataStorage &RawDataStorage::instance() {
    static RawDataStorage obj(nullptr);
    return obj;
}

void RawDataStorage::loadPcapngFile(const QString &file) {
    std::scoped_lock<std::mutex> lock(mMutex);

//    qDebug() << "RawDataStorage::loadPcapngFile: " << file;

//    pcpp::IFileReaderDevice* reader = pcpp::IFileReaderDevice::getReader(file.toStdString());

//    if (reader == nullptr) {
//        qInfo() << "Cannot determine reader for file type";
//        return;
//    }

//    if (!reader->open()) {
//        qInfo() << "Cannot open " << file << " for reading";
//        return;
//    }

//    uint count = 0;
//    pcpp::RawPacketVector a;
//    pcpp::RawPacket rawPacket;
//    while (reader->getNextPacket(rawPacket)) {
//        mRawPackages.push_back(rawPacket);
//        mParsetPackages.push_back(pcpp::Packet(&rawPacket));
//        ++count;
//        emit packageAdded();
//    }

//    emit packagesAded(count);
}

void RawDataStorage::savePcapngFile(const QString &file) {
    std::scoped_lock<std::mutex> lock(mMutex);

//    pcpp::PcapNgFileWriterDevice writer(file.toStdString());

//    if (!writer.open()) {
//        qInfo() << "Cannot open output.pcapng for writing";
//        return;
//    }

//    for (auto &pkg : mRawPackages) {
//        writer.writePacket(pkg);
//    }

//    writer.close();
}

