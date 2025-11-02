#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.setWindowTitle("Color Vision - HSV ↔ XYZ ↔ LAB Converter");
    window.show();
    return app.exec();
}
