#include "audiodevicebase.h"

double STANDARD_SAMPLERATE[] = {
    //    8000.0, 9600.0, 11025.0, 12000.0, 16000.0, 22050.0, 24000.0, 32000.0,
    44100.0, 48000.0, 88200.0, 96000.0, 192000.0, -1 /* negative terminated  list */
};

double STANDARD_BUFFERSIZE[] = {
    32, 64, 128, 526, 1024, 2048, -1
};

AudioDeviceBase::AudioDeviceBase():
    m_strPortaudioVersion(""),
    m_nPortaudioVersion(0),
    m_nNumberOfDevices(0),
    m_paStream(NULL),
    m_paError(0),
    m_nDefaultInputNumberOfChanel(2),
    m_nDefaultOutputNumberOfChanel(2),
    m_paDefaultSampleFormat(paFloat32),
    m_nSampleRate(44100),
    m_nBufferSize(1024)
{
    Initialize();
}

AudioDeviceBase::~AudioDeviceBase()
{
    Terminate();
}

void AudioDeviceBase::Initialize()
{
    m_paError = Pa_Initialize();
    if( m_paError == paNoError )
    {
        RetrivePortAudioInformation();
        SetDefaultAudioIODevice();
        OpenStream();
    }
}

void AudioDeviceBase::Terminate()
{
    StopStream();
    m_paError = Pa_CloseStream(m_paStream);
    Pa_Terminate();
}

void AudioDeviceBase::OpenStream()
{
    if( m_paError == paNoError )
    {
        m_paError = Pa_OpenStream(
                    &m_paStream,
                    &m_paInputParameters, /* no input */
                    &m_paOutputParameters,
                    m_nSampleRate,
                    m_nBufferSize,
                    paClipOff|paDitherOff, /* we won't output out of range samples so don't bother clipping them */
                    &AudioDeviceBase::paCallback,
                    this /* Using 'this' for userData so we can cast to Sine* in paCallback method */
                    );
    }
}

int AudioDeviceBase::CallbackFunction(const void *inputBuffer, void *outputBuffer,
                                      unsigned long framesPerBuffer,
                                      const PaStreamCallbackTimeInfo* timeInfo,
                                      PaStreamCallbackFlags statusFlags)
{
    float *out = (float*)outputBuffer;
    unsigned long i;

    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    (void) inputBuffer;

    //clean buffer
    for(i = 0; i < framesPerBuffer; i++){
        *out++ = 0.0;
        *out++ = 0.0;
    }

    if(!m_pTestModule.isEmpty())
    {
        //process each test module in the list
        foreach( TestModule *pMod, m_pTestModule )
        {
            pMod->process(inputBuffer,outputBuffer,framesPerBuffer);
        }
    }
    return paContinue;
}

/* This routine will be called by the PortAudio engine when audio is needed.
** It may called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
int AudioDeviceBase::paCallback( const void *inputBuffer, void *outputBuffer,
                                 unsigned long framesPerBuffer,
                                 const PaStreamCallbackTimeInfo* timeInfo,
                                 PaStreamCallbackFlags statusFlags,
                                 void *userData )
{
    /* Here we cast userData to Sine* type so we can call the instance method paCallbackMethod, we can do that since
    we called Pa_OpenStream with 'this' for userData */
    return ((AudioDeviceBase*)userData)->CallbackFunction(inputBuffer, outputBuffer,
                                                          framesPerBuffer,
                                                          timeInfo,
                                                          statusFlags);
}

void AudioDeviceBase::StartStream()
{
    if( m_paError == paNoError )
    {
        m_paError = Pa_StartStream(m_paStream);
    }

}

void AudioDeviceBase::StopStream()
{
    if( m_paError == paNoError )
    {
        m_paError = Pa_StopStream(m_paStream);
    }
}

