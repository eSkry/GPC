#ifndef APPSETTINGSWINDOW_H
#define APPSETTINGSWINDOW_H

#include <QWidget>

#include "appsettings.h"

namespace Ui {
class AppSettingsWindow;
}

class AppSettingsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AppSettingsWindow(QWidget *parent = nullptr);
    ~AppSettingsWindow();

private:
    Ui::AppSettingsWindow *ui;
    AppSettings *mSettings;
};

#endif // APPSETTINGSWINDOW_H
