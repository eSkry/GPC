#include "appsettings.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

AppSettings::AppSettings(QObject *parent)
    : QObject{parent} {

    mSettingsFilePath = "./app_settings.json";

    setPkgLengthBytesCount(2);
    setPkgLenghtOffsetCount(0);
    setPkgLengthEndian(true);

    setOpcodeBytesCount(2);
    setOpcodeBytesOffset(2);
}

void AppSettings::setProgrammPath(const QString &path) {
    mExecutablePath = path;

    emit programmPathChanged(mExecutablePath);
}

void AppSettings::setTsharkArguments(const QString &args) {
    mTsharkParams = args;

    emit tsharkArgsChanged(mTsharkParams);
}

void AppSettings::setPkgLengthBytesCount(int count) {
    mPkgLengthBytesCount = count;

    emit pkgLenghtBytesCountChanged(count);
}

void AppSettings::setPkgLenghtOffsetCount(int offset) {
    mPkgLenghtBytesOffset = offset;

    emit pkgLenghtBytesOffsetChanged(offset);
}

void AppSettings::setPkgLengthEndian(bool littleEndian) {
    mPkgLengthBytesLittleEndian = littleEndian;

    emit pkgLengthBytesEndianChanged(littleEndian);
}

void AppSettings::setOpcodeBytesCount(int count) {
    mOpcodeBytesCount = count;

    emit opcodeBytesCountChanged(count);
}

void AppSettings::setOpcodeBytesOffset(int offset) {
    mOpcodeBytesOffset = offset;

    emit opcodeBytesOffsetChanged(offset);
}

void AppSettings::load() {
    QFile file;
    file.setFileName(mSettingsFilePath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
    auto json = doc.object();

    if (json.contains("executable_path"))
        setProgrammPath(json["executable_path"].toString());

    if (json.contains("tshark_args"))
        setTsharkArguments(json["tshark_args"].toString());
    else
        setTsharkArguments("-i 1 -Y \"(tcp && tcp.payload && tcp.port == 8889\" -T fields -e data");

    if (json.contains("pkg_length_bytes_count"))
        setPkgLengthBytesCount(json["pkg_length_bytes_count"].toInt());

    if (json.contains("pkg_lenght_offset_count"))
        setPkgLenghtOffsetCount(json["pkg_lenght_offset_count"].toInt());

    if (json.contains("pkg_lenght_little_endian"))
        setPkgLengthEndian(json["pkg_lenght_little_endian"].toBool());

    if (json.contains("opcode_bytes_count"))
        setOpcodeBytesCount(json["opcode_bytes_count"].toInt());

    if (json.contains("opcode_bytes_offset"))
        setOpcodeBytesOffset(json["opcode_bytes_offset"].toInt());
}

void AppSettings::save() {
    QJsonObject json;

    json["executable_path"] = mExecutablePath;
    json["tshark_args"] = mTsharkParams;
    json["pkg_length_bytes_count"] = mPkgLengthBytesCount;
    json["pkg_lenght_offset_count"] = mPkgLenghtBytesOffset;
    json["pkg_lenght_little_endian"] = mPkgLengthBytesLittleEndian;
    json["opcode_bytes_count"] = mOpcodeBytesCount;
    json["opcode_bytes_offset"] = mOpcodeBytesOffset;

    QJsonDocument doc(json);

    QFile file;
    file.setFileName(mSettingsFilePath);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
}
