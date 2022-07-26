#ifndef PACKAGEDETAILWINDOW_H
#define PACKAGEDETAILWINDOW_H

#include <QWidget>
#include <QColor>
#include <QString>

#include "PackageData.h"
#include "selectedopcodemodel.h"

namespace Ui {
class PackageDetailWindow;
}

class PackageDetailWindow : public QWidget {
    Q_OBJECT
public:
    explicit PackageDetailWindow(const QString& opcode, QWidget *parent = nullptr);
    ~PackageDetailWindow();

public slots:

    void setColor(const QColor& color);

private:
    Ui::PackageDetailWindow *ui;
    QString mOpcode;
    DataStorage *pkgStorage;
    SelectedOpcodeModel *model;
};

#endif // PACKAGEDETAILWINDOW_H
