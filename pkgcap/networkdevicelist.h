#ifndef NETWORKDEVICELIST_H
#define NETWORKDEVICELIST_H

#include <PcapLiveDeviceList.h>

#include <QObject>
#include <QAbstractListModel>

#include <vector>

/**
 * @brief The NetworkDeviceListModel class - Модель для получения списка устройств для захвата.
 */
class NetworkDeviceListModel : public QAbstractListModel {
    Q_OBJECT
public:
    explicit NetworkDeviceListModel(QObject *parent = nullptr);

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual int rowCount( const QModelIndex & parent = QModelIndex() ) const;

public slots:
    void updateList();

private:
    std::vector<pcpp::PcapLiveDevice*> mDeviceList;
};

#endif // NETWORKDEVICELIST_H
