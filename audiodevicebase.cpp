#include "audiodevicebase.h"

static double standardSampleRates[] = {
    8000.0, 9600.0, 11025.0, 12000.0, 16000.0, 22050.0, 24000.0, 32000.0,
    44100.0, 48000.0, 88200.0, 96000.0, 192000.0, -1 /* negative terminated  list */
};


AudioDeviceBase::AudioDeviceBase()
{
    Initialize();
}

AudioDeviceBase::~AudioDeviceBase(){
    Terminate();
}

void AudioDeviceBase::Initialize(){
    const PaDeviceInfo *deviceInfo;
    Pa_Initialize();

    //Get Portaudio Version Number
    m_strVersion = Pa_GetVersionText();
    m_nVersion = Pa_GetVersion();

    m_nDevices = Pa_GetDeviceCount();
    if(m_nDevices < 0 ){
        qDebug() << "ERROR: Pa_GetDeviceCount returned 0x"<< m_nDevices;
        error = m_nDevices;
    }

    // Mark Global API and specific Decice
    int defaultDisplay;
    qDebug() << "Number of Devices = " << m_nDevices;
    for(int i = 0; i < m_nDevices; i++){
        deviceInfo = Pa_GetDeviceInfo(i);
        qDebug() << "------------- Device "<< i <<" -------------";
        defaultDisplay = 0;
        if( i == Pa_GetDefaultInputDevice()){
           qDebug() << "Default Input";
           defaultDisplay = 1;
        }else if(i == Pa_GetHostApiInfo(deviceInfo->hostApi)->defaultInputDevice){
            const PaHostApiInfo *hostApiInfo = Pa_GetHostApiInfo(deviceInfo->hostApi);
            qDebug() << "Default " << hostApiInfo->name << "Input";
            defaultDisplay = 1;
        }

        if(i == Pa_GetDefaultOutputDevice()){
            qDebug() << "Default Output";
            defaultDisplay = 1;
        }else if(i == Pa_GetHostApiInfo(deviceInfo->hostApi)->defaultOutputDevice){
            const PaHostApiInfo *hostApiInfo = Pa_GetHostApiInfo(deviceInfo->hostApi);
            qDebug() << "Default " << hostApiInfo->name << " Output";
            defaultDisplay = 1;
        }

        //print device info field
        qDebug() << "Name:  "                       << deviceInfo->name;
        qDebug() << "Max Input Chanel: "            << deviceInfo->maxInputChannels;
        qDebug() << "Max Output Chanel: "           << deviceInfo->maxOutputChannels;
        qDebug() << "Default Sample Rate"           << deviceInfo->defaultSampleRate;
        qDebug() << "Default High Input Latency:\t"  << deviceInfo->defaultHighInputLatency;
        qDebug() << "Default High Output Latency:\t" << deviceInfo->defaultHighOutputLatency;
        qDebug() << "Default Low Input Latency:\t"   << deviceInfo->defaultLowInputLatency;
        qDebug() << "Default Low Output Latency:\t"  << deviceInfo->defaultLowOutputLatency;

        //inputStream
        inputParameters.device                      = i;
        inputParameters.channelCount                = deviceInfo->maxInputChannels;
        inputParameters.sampleFormat                = paInt16;
        inputParameters.suggestedLatency            = 0;
        inputParameters.hostApiSpecificStreamInfo   = NULL;

        //outputStream
        outputParameters.device                     = i;
        outputParameters.channelCount               = deviceInfo->maxOutputChannels;
        outputParameters.sampleFormat               = paInt16;
        outputParameters.suggestedLatency           = 0;
        outputParameters.hostApiSpecificStreamInfo  = NULL;

    }

}

void AudioDeviceBase::Terminate(){
    Pa_Terminate();
}
