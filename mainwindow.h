#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QSqlQueryModel>

#include "tsharkwrapper.h"
#include "PackageData.h"
#include "packagedatatablemodel.h"
#include "ignorelisttablemodel.h"
#include "opcodenametablemodel.h"
#include "packagedatatotalmodel.h"

#include "appsettings.h"

#include "errorswindow.h"
#include "packagedetailwindow.h"
#include "appsettingswindow.h"
#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:

private slots:

    void tsharkStart();
    void tsharkStopped();

private:
    Ui::MainWindow *ui;
    quint64 mPkgCount;
    TsharkWrapperPtr tsharkPtr;
    AppSettings *appSettings;

    DataStorage *pkgStorage;
    PackageDataTableModel *pkgModel;
    PackageDataTotalModel *pkgTotalModel;
    QSqlTableModel *sqlModel;

    ErrorsWindow *errorsWindow;
    AppSettingsWindow *appSettingsWindow;
    std::map<QString, PackageDetailWindow*> mPackageDetailWindows;

    DataBase *mDataBase;
};
#endif // MAINWINDOW_H
