#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QPushButton>
#include <QFileDialog>
#include <QThreadPool>
#include <QTableWidgetItem>
#include <QDebug>
#include <QSortFilterProxyModel>

#include <memory>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    mPackagesStorage = &RawDataStorage::instance();
    appSettingsWindow = new AppSettingsWindow(nullptr);
    appSettings = &AppSettings::instance();

    mPackageCapture = new PackageCapture(this);
    connect(ui->interfaceCombobox, qOverload<int>(&QComboBox::currentIndexChanged), [this](int index) {
        mPackageCapture->setPcapDeviceByIpOrName(ui->interfaceCombobox->currentData(Qt::UserRole).toString());
    });

    capturedPackagesModel = new CapturedPackagesModel(this);
    ui->capturedPackagesTableView->verticalHeader()->setDefaultSectionSize(18);
    ui->capturedPackagesTableView->setModel(capturedPackagesModel);

    networkDeviceListModel = new NetworkDeviceListModel(this);
    networkDeviceListModel->updateList();
    ui->interfaceCombobox->setModel(networkDeviceListModel);

    ui->actionSavePcapng->setShortcut(QKeySequence::Save);
    connect(ui->actionSavePcapng, &QAction::triggered, [this](bool) {
        auto filePath = QFileDialog::getSaveFileName(this, "Save File", "./", tr("pcapng (*.pcapng)"));
        if (!filePath.endsWith(".pcapng", Qt::CaseInsensitive))
            filePath += ".pcapng";

        mPackagesStorage->savePcapngFile(filePath);
    });

    ui->actionOpenPcapng->setShortcut(QKeySequence::Open);
    connect(ui->actionOpenPcapng, &QAction::triggered, [this](bool){
        auto filePath = QFileDialog::getOpenFileName(this, tr("Open tshark"), "./", "pcapng (*.pcapng);;pcap (*.pcap)");
        if (!filePath.isEmpty())
            mPackagesStorage->loadPcapngFile(filePath);
    });

    connect(ui->btnStartSniff, &QPushButton::pressed, [this]() {
        if (mPackageCapture->isStarted()) {
            mPackageCapture->stopCapture();
            ui->btnStartSniff->setText("Start");
        } else {
            mPackageCapture->setPcapFilter(ui->tsharkFilterLine->text());
            mPackageCapture->startCapture();
            ui->btnStartSniff->setText("Stop");
        }
    });

//    pkgModel = new PackageDataTableModel(this);
//    ui->packageTable->setModel(pkgModel);

//    connect(ui->packageTable, &QTableView::doubleClicked, [this](const QModelIndex &index) {
//        auto opcode = pkgStorage->getPackageListItem(index.row()).opcode;

//        if (opcode.isEmpty())
//            return;

//        auto it = mPackageDetailWindows.find(opcode);
//        if (it != mPackageDetailWindows.end()) {
//            mPackageDetailWindows[opcode]->show();
//            mPackageDetailWindows[opcode]->setFocus();
//        } else {
//            PackageDetailWindow *window = new PackageDetailWindow(opcode, nullptr);
//            mPackageDetailWindows[opcode] = window;

//            window->setWindowTitle("Package " + opcode);
//            window->show();
//        }
//    });

//    pkgTotalModel = new PackageDataTotalModel(this);
//    ui->packageTotalTable->setModel(pkgTotalModel);
//    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
//    proxyModel->setSourceModel(pkgTotalModel);
//    ui->packageTotalTable->setModel(proxyModel);
//    ui->packageTotalTable->setSortingEnabled(true);
//    ui->packageTotalTable->sortByColumn(2, Qt::AscendingOrder);


    connect(ui->packageTotalTable, &QTableView::doubleClicked, [this](const QModelIndex &a_index){
        QModelIndexList indexList = ui->packageTotalTable->selectionModel()->selectedIndexes();
        int row;
        foreach (QModelIndex index, indexList) {
            auto opcode = ui->packageTotalTable->model()->data( ui->packageTotalTable->model()->index(index.row(), 0)).toString();

            auto it = mPackageDetailWindows.find(opcode);
            if (it != mPackageDetailWindows.end()) {
                mPackageDetailWindows[opcode]->show();
                mPackageDetailWindows[opcode]->setFocus();
            } else {
                PackageDetailWindow *window = new PackageDetailWindow(opcode, nullptr);
                mPackageDetailWindows[opcode] = window;

                window->setWindowTitle("Package " + opcode);
                window->show();
            }
        }
    });

    connect(ui->actionSettings, &QAction::triggered, [this](bool) {
        appSettingsWindow->show();
    });


    connect(ui->tsharkFilterLine, &QLineEdit::textChanged, appSettings, &AppSettings::setTsharkArguments);
    connect(appSettings, &AppSettings::tsharkArgsChanged, ui->tsharkFilterLine, &QLineEdit::setText);

    DataLoader::instance().load();
    appSettings->load();
}

MainWindow::~MainWindow() {
    for (auto it : mPackageDetailWindows) {
        it.second->close();
        it.second->deleteLater();
    }

    appSettings->save();
    delete ui;
}











