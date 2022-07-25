#ifndef HELPERS_H
#define HELPERS_H

#include <QFile>
#include <QString>


bool readFileAsText(const QString& file, QString& outData);
QString readFileAsText(const QString& file);

#endif // HELPERS_H
