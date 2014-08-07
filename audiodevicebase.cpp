#include "audiodevicebase.h"
#include <math.h>

double STANDARD_SAMPLERATE[] = {
    8000.0, 9600.0, 11025.0, 12000.0, 16000.0, 22050.0, 24000.0, 32000.0,
    44100.0, 48000.0, 88200.0, 96000.0, 192000.0, -1 /* negative terminated  list */
};

int BIT_RESOLUTION[] = {
    8, 16, 32, 64, -1 /* negative terminated  list */
};

static unsigned int          tempPhase = 0;

AudioDeviceBase::AudioDeviceBase():

    m_strPortaudioVersion(""),
    m_nPortaudioVersion(0),
    m_nNumberOfDevices(0),
    m_paStream(NULL),
    m_paError(0),

    m_nMinBufferSize(14),
    m_nMaxBufferSize(2048),

    m_nDefaultInputNumberOfChanel(2),
    m_nDefaultOutputNumberOfChanel(2),
    m_paDefaultSampleFormat(paInt32),
    m_dblDefaultSampleRates(44100.0),
    m_nDefaultBufferSize(2048),

    m_dblData(NULL)

{
    Initialize();
}

AudioDeviceBase::~AudioDeviceBase(){
    Terminate();
}

void AudioDeviceBase::Initialize(){
    m_paError = Pa_Initialize();
    if( m_paError == paNoError )
    {
        RetrivePortAudioInformation();
        SetDefaultAudioIODevice();
        SetData();
        AdjustDataTable();

        OpenStream();
        //StartStream();
    }
}

void AudioDeviceBase::Terminate(){
    StopStream();
    m_paError = Pa_CloseStream(m_paStream);
    Pa_Terminate();
    if(m_dblData)
        delete m_dblData;
}


void AudioDeviceBase::OpenStream()
{
    if( m_paError == paNoError )
    {
        m_paError = Pa_OpenStream(  &m_paStream,
                                    &m_paInputParameters,      /* No input. */
                                    &m_paOutputParameters,
                                    m_dblDefaultSampleRates,
                                    m_nDefaultBufferSize,       /* Frames per buffer. */
                                    paClipOff,                  /* We won't output out of range samples so don't bother clipping them. */
                                    PortAudioCallback,
                                    &m_dblData );
    }

}

void AudioDeviceBase::AdjustDataTable()
{
    for(int i=0; i<m_nDefaultBufferSize; i++ )
    {
        m_dblData[i] = (float) sin( ((double)i/(double)m_nDefaultBufferSize) * M_PI * 2. );
    }
}

