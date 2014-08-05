#ifndef AUDIODEVICEBASE_H
#define AUDIODEVICEBASE_H

#include "portaudio.h"
#include "observer.h"
#include <QString>
#include <QDebug>

class AudioDeviceBase : public Subject
{
public:
    AudioDeviceBase();
    ~AudioDeviceBase();

    void StartStream();
    void StopStream();

    //AudioDeviceBase Interface
    void get_AudioInputList();
    void get_AudioOutputList();
    double* get_AvailableSamplingRate();
    void get_AvailableBufferSize(int &minBufferSize, int &maxBufferSize);
    void get_DefaultBufferSize(int &bufferSize);
    void get_DefaultSamplingRate(double &samplingRate);

private:
    //Streaming function
    void Initialize();
    void Terminate();

    //portAudio version
    QString m_strVersion;
    int32_t m_nVersion;

    //Portaudio Devices
    int32_t m_nDevices;

    //PaStream
    PaStream*           stream;
    PaStreamParameters inputParameters;
    PaStreamParameters outputParameters;
    PaError error;
};

#endif // AUDIODEVICEBASE_H
