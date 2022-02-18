#include "mainwindow.h"

#include <QApplication>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    QScreen *desktop = QGuiApplication::primaryScreen();

    w.move((desktop -> geometry().width() - w.width()) /  2, w.y());
    return a.exec();
}
