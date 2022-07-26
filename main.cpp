#include "ui/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {

#ifdef _WIN32
    setlocale(LC_ALL, ".UTF-8");
#else
    setlocale(LC_ALL, "");
#endif

#if defined(Q_OS_WIN)
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
