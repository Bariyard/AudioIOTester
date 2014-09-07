#include "mainwindow.h"
#include <QApplication>
#include "audiodevicebase.h"
#include <QDebug>
#include "audiopreferencedialog.h"

void test();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //MainWindow w;
    //w.show();

    AudioDeviceBase *audioDeviceBase = new AudioDeviceBase();
    AudioPreferenceDialog *audioPrefDialog = new AudioPreferenceDialog(0,audioDeviceBase);
    audioPrefDialog->show();
    return a.exec();
}
