#ifndef PACKETLISTRECORD_H
#define PACKETLISTRECORD_H

#include <QString>
#include <QStringList>
#include <QVariant>

#include <Packet.h>
#include <RawPacket.h>

#include <vector>
#include <map>

namespace GPC {

enum RecordColumns : int {
    No = 0,
    DeltaTime,

    /* Addr */
    SourceIp,
    DestinationIp,

    LengthPackage,
    Protocol,

    /* PORT */
    SourcePort,
    DestinationPort,
    PortDirection, // Строка вида: portFrom -> portTo

    Info,

    __COLUMNS_COUNT,
    Unknow
};

enum RecordColumnDataType : int {
    InvalidType = -1,

    Int = 0,
    String,
    Double,
    Binary,

    __TYPES_COUNT
};


struct RecordColumn {
    static RecordColumnDataType getDataTypeByColumn(RecordColumns col);
    static const QString& getColumnName(RecordColumns col);

    RecordColumn(const QVariant& data, RecordColumns colType);
    RecordColumn();

    RecordColumnDataType getDataType();
    RecordColumns getColumnType();
    const QString& getColumnName();

    void setData(const QVariant newData);
    const QVariant& getData();

private:
    RecordColumns columnType;

    QVariant data;
};


/**
 * @brief The PacketRecord class
 */
class PacketRecord {
public:
    PacketRecord(pcpp::RawPacket* rawPacket);

    pcpp::RawPacket* rawPacketReadOnly();
    pcpp::Packet* getParsedPacket();

    const QVariant& getColumnData(RecordColumns columnType);
    void setColumnData(QVariant& newData, RecordColumns columnType);

private:
    std::map<RecordColumns, RecordColumn> _columns;

    pcpp::Packet _rawParsedPacket;
};

using PacketRecordList = std::vector<PacketRecord>;

}


#endif // PACKETLISTRECORD_H
