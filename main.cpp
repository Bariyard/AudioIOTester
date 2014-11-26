#include <QApplication>
#include <QDebug>
#include "audiodevicebase.h"
#include "audiopreferencedialog.h"

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
