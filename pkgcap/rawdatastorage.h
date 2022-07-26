#ifndef RAWDATASTORAGE_H
#define RAWDATASTORAGE_H

#include <QObject>
#include <QString>

#include <string>
#include <vector>
#include <mutex>

#include <PcapFileDevice.h>
#include <Packet.h>

#include "../packetlistrecord.h"

class RawDataStorage : public QObject {
    Q_OBJECT

signals:
    void packageAdded();
    void packagesAded(uint count);

public:
    RawDataStorage(const RawDataStorage&) = delete;
    RawDataStorage& operator=(const RawDataStorage&) = delete;

    static RawDataStorage& instance();

    explicit RawDataStorage(QObject *parent = nullptr);

    std::size_t getRecordsCount();
    GPC::PacketRecord* getRecord(std::size_t index);

    void addPacket(GPC::PacketRecord * packet);

public slots:

    void loadPcapngFile(const QString& file);
    void savePcapngFile(const QString& file);

private:
    std::vector<GPC::PacketRecord *> _records;

    std::mutex mMutex;
};



#endif // RAWDATASTORAGE_H
