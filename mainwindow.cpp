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
    mPkgCount = 0;
    pkgStorage = &DataStorage::instance();
    errorsWindow = new ErrorsWindow(nullptr);
    appSettingsWindow = new AppSettingsWindow(nullptr);
    appSettings = &AppSettings::instance();
    mDataBase = &DataBase::instance();
    mDataBase->createNew("default.db");


    connect(ui->btnErrors, &QPushButton::pressed, errorsWindow, &ErrorsWindow::show);
    connect(ui->btnStartSniff, &QPushButton::pressed, this, &MainWindow::tsharkStart);

    connect(ui->btn_tsharkPathOpen, &QPushButton::pressed, [this](){
        auto fileName = QFileDialog::getOpenFileName(this, tr("Open tshark"), "./", "Executable Windows (*.exe);;tshark (tshark.exe);;Executable Linux (*)");
        if (!fileName.isEmpty())
            this->ui->tsharkLineEdit->setText(fileName);
    });

    connect(ui->btnClearData, &QPushButton::pressed, pkgStorage, &DataStorage::clearPackageList);

//    sqlModel = new QSqlTableModel(this, mDataBase->getDataBase());
//    sqlModel->setTable("packages");
//    sqlModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
//    sqlModel->select();
//    ui->packageTable->setModel(sqlModel);

//    connect(mDataBase, &DataBase::addedNewPackage, [this](){
//        while (sqlModel->canFetchMore())
//            sqlModel->fetchMore();
//    });

    pkgModel = new PackageDataTableModel(this);
    ui->packageTable->setModel(pkgModel);

    connect(ui->packageTable, &QTableView::doubleClicked, [this](const QModelIndex &index) {
        auto opcode = pkgStorage->getPackageListItem(index.row()).opcode;

        if (opcode.isEmpty())
            return;

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
    });

    pkgTotalModel = new PackageDataTotalModel(this);
    ui->packageTotalTable->setModel(pkgTotalModel);
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

    connect(ui->btnSave, &QPushButton::pressed, [this](){
        auto &loader = DataLoader::instance();
        loader.save();
    });

    connect(ui->actionSettings, &QAction::triggered, [this](bool) {
        appSettingsWindow->show();
    });


    connect(ui->tsharkLineEdit, &QLineEdit::textChanged, appSettings, &AppSettings::setProgrammPath);
    connect(ui->tsharkFilterLine, &QLineEdit::textChanged, appSettings, &AppSettings::setTsharkArguments);

    connect(appSettings, &AppSettings::programmPathChanged, ui->tsharkLineEdit, &QLineEdit::setText);
    connect(appSettings, &AppSettings::tsharkArgsChanged, ui->tsharkFilterLine, &QLineEdit::setText);

    DataLoader::instance().load();
    appSettings->load();
}

MainWindow::~MainWindow() {
    for (auto it : mPackageDetailWindows) {
        it.second->close();
        it.second->deleteLater();
    }
    errorsWindow->close();
    errorsWindow->deleteLater();

    appSettings->save();
    delete ui;
}


void MainWindow::tsharkStart() {
    if (!tsharkPtr) {
        ui->btnStartSniff->setText("Stop");
        tsharkPtr = std::make_shared<TsharkWrapper>();

        connect(tsharkPtr.get(), &TsharkWrapper::subprocessFinished, this, &MainWindow::tsharkStopped);
        connect(tsharkPtr.get(), &TsharkWrapper::errorMessage, errorsWindow, &ErrorsWindow::addString);

        QString cmd = ui->tsharkLineEdit->text() + " ";
        cmd += ui->tsharkFilterLine->text();
        QThreadPool::globalInstance()->start(new ProcessThread(tsharkPtr, cmd));
    } else {
        tsharkPtr->terminate();
    }
}

void MainWindow::tsharkStopped() {
    if (tsharkPtr) {
        tsharkPtr = nullptr;

        ui->btnStartSniff->setText("Start");
    }
}











