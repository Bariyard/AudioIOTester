#include "mainwindow.h"
#include <QApplication>
#include "audiodevicebase.h"
#include <QDebug>

void test();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    AudioDeviceBase *audioDeviceBase = new AudioDeviceBase;
    return a.exec();
}