void AudioDeviceBase::RetrivePortAudioInformation()
{
    //Get Portaudio Information
    m_strPortaudioVersion = Pa_GetVersionText();
    m_nPortaudioVersion = Pa_GetVersion();
    m_nNumberOfDevices = Pa_GetDeviceCount();
    if(m_nNumberOfDevices < 0 )
    {
        qDebug() << "ERROR: Pa_GetDeviceCount returned 0x"<< m_nNumberOfDevices;
    }

    const PaDeviceInfo *deviceInfo;
    // Mark Global API and specific Decice
    int defaultDisplay;
    qDebug() << "Number of Devices = " << m_nNumberOfDevices;
    for(int i = 0; i < m_nNumberOfDevices; i++)
    {
        AudioDevice device;
        deviceInfo = Pa_GetDeviceInfo(i);
        device.strName = deviceInfo->name;
        device.nIndex = i;

        qDebug() << "------------- Device "<< i <<" -------------";
        defaultDisplay = 0;
        if( i == Pa_GetDefaultInputDevice())
        {
            qDebug() << "Default Input";
            device.bIsInputOrOutput     = true;
            device.bIsDefaultDevice     = true;
        }
        else if(i == Pa_GetHostApiInfo(deviceInfo->hostApi)->defaultInputDevice)
        {
            const PaHostApiInfo *hostApiInfo = Pa_GetHostApiInfo(deviceInfo->hostApi);
            qDebug() << "Default " << hostApiInfo->name << "Input";
        }

        if(i == Pa_GetDefaultOutputDevice())
        {
            qDebug() << "Default Output";
            device.bIsInputOrOutput   = false;
            device.bIsDefaultDevice         = true;
        }
        else if(i == Pa_GetHostApiInfo(deviceInfo->hostApi)->defaultOutputDevice)
        {
            const PaHostApiInfo *hostApiInfo = Pa_GetHostApiInfo(deviceInfo->hostApi);
            qDebug() << "Default " << hostApiInfo->name << " Output";
        }

        m_AudioDevice.push_back(device);
        //print device info field
        qDebug() << "Name:  "                       << deviceInfo->name;
        qDebug() << "Max Input Chanel: "            << deviceInfo->maxInputChannels;
        qDebug() << "Max Output Chanel: "           << deviceInfo->maxOutputChannels;
        qDebug() << "Default Sample Rate"           << deviceInfo->defaultSampleRate;
        qDebug() << "Default High Input Latency:\t"  << deviceInfo->defaultHighInputLatency;
        qDebug() << "Default High Output Latency:\t" << deviceInfo->defaultHighOutputLatency;
        qDebug() << "Default Low Input Latency:\t"   << deviceInfo->defaultLowInputLatency;
        qDebug() << "Default Low Output Latency:\t"  << deviceInfo->defaultLowOutputLatency;
    }
}

void AudioDeviceBase::SetDefaultAudioIODevice()
{
    //inputStream
    m_paInputParameters.device                      = Pa_GetDefaultInputDevice();
    if (m_paInputParameters.device == paNoDevice)
        m_paError = paNoDevice;
    m_nDefaultInputNumberOfChanel                   = Pa_GetDeviceInfo(m_paInputParameters.device)->maxInputChannels;
    m_paInputParameters.channelCount                = m_nDefaultInputNumberOfChanel;
    m_paInputParameters.sampleFormat                = m_paDefaultSampleFormat;
    m_paInputParameters.suggestedLatency            = Pa_GetDeviceInfo(m_paInputParameters.device)->defaultLowInputLatency;
    m_paInputParameters.hostApiSpecificStreamInfo   = NULL;

    //outputStream
    m_paOutputParameters.device                     = Pa_GetDefaultOutputDevice();
    if (m_paOutputParameters.device == paNoDevice)
        m_paError = paNoDevice;
    m_nDefaultOutputNumberOfChanel                  = Pa_GetDeviceInfo(m_paOutputParameters.device)->maxOutputChannels;
    m_paOutputParameters.channelCount               = m_nDefaultOutputNumberOfChanel;
    m_paOutputParameters.sampleFormat               = m_paDefaultSampleFormat;
    m_paOutputParameters.suggestedLatency           = Pa_GetDeviceInfo(m_paOutputParameters.device)->defaultLowOutputLatency;
    m_paOutputParameters.hostApiSpecificStreamInfo  = NULL;
}

QList<AudioDevice>* AudioDeviceBase::get_AudioDeviceList()
{
    return &m_AudioDevice;
}

double* AudioDeviceBase::get_AvailableSamplingRate()
{
    return STANDARD_SAMPLERATE;
}

double* AudioDeviceBase::get_AvailableBufferSize()
{
    return STANDARD_BUFFERSIZE;
}

int AudioDeviceBase::get_SamplingRate()
{
    return  m_nSampleRate;
}

int AudioDeviceBase::get_BufferSize()
{
    return m_nBufferSize;
}

