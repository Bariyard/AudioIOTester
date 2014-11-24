#include "mainwindow.h"
#include <QApplication>
#include "audiodevicebase.h"
#include <QDebug>
#include "audiopreferencedialog.h"

#include "testmodule.h"
#include "synthesizer.h"

#include <QList>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //MainWindow w;
    //w.show();


    AudioDeviceBase *audioDeviceBase = new AudioDeviceBase();
    Synthesizer *synthesizer = new Synthesizer(audioDeviceBase);

    AudioPreferenceDialog *audioPrefDialog = new AudioPreferenceDialog(0,audioDeviceBase,synthesizer);
    audioPrefDialog->show();
    return a.exec();
}
