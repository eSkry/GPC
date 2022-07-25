#ifndef IGNORELISTTABLEMODEL_H
#define IGNORELISTTABLEMODEL_H

#include <QAbstractTableModel>
#include <QModelIndex>
#include <vector>

#include "PackageData.h"

/**
 * @brief The PackageDataTableModel class
 */
//class IgnoreListTableModel : public QAbstractTableModel {
//    Q_OBJECT
//public:
//    explicit IgnoreListTableModel(QObject *parent = nullptr);

//    QVariant data(const QModelIndex& index, int role) const;
//    int rowCount(const QModelIndex &parent = QModelIndex()) const;
//    int columnCount(const QModelIndex &parent) const;

//    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
//    //добавление строки
//    bool insertRows(int row, int count, const QModelIndex &parent);

//public slots:
//    void updatePackages();

//public:
//    QStringList mTableHeaders;
//    DataStorage *pkgStorage;
//};

#endif // IGNORELISTTABLEMODEL_H
