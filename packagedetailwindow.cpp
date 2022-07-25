#include "packagedetailwindow.h"
#include "ui_packagedetailwindow.h"

#include <QColorDialog>

PackageDetailWindow::PackageDetailWindow(const QString& opcode, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PackageDetailWindow),
    mOpcode{opcode} {
    ui->setupUi(this);
    pkgStorage = &DataStorage::instance();
    model = new SelectedOpcodeModel(opcode, this);

    ui->packagesTableView->setModel(model);

    connect(ui->btnChoseColor, &QPushButton::pressed, [this]() {
        auto color = QColorDialog::getColor();
        setColor(color);
    });

    ui->pkgNameTextEdit->setText(pkgStorage->getPackageInfoItem(mOpcode).name);
    setColor(pkgStorage->getPackageInfoItem(mOpcode).color);

    connect(ui->pkgNameTextEdit, &QLineEdit::returnPressed, [this]() {
        pkgStorage->setNamePackage(mOpcode, ui->pkgNameTextEdit->text());
    });

    connect(ui->pkgNameTextEdit, &QLineEdit::returnPressed, [this]() {
        QColor color;
        if (ui->pkgNameTextEdit->text().isEmpty()) {
            color = Qt::transparent;
        } else {
            color = QColor(ui->pkgNameTextEdit->text());
        }
        setColor(color);
    });

    connect(ui->cbIgnorePackage, &QCheckBox::stateChanged, [this](int){
        pkgStorage->getPackageInfoItem(mOpcode).ignore = ui->cbIgnorePackage->isChecked();
    });
}

PackageDetailWindow::~PackageDetailWindow() {
    delete ui;
}

void PackageDetailWindow::setColor(const QColor &color) {
    auto &settings = pkgStorage->getPackageInfoItem(mOpcode);
    settings.color = color;
    ui->colorTextLineEdit->setText( color.name() );
    ui->colorTextLineEdit->setStyleSheet("background-color:" + color.name() + ";");
}
