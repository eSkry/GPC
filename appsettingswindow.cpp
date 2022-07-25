#include "appsettingswindow.h"
#include "ui_appsettingswindow.h"

#include <QtGlobal>

AppSettingsWindow::AppSettingsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AppSettingsWindow) {
    ui->setupUi(this);

    mSettings = &AppSettings::instance();

    // Updates from settings
    connect(mSettings, &AppSettings::opcodeBytesCountChanged, ui->opcodeLengthSpinBox, &QSpinBox::setValue);
    connect(mSettings, &AppSettings::opcodeBytesOffsetChanged, ui->opcodeOffsetSpinBox, &QSpinBox::setValue);

    connect(mSettings, &AppSettings::pkgLenghtBytesCountChanged, ui->pkgLenBytesCountSpinBox, &QSpinBox::setValue);
    connect(mSettings, &AppSettings::pkgLenghtBytesOffsetChanged, ui->pkgLenByteBeginOffset, &QSpinBox::setValue);
    connect(mSettings, &AppSettings::pkgLengthBytesEndianChanged, ui->pkgLenEndian, &QCheckBox::setChecked);

    // Updates from window
    connect(ui->opcodeLengthSpinBox, qOverload<int>(&QSpinBox::valueChanged), mSettings, &AppSettings::opcodeBytesCountChanged);
    connect(ui->opcodeOffsetSpinBox, qOverload<int>(&QSpinBox::valueChanged), mSettings, &AppSettings::opcodeBytesOffsetChanged);

    connect(ui->pkgLenBytesCountSpinBox, qOverload<int>(&QSpinBox::valueChanged), mSettings, &AppSettings::pkgLenghtBytesCountChanged);
    connect(ui->pkgLenByteBeginOffset, qOverload<int>(&QSpinBox::valueChanged), mSettings, &AppSettings::pkgLenghtBytesOffsetChanged);
    connect(ui->pkgLenEndian, &QCheckBox::clicked, mSettings, &AppSettings::pkgLengthBytesEndianChanged);

    ///////////////////////////////////////////////////////
    connect(ui->saveBtn, &QPushButton::pressed, [this]() {
        mSettings->save();
    });
}

AppSettingsWindow::~AppSettingsWindow() {
    delete ui;
}
