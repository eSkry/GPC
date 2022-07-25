#include "helpers.h"


bool readFileAsText(const QString &fileName, QString &outData) {
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        outData = file.readAll();
        file.close();
        return true;
    }

    return false;
}

QString readFileAsText(const QString& fileName) {
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        return file.readAll();

    return QString{};
}
