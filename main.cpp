#include <QApplication>
#include <QDebug>
#include "audiodevicebase.h"
#include "audiopreferencedialog.h"

#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile file(":/stylesheet.qss");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        a.setStyleSheet(file.readAll());
        file.close();
    }
    AudioDeviceBase *audioDeviceBase = new AudioDeviceBase();
    AudioPreferenceDialog *audioPrefDialog = new AudioPreferenceDialog(0,audioDeviceBase);
    audioPrefDialog->show();



    return a.exec();
}
