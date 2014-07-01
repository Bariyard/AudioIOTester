#ifndef AUDIODEVICEBASE_H
#define AUDIODEVICEBASE_H

#include "portaudio.h"
#include <QString>
#include <QDebug>

class AudioDeviceBase
{
public:
    AudioDeviceBase();
    ~AudioDeviceBase();
    //Streaming function
    void Initialize();
    void Terminate();


private:

    //portAudio version
    QString m_strVersion;
    int32_t m_nVersion;

    //Portaudio Devices
    int32_t m_nDevices;

    //PaStream
    PaStreamParameters inputParameters;
    PaStreamParameters outputParameters;
    PaError error;
};

#endif // AUDIODEVICEBASE_H
