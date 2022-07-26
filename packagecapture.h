#ifndef PACKAGECAPTURE_H
#define PACKAGECAPTURE_H

#include <QObject>

#include <PcapLiveDevice.h>

#include "pkgcap/rawdatastorage.h"
#include "packetlistrecord.h"

class PackageCapture : public QObject {
    Q_OBJECT
public:
    explicit PackageCapture(QObject *parent = nullptr);

    bool isStarted() const;

public slots:

    void startCapture();
    void stopCapture();

    void setPcapDeviceByIpOrName(const QString& ipOrName);
    void setPcapFilter(const QString& filter);

signals:

    void stateChanged(bool isStarted);

private:
    bool mIsStarted;
    RawDataStorage *mRawStorage;
    QString mPcapFilter;
    QString mPcapDeviceName;

    pcpp::PcapLiveDevice *mPcapDevice;
};

#endif // PACKAGECAPTURE_H
