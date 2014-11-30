#ifndef AUDIODEVICEBASE_H
#define AUDIODEVICEBASE_H

#include <QString>
#include <QDebug>
#include <QObject>
#include <QList>

#include "portaudio.h"
#include "testmodule.h"


struct AudioDevice{
    QString strName;
    int     nIndex;
    bool    bIsInputOrOutput;    //input = true, output = false
    bool    bIsDefaultDevice;
};

class AudioDeviceBase : public QObject
{
    Q_OBJECT
public:
    AudioDeviceBase();
    virtual ~AudioDeviceBase();

    void StartStream();
    void StopStream();

    //AudioDeviceBase Interface
    QList<AudioDevice>* get_AudioDeviceList();
    double*     get_AvailableSamplingRate();
    double*     get_AvailableBufferSize();
    int         get_SamplingRate();
    int         get_BufferSize();


    void put_InputDevice(int nDevice, bool bIsStreamActive = false);
    void put_OutputDevice(int nDevice, bool bIsStreamActive = false);
    void put_SamplingRate(int nSamplingRate, bool bIsStreamActive = false);
    void put_BufferSize(int nBufferSize, bool bIsStreamActive = false);

    void RegisterTestModule(TestModule *pModule);

private:
    //Callback
    int CallbackFunction(const void *inputBuffer, void *outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo* timeInfo,
                        PaStreamCallbackFlags statusFlags);

    static int paCallback( const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData );

    //Streaming function
    void Initialize();
    void Terminate();
    void OpenStream();
    void ResetTestModule();

    //Portaudio Information
    void RetrivePortAudioInformation();
    void SetDefaultAudioIODevice();
    void ShowDeviceInfo(const PaStreamParameters paStream);
    QString             m_strPortaudioVersion;
    int                 m_nPortaudioVersion;
    int                 m_nNumberOfDevices;

    QList<AudioDevice>  m_AudioDevice;

    //PaStream
    PaStream*           m_paStream;
    PaStreamParameters  m_paInputParameters;
    PaStreamParameters  m_paOutputParameters;
    PaError             m_paError;

    //Default value
    int                 m_nDefaultInputNumberOfChanel;
    int                 m_nDefaultOutputNumberOfChanel;
    PaSampleFormat      m_paDefaultSampleFormat;
    int                 m_nSampleRate;
    unsigned int        m_nBufferSize;

    QList<TestModule*>   m_pTestModule;
};

#endif // AUDIODEVICEBASE_H
