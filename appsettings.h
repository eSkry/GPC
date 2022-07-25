#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QObject>
#include <QString>

class AppSettings : public QObject {
    Q_OBJECT
signals:

    void programmPathChanged(const QString& path);
    void tsharkArgsChanged(const QString& args);

    void pkgLenghtBytesCountChanged(int count);
    void pkgLenghtBytesOffsetChanged(int offset);
    void pkgLengthBytesEndianChanged(bool littleEndian);

    void opcodeBytesCountChanged(int count);
    void opcodeBytesOffsetChanged(int offset);

public:
    AppSettings(const AppSettings&) = delete;
    AppSettings& operator=(const AppSettings&) = delete;

    explicit AppSettings(QObject *parent = nullptr);

    static AppSettings& instance() {
        static AppSettings obj(nullptr);
        return obj;
    }

    const QString& getExecutablePath() const {
        return mExecutablePath;
    }

    const QString& getTsharkParams() const {
        return mTsharkParams;
    }

public slots:

    void setProgrammPath(const QString& path);
    void setTsharkArguments(const QString& args);

    void setPkgLengthBytesCount(int count);
    void setPkgLenghtOffsetCount(int offset);
    void setPkgLengthEndian(bool littleEndian);

    void setOpcodeBytesCount(int count);
    void setOpcodeBytesOffset(int offset);

    void load();
    void save();

private:
    QString mExecutablePath;
    QString mTsharkParams;

    int mPkgLengthBytesCount; // Количество байт в пакете обозначающих его длину
    int mPkgLenghtBytesOffset; // Смещение от начала пакета до байтов в которых содержится длина пакета
    bool mPkgLengthBytesLittleEndian; // В каком endian читать байты длины

    int mOpcodeBytesCount; // Количество байт обозначающих opcode
    int mOpcodeBytesOffset; // Смещение от начала пакета до байтов в которых содержатся opcode

    QString mSettingsFilePath;
};

#endif // APPSETTINGS_H
