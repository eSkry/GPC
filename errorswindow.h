#ifndef ERRORSWINDOW_H
#define ERRORSWINDOW_H

#include <QWidget>

namespace Ui {
class ErrorsWindow;
}

class ErrorsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ErrorsWindow(QWidget *parent = nullptr);
    ~ErrorsWindow();

public slots:

    void addString(const QString& text);

private:
    Ui::ErrorsWindow *ui;
};

#endif // ERRORSWINDOW_H