int AudioDeviceBase::PortAudioCallback( const void *inputBuffer,
                              void *outputBuffer,
                              unsigned long framesPerBuffer,
                              const PaStreamCallbackTimeInfo* timeInfo,
                              PaStreamCallbackFlags statusFlags,
                              void *userData )
{
    int8_t *data = (int8_t*)userData;
        float *out = (float*)outputBuffer;
        float *in = (float*)inputBuffer;
        unsigned long i;

        (void) timeInfo; /* Prevent unused variable warnings. */
        (void) statusFlags;
        (void) inputBuffer;

        for( i=0; i<framesPerBuffer; i++ )
        {
            //*out++ = data[tempPhase];  /* left */
            //*out++ = data[tempPhase];  /* right */
            //tempPhase++;
            //if(tempPhase >= framesPerBuffer) tempPhase -= framesPerBuffer;
            //data->left_phase += 1;
            //if( data->left_phase >= TABLE_SIZE ) data->left_phase -= TABLE_SIZE;
            //data->right_phase += 3; /* higher pitch so we can distinguish left and right. */
            //if( data->right_phase >= TABLE_SIZE ) data->right_phase -= TABLE_SIZE;
            *out++ = *in++;
            *out++ = *in++;
        }

    return paContinue;
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
    if(m_nNumberOfDevices < 0 ){
        qDebug() << "ERROR: Pa_GetDeviceCount returned 0x"<< m_nNumberOfDevices;
    }

    const PaDeviceInfo *deviceInfo;
    // Mark Global API and specific Decice
    int defaultDisplay;
    qDebug() << "Number of Devices = " << m_nNumberOfDevices;
    for(int i = 0; i < m_nNumberOfDevices; i++){
        AudioDevice device;
        deviceInfo = Pa_GetDeviceInfo(i);
        device.name = deviceInfo->name;
        device.index = i;

        qDebug() << "------------- Device "<< i <<" -------------";
        defaultDisplay = 0;
        if( i == Pa_GetDefaultInputDevice()){
           qDebug() << "Default Input";
           device.isInputOrOutput   = true;
           device.isDefault         = true;
        }else if(i == Pa_GetHostApiInfo(deviceInfo->hostApi)->defaultInputDevice){
            const PaHostApiInfo *hostApiInfo = Pa_GetHostApiInfo(deviceInfo->hostApi);
            qDebug() << "Default " << hostApiInfo->name << "Input";
        }

        if(i == Pa_GetDefaultOutputDevice()){
            qDebug() << "Default Output";
            device.isInputOrOutput   = false;
            device.isDefault         = true;
        }else if(i == Pa_GetHostApiInfo(deviceInfo->hostApi)->defaultOutputDevice){
            const PaHostApiInfo *hostApiInfo = Pa_GetHostApiInfo(deviceInfo->hostApi);
            qDebug() << "Default " << hostApiInfo->name << " Output";
        }

        m_audioDevice.push_back(device);
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
    if (m_paInputParameters.device == paNoDevice) {
        m_paError = paNoDevice;
    }
    m_nDefaultInputNumberOfChanel                   = Pa_GetDeviceInfo( m_paInputParameters.device )->maxInputChannels;

    m_paInputParameters.channelCount                = m_nDefaultInputNumberOfChanel;
    m_paInputParameters.sampleFormat                = m_paDefaultSampleFormat;
    m_paInputParameters.suggestedLatency            = Pa_GetDeviceInfo( m_paInputParameters.device )->defaultLowInputLatency;
    m_paInputParameters.hostApiSpecificStreamInfo   = NULL;

    //outputStream
    m_paOutputParameters.device                     = Pa_GetDefaultOutputDevice();
    if (m_paOutputParameters.device == paNoDevice) {
        m_paError = paNoDevice;
    }
    m_nDefaultOutputNumberOfChanel                  = Pa_GetDeviceInfo( m_paOutputParameters.device )->maxOutputChannels;

    m_paOutputParameters.channelCount               = m_nDefaultOutputNumberOfChanel;
    m_paOutputParameters.sampleFormat               = m_paDefaultSampleFormat;
    m_paOutputParameters.suggestedLatency           = Pa_GetDeviceInfo( m_paOutputParameters.device )->defaultLowOutputLatency;
    m_paOutputParameters.hostApiSpecificStreamInfo  = NULL;
}

void AudioDeviceBase::SetData()
{
    m_dblData = new double[m_nDefaultBufferSize];
}

QList<AudioDevice>* AudioDeviceBase::get_AudioDeviceList()
{
    return &m_audioDevice;
}

double* AudioDeviceBase::get_AvailableSamplingRate()
{
    return STANDARD_SAMPLERATE;
}


void AudioDeviceBase::get_AvailableBufferSize(int &minBufferSize, int &maxBufferSize)
{
    minBufferSize = m_nMinBufferSize;
    maxBufferSize = m_nMaxBufferSize;
}

int* AudioDeviceBase::get_BitResolution()
{
    return BIT_RESOLUTION;
}

void AudioDeviceBase::get_DefaultBufferSize(int &bufferSize)
{
    bufferSize = m_nDefaultBufferSize;
}

void AudioDeviceBase::get_DefaultSamplingRate(double &samplingRate)
{
    samplingRate = m_dblDefaultSampleRates;
}
