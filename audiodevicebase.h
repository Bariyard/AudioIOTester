#ifndef AUDIODEVICEBASE_H
#define AUDIODEVICEBASE_H

#include "portaudio.h"
#include "observer.h"
#include <QString>
#include <QDebug>

enum AudioBitDepth{
    AudioBitDepthunknown    = 0,
    AudioBitDepth8Bit       = 1,
    AudioBitDepth16Bit      = 2,
    AudioBitDepth32Bit      = 3,
    AudioBitDepth64Bit      = 4
};

struct AudioDevice{
    QString name;
    int     index;
    bool    isInputOrOutput;    //input = true, output = false
    bool    isDefault;
};

typedef struct
{
    double *data;
    int left_phase;
    int right_phase;
    unsigned int framesToGo;
}
AudioData;


class AudioDeviceBase : public Subject
{
public:
    AudioDeviceBase();
    ~AudioDeviceBase();


    void StartStream();
    void StopStream();

    //AudioDeviceBase Interface
    QList<AudioDevice>* get_AudioDeviceList();
    double* get_AvailableSamplingRate();
    void get_AvailableBufferSize(int &minBufferSize, int &maxBufferSize);
    int* get_BitResolution();
    void get_DefaultBufferSize(int &bufferSize);
    void get_DefaultSamplingRate(double &samplingRate);


    //data table
    void AdjustDataTable();

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
    double              m_dblDefaultSampleRates;
    int                 m_nDefaultBufferSize;
    //int                 m_nDefaultBitRate;

    void SetData();

    AudioData           *m_SAudioData;

};

#endif // AUDIODEVICEBASE_H
