#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QSqlQueryModel>

#include "packagedatatablemodel.h"
#include "ignorelisttablemodel.h"
#include "opcodenametablemodel.h"
#include "packagedatatotalmodel.h"
#include "models/capturedpackagesmodel.h"

#include "appsettings.h"
#include "pkgcap/networkdevicelist.h"

#include "packagedetailwindow.h"
#include "appsettingswindow.h"
#include "pkgcap/rawdatastorage.h"
#include "packagecapture.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:

private:
    Ui::MainWindow *ui;
    AppSettings *appSettings;

    NetworkDeviceListModel *networkDeviceListModel;
    CapturedPackagesModel *capturedPackagesModel;

    AppSettingsWindow *appSettingsWindow;
    std::map<QString, PackageDetailWindow*> mPackageDetailWindows;

    RawDataStorage *mPackagesStorage;
    PackageCapture *mPackageCapture;
};
#endif // MAINWINDOW_H
