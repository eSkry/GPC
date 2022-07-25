#ifndef PACKAGEDATA_H
#define PACKAGEDATA_H

#include <QString>
#include <QColor>
#include <QObject>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <memory>
#include <vector>
#include <map>
#include <list>
#include <mutex>
#include <QDateTime>



struct BDOPkgData {
    QDateTime       timestamp;
    QString         opcode;
    QString         data;
    uint            pkgLen;
};

struct PkgInfo {
    PkgInfo() {
        color = QColor("#a28415");
        count = 0;
        ignore = false;
    }
    PkgInfo(const QString& name, const QColor& color = QColor("#a28415"), bool ignore = false, uint count = 0)
        : name{name}
        , color{color}
        , ignore{ignore}
        , count{count} {

    }

    QString         name;
    QColor          color;
    QDateTime       lastTimestamp;
    uint            count;
    bool            ignore;
};

class DataStorage : public QObject {
    Q_OBJECT

signals:

    void addedBDOPackage();
    void addedBDOPackage2(const QString& opcode);
    void cleanedBDOPackages();
    void pkgInfoUpdated();

public:
    DataStorage(const DataStorage&) = delete;
    DataStorage& operator=(const DataStorage&) = delete;

    static DataStorage& instance() {
        static DataStorage obj(nullptr);
        return obj;
    }

    DataStorage(QObject *parent) : QObject(parent) {
        mPkgList.reserve(4096 * 2);
        defaultPkg.opcode = "X";
        defaultPkg.pkgLen = -1;
        defaultPkg.data = "Обращение к несуществующему обьекту BDOPkgData";
    }

public slots:

    void addPackageListItem(const BDOPkgData& pkg) {
        bool needUpdateOpcodeList = false;

        {
            std::scoped_lock<std::mutex> lock(mMutex);

            mPkgList.push_back(pkg);

            auto &groupedIdx = mGroupedPackagesByOpcodeKeyList[pkg.opcode];
            if (groupedIdx.size() == 0)
                groupedIdx.reserve(1024);

            groupedIdx.push_back(mPkgList.size() - 1); // Index of current record

            auto &pkgInfo = mPkgInfo[pkg.opcode];
            pkgInfo.count++;
            pkgInfo.lastTimestamp = pkg.timestamp;

            needUpdateOpcodeList = mPkgInfo.size() != mOpcodeList.size();
            if (needUpdateOpcodeList) { // Если появился новый opcode
                mOpcodeList.clear();
                std::size_t idx = 0;
                for (auto it = mPkgInfo.begin(); it != mPkgInfo.end(); it++, idx++)
                    mOpcodeList[idx] = it->first; // Жирно но пока так
            }
        }

        if (needUpdateOpcodeList)
            emit pkgInfoUpdated();

        emit addedBDOPackage();
        emit addedBDOPackage2(mPkgList[mPkgList.size() -1].opcode);
    }

    void clearPackageList() {
        {
            std::scoped_lock<std::mutex> lock(mMutex);
            mPkgList.clear();
        }

        emit cleanedBDOPackages();
    }

    void setNamePackage(const QString& opcode, const QString& name) {
        std::scoped_lock<std::mutex> lock(mMutex);

        mPkgInfo[opcode].name = name;
    }

public:

    std::size_t getPackageListSize() {
        std::lock_guard<std::mutex> lock(mMutex);
        return mPkgList.size();
    }

    BDOPkgData& getPackageListItem(std::size_t i) {
        if (i >= mPkgList.size())
            return defaultPkg;

        return mPkgList[i];
    }

    /////////////////////////////////////////////////////////

    std::size_t getOpcodeListSize() {
        std::lock_guard<std::mutex> lock(mMutex);
        return mOpcodeList.size();
    }

    const QString& getOpcodeByIdx(std::size_t idx) {
        return mOpcodeList[idx];
    }

    /////////////////////////////////////////////////////////

    std::size_t getPackageInfoListSize() {
        return mPkgInfo.size();
    }

    PkgInfo& getPackageInfoItem(const QString& opcode) {
        return mPkgInfo[opcode];
    }

    PkgInfo& getPackageInfoItem(std::size_t packageListIndex) {
        auto &opcode = mPkgList[packageListIndex].opcode;
        return mPkgInfo[opcode];
    }

    /////////////////////////////////////////////////////////

    bool isPakcageIgnored(const QString& opcode) {
        return mPkgInfo[opcode].ignore;
    }

    /////////////////////////////////////////////////////////

    const std::vector<std::size_t>& getPackageKeyListForOpcode(const QString& opcode) {
        return mGroupedPackagesByOpcodeKeyList[opcode];
    }

private:
    BDOPkgData defaultPkg; // Затыка для out of bounds

    std::vector<BDOPkgData>             mPkgList;    /* packages */
    std::map<std::size_t, QString>      mOpcodeList; /* index:opcode */
    std::map<QString, PkgInfo>          mPkgInfo;    /* opcode:data */

    std::map<QString, std::vector<std::size_t>> mGroupedPackagesByOpcodeKeyList; /* opcode:index list from mPkgList  */

    std::mutex mMutex;
};


class DataLoader : public QObject {
    Q_OBJECT

signals:

public:
    DataLoader(QObject *parent) : QObject(parent) {

    }

    static DataLoader& instance() {
        static DataLoader obj(nullptr);
        return obj;
    }

    void save() {
        DataStorage &pkgStorage = DataStorage::instance();

        QJsonArray jsonParamsStorage;

        { // paramStorage
            auto count = pkgStorage.getPackageInfoListSize();
            for (int i = 0; i < count; i++) {
                QJsonObject obj;
                auto opcode = pkgStorage.getOpcodeByIdx(i);
                obj.insert("opcode", opcode);
                auto &infoItem = pkgStorage.getPackageInfoItem(opcode);
                obj.insert("color", infoItem.color.name());
                obj.insert("name", infoItem.name);
                obj.insert("ingnore", infoItem.ignore);
                jsonParamsStorage.push_back(obj);
            }
        }

        QJsonObject mainJson;
        mainJson.insert("params_storage", jsonParamsStorage);

        QFile file;
        file.setFileName("./pkg_settings.json");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QJsonDocument doc(mainJson);

        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
    }

    void load() {
        QFile file;
        file.setFileName("./pkg_settings.json");
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QString data = file.readAll();
        file.close();

        if (data.isEmpty() || data.isNull())
            return;

        QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject json = doc.object();

        if (json.contains("params_storage")) {
        DataStorage &pkgStorage = DataStorage::instance();
            QJsonArray jsonParamsStorage = json["params_storage"].toArray();

            for (int i = 0; i < jsonParamsStorage.size(); i++) {
                auto opcode = jsonParamsStorage.at(i)["opcode"].toString();
                auto &pkg = pkgStorage.getPackageInfoItem(opcode);
                pkg.name = jsonParamsStorage.at(i)["name"].toString();
                pkg.ignore = jsonParamsStorage.at(i)["ignore"].toBool();

                auto color = jsonParamsStorage.at(i)["color"].toString();
                if (color.isEmpty() || color.isNull()) {
                    pkg.color = Qt::transparent;
                } else {
                    pkg.color = QColor(jsonParamsStorage.at(i)["color"].toString());
                }
            }
        }
    }

private:

};

#endif // PACKAGEDATA_H
























