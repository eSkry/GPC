#include "errorswindow.h"
#include "ui_errorswindow.h"

ErrorsWindow::ErrorsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ErrorsWindow) {
    ui->setupUi(this);

    connect(ui->btnClear, &QPushButton::pressed, ui->errorTextEdit, &QPlainTextEdit::clear);
}

ErrorsWindow::~ErrorsWindow() {
    delete ui;
}

void ErrorsWindow::addString(const QString &text) {
    ui->errorTextEdit->appendPlainText(text);
}
