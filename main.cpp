#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(1200,950);
    w.setWindowIcon(QIcon(":/cards/cards/icon.png"));
    w.setWindowTitle("Ronda");

    w.show();
    return a.exec();
}