void AudioDeviceBase::put_InputDevice(int nDevice, bool bIsStreamActive)
{

    if(nDevice != m_paInputParameters.device)
    {
        if(bIsStreamActive)
            StopStream();
        Pa_CloseStream(m_paStream);

        if (m_paInputParameters.device == paNoDevice)
        {
            m_paError = paNoDevice;
            SetDefaultAudioIODevice();
            return;
        }
        //inputStream
        m_paInputParameters.device                      = nDevice;
        m_nDefaultInputNumberOfChanel                   = Pa_GetDeviceInfo( m_paInputParameters.device )->maxInputChannels;
        m_paInputParameters.channelCount                = m_nDefaultInputNumberOfChanel;
        m_paInputParameters.sampleFormat                = m_paDefaultSampleFormat;
        m_paInputParameters.suggestedLatency            = Pa_GetDeviceInfo( m_paInputParameters.device )->defaultLowInputLatency;
        m_paInputParameters.hostApiSpecificStreamInfo   = NULL;

        OpenStream();
        if(bIsStreamActive)
            StartStream();
        ShowDeviceInfo(m_paInputParameters);
    }
}

void AudioDeviceBase::put_OutputDevice(int nDevice, bool bIsStreamActive)
{
    if(nDevice != m_paOutputParameters.device )
    {
        if(bIsStreamActive)
            StopStream();
        Pa_CloseStream(m_paStream);

        if (m_paOutputParameters.device == paNoDevice)
        {
            m_paError = paNoDevice;
            SetDefaultAudioIODevice();
            return;
        }
        m_paOutputParameters.device                     = nDevice;
        m_nDefaultOutputNumberOfChanel                  = Pa_GetDeviceInfo( m_paOutputParameters.device )->maxOutputChannels;
        m_paOutputParameters.channelCount               = m_nDefaultOutputNumberOfChanel;
        m_paOutputParameters.sampleFormat               = m_paDefaultSampleFormat;
        m_paOutputParameters.suggestedLatency           = Pa_GetDeviceInfo( m_paOutputParameters.device )->defaultLowOutputLatency;
        m_paOutputParameters.hostApiSpecificStreamInfo  = NULL;

        OpenStream();
        if(bIsStreamActive)
            StartStream();
        ShowDeviceInfo(m_paOutputParameters);
    }
}

void AudioDeviceBase::put_SamplingRate(int nSamplingRate, bool bIsStreamActive)
{
    if(nSamplingRate != m_nSampleRate)
    {
        m_nSampleRate = nSamplingRate;
        qDebug() << "Current Sampling Rate: " << m_nSampleRate;
        if(bIsStreamActive)
            StopStream();
        Pa_CloseStream(m_paStream);

        ResetTestModule();
        OpenStream();
        if(bIsStreamActive)
            StartStream();
    }
}

void AudioDeviceBase::put_BufferSize(int nBufferSize, bool bIsStreamActive)
{
    if(m_nBufferSize != (unsigned int)nBufferSize)
    {
        m_nBufferSize = nBufferSize;
        qDebug() << "Current Buffer Size: " << m_nBufferSize;
        if(bIsStreamActive)
            StopStream();
        Pa_CloseStream(m_paStream);

        ResetTestModule();
        OpenStream();
        if(bIsStreamActive)
            StartStream();
    }
}

void AudioDeviceBase::ShowDeviceInfo(const PaStreamParameters paStream)
{
    const PaDeviceInfo *deviceInfo;
    deviceInfo = Pa_GetDeviceInfo(paStream.device);
    qDebug() << "------------- Device "<< paStream.device <<" -------------";
    qDebug() << "Name:  "                       << deviceInfo->name;
    qDebug() << "Max Input Chanel: "            << deviceInfo->maxInputChannels;
    qDebug() << "Max Output Chanel: "           << deviceInfo->maxOutputChannels;
    qDebug() << "Default Sample Rate"           << deviceInfo->defaultSampleRate;
    qDebug() << "Default High Input Latency:\t"  << deviceInfo->defaultHighInputLatency;
    qDebug() << "Default High Output Latency:\t" << deviceInfo->defaultHighOutputLatency;
    qDebug() << "Default Low Input Latency:\t"   << deviceInfo->defaultLowInputLatency;
    qDebug() << "Default Low Output Latency:\t"  << deviceInfo->defaultLowOutputLatency;
}

void AudioDeviceBase::RegisterTestModule(TestModule *pModule)
{
    m_pTestModule.push_back(pModule);
}

void AudioDeviceBase::ResetTestModule()
{
    if(!m_pTestModule.isEmpty())
    {
        foreach(TestModule *pMod, m_pTestModule ){
            pMod->reset();
        }
    }
}

