#ifndef AUDIODEVICEBASE_H
#define AUDIODEVICEBASE_H

#include "portaudio.h"
#include <QString>
#include <QDebug>
#include <QObject>
#include <QList>

#include "testmodule.h"


struct AudioDevice{
    QString name;
    int     index;
    bool    isInputOrOutput;    //input = true, output = false
    bool    isDefault;
};

typedef struct
{
    float *data;
    int left_phase;
    int right_phase;
    unsigned int framesToGo;
}
AudioData;


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
    double* get_AvailableSamplingRate();
    void get_AvailableBufferSize(int &minBufferSize, int &maxBufferSize);
    int* get_BitResolution();
    void get_DefaultBufferSize(int &bufferSize);
    void get_DefaultSamplingRate(double &samplingRate);
    int get_SamplingRate();


    //data table
    //void AdjustDataTable();
    void put_InputDevice(int nDevice, bool bIsStreamActive = false);
    void put_OutputDevice(int nDevice, bool bIsStreamActive = false);
    void put_SamplingRate(int nSamplingRate, bool bIsStreamActive = false);
    void put_BufferSize(int nBufferSize, bool bIsStreamActive = false);
    void put_BitResoulution(int nBitResolution);
    //void put_AudioFrequency(double dblFrequency);

    void put_DataTable(const AudioData *audioData);


    void registerTestModule(TestModule *mod);



private:
    //Callback
    int paCallbackMethod(const void *inputBuffer, void *outputBuffer,
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

    //Portaudio Information
    void RetrivePortAudioInformation();
    void SetDefaultAudioIODevice();
    void ShowDeviceInfo(const PaStreamParameters paStream);
    QString             m_strPortaudioVersion;
    int                 m_nPortaudioVersion;
    int                 m_nNumberOfDevices;

    QList<AudioDevice>  m_audioDevice;

    //PaStream
    PaStream*           m_paStream;
    PaStreamParameters  m_paInputParameters;
    PaStreamParameters  m_paOutputParameters;
    PaError             m_paError;

    int                 m_nMinBufferSize;
    int                 m_nMaxBufferSize;

    //Default value
    int                 m_nDefaultInputNumberOfChanel;
    int                 m_nDefaultOutputNumberOfChanel;
    PaSampleFormat      m_paDefaultSampleFormat;
    double              m_dblSampleRates;
    unsigned int        m_nBufferSize;
    //int                 m_nDefaultBitRate;

    void SetData();

    AudioData           *m_SAudioData;
    QList<TestModule*>   m_TestModule;
};

#endif // AUDIODEVICEBASE_H
