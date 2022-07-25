#ifndef SELECTEDOPCODEMODEL_H
#define SELECTEDOPCODEMODEL_H


#include <QAbstractTableModel>
#include <QModelIndex>
#include <map>

#include "PackageData.h"


class SelectedOpcodeModel : public QAbstractTableModel {
    Q_OBJECT
public:
    SelectedOpcodeModel(const QString& opcode, QObject *parent = nullptr);

    QVariant data(const QModelIndex& index, int role) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    //добавление строки
    bool insertRows(int row, int count, const QModelIndex &parent);

public slots:
    void updatePackages(const QString& opcode);
private:
    QStringList mTableHeaders;
    DataStorage *pkgStorage;

    QString mOpcode;
};

#endif // SELECTEDOPCODEMODEL_H
