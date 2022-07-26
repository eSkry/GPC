#include "packetlistrecord.h"

#include <IPv4Layer.h>
#include <IPv6Layer.h>
#include <EthLayer.h>
#include <TcpLayer.h>
#include <UdpLayer.h>

GPC::PacketRecord::PacketRecord(pcpp::RawPacket* rawPacket) {
    _rawParsedPacket = pcpp::Packet(rawPacket);

    auto lastLayer = _rawParsedPacket.getLastLayer();
    if (!lastLayer)
        return;

    auto prevLastLayer = lastLayer->getPrevLayer();
    if (!prevLastLayer)
        return;

    QVariant layerProtocol = (unsigned long long)prevLastLayer->getProtocol();
    setColumnData(layerProtocol, RecordColumns::Protocol);

    if (_rawParsedPacket.isPacketOfType(pcpp::IP)) {
        auto layer = _rawParsedPacket.getLayerOfType<pcpp::IPv4Layer>();

        QVariant sourceIp = QString(layer->getSrcIPAddress().toString().c_str());
        setColumnData(sourceIp, RecordColumns::SourceIp);

        QVariant destIp = QString(layer->getDstIPAddress().toString().c_str());
        setColumnData(destIp, RecordColumns::DestinationIp);
    }

    QVariant pkgLen = (unsigned long long)prevLastLayer->getDataLen();
    setColumnData(pkgLen, RecordColumns::LengthPackage);

    if (_rawParsedPacket.isPacketOfType(pcpp::TCP)
            || _rawParsedPacket.isPacketOfType(pcpp::UDP)) {
        auto layer = _rawParsedPacket.getLayerOfType<pcpp::TcpLayer>();

        QVariant portSrc = layer->getSrcPort();
        setColumnData(portSrc, RecordColumns::SourcePort);

        QVariant portDst = layer->getDstPort();
        setColumnData(portDst, RecordColumns::DestinationPort);

        QVariant portDir = QString::number(layer->getSrcPort()) + " -> " + QString::number(layer->getDstPort());
        setColumnData(portDir, RecordColumns::PortDirection);
    }
}

pcpp::RawPacket *GPC::PacketRecord::rawPacketReadOnly() {
    return _rawParsedPacket.getRawPacketReadOnly();
}

pcpp::Packet *GPC::PacketRecord::getParsedPacket() {
    return &_rawParsedPacket;
}

const QVariant& GPC::PacketRecord::getColumnData(RecordColumns column) {
    return _columns[column].getData();
}

void GPC::PacketRecord::setColumnData(QVariant &newData, RecordColumns columnType) {
    _columns[columnType].setData(newData);
}

GPC::RecordColumnDataType GPC::RecordColumn::getDataTypeByColumn(RecordColumns col) {
    switch (col) {
    case RecordColumns::No:
    case RecordColumns::LengthPackage:
    case RecordColumns::Protocol:
    case RecordColumns::SourcePort:
    case RecordColumns::DestinationPort:
        return RecordColumnDataType::Int;
    case RecordColumns::SourceIp:
    case RecordColumns::DestinationIp:
    case RecordColumns::PortDirection:
    case RecordColumns::Info:
        return RecordColumnDataType::String;
    case RecordColumns::DeltaTime:
        return RecordColumnDataType::Double;

    case RecordColumns::__COLUMNS_COUNT:
        return RecordColumnDataType::InvalidType;
    }

    return RecordColumnDataType::InvalidType;
}

const QString &GPC::RecordColumn::getColumnName(RecordColumns col) {
    static QString defaultStr("Unknow column name");

    // Должно соответствовать enum RecordColumns
    static QStringList colNames = QStringList()
            << "No."
            << "DeltaTime"

            << "Source"
            << "Destination"

            << "Length"
            << "Protocol"

            << "Src port"
            << "Dst port"
            << "Port dir"

            << "Info";

    if (col >= RecordColumns::__COLUMNS_COUNT)
        return defaultStr;

    return colNames[(int)col];
}

GPC::RecordColumn::RecordColumn(const QVariant &newData, RecordColumns colType) {
    columnType = colType;
    data = newData;
}

GPC::RecordColumn::RecordColumn() {
    columnType = RecordColumns::Unknow;
}

GPC::RecordColumnDataType GPC::RecordColumn::getDataType() {
    return getDataTypeByColumn(columnType);
}

GPC::RecordColumns GPC::RecordColumn::getColumnType() {
    return columnType;
}

const QString &GPC::RecordColumn::getColumnName() {
    return getColumnName(columnType);
}

void GPC::RecordColumn::setData(const QVariant newData) {
    data = newData;
}

const QVariant &GPC::RecordColumn::getData() {
    return data;
}
