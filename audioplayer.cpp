#include "audioplayer.h"
#include "sndfile.h"
#include "waveform.h"

AudioPlayer::AudioPlayer(AudioDeviceBase* pAudioDeviceBase)
{
    m_pAudioDeviceBase  = pAudioDeviceBase;
    m_bIsModuleEnable   = false;
    m_bEndOfFile        = true;
    m_strAudioPath      = "";
    m_pAudioDeviceBase->RegisterTestModule(this);
}

AudioPlayer::~AudioPlayer()
{

}

void AudioPlayer::reset()
{

}


void AudioPlayer::process(const void */*inputBuffer*/, void *outputBuffer, const unsigned long framesPerBuffer)
{
    if(m_bEndOfFile){
        qDebug() << "End Of Audio File";
        return;
    }

    if(m_bIsModuleEnable)
    {
        float *out = (float*)outputBuffer;
        for (unsigned int i = 0; i < framesPerBuffer; i++) {
            if(!m_bEndOfFile){
                *out++ += *m_pAudioData++;
                if(m_pAudioData == m_pEndFrame) m_bEndOfFile = true;
                *out++ += *m_pAudioData++;
                if(m_pAudioData == m_pEndFrame) m_bEndOfFile = true;
            }else{
                *out++ += 0.0;
                *out++ += 0.0;
            }
        }
    }
}

void AudioPlayer::eneble()
{
    qDebug() << "Audio Player is Enable";
    m_bIsModuleEnable = true;
}

void AudioPlayer::disable()
{
    qDebug() << "Audio Player is Disable";
    m_bIsModuleEnable = false;
}

bool AudioPlayer::isEnabled()
{
    return m_bIsModuleEnable;
}

void AudioPlayer::readAudioFile(char* pFilename)
{
    SF_INFO sf_info;
    qDebug() << "Read: " << pFilename;
    SNDFILE* sndfile = sf_open(pFilename, SFM_READ, &sf_info);
    if (NULL == sndfile)
    {
        qDebug() << "can't read sound file " << sf_strerror(sndfile);
    }
    m_pAudioData = (float*)malloc(sf_info.frames * sf_info.channels * sizeof(float));
    //request all the frames
    sf_count_t num_frames_read = sf_readf_float(sndfile, m_pAudioData, sf_info.frames);
    qDebug() << "Num frame read: " << num_frames_read;
    //calculate number of frame and channel
    m_nNumFrame = (sf_info.frames * sf_info.channels);
    qDebug() << "Calculate num frame: "<< m_nNumFrame;
    m_pStartFrame   = &m_pAudioData[0];
    m_pEndFrame     = &m_pAudioData[m_nNumFrame-1];
    m_nCurrentFrame = 0;
    m_bEndOfFile    = false;
    sf_close(sndfile);
}

const float* AudioPlayer::get_AudioData()
{
    return m_pStartFrame;
}

unsigned long AudioPlayer::get_NumberOfSample()
{
    return m_nNumFrame;
}

void AudioPlayer::set_AudioFilePath(QString strFilePath)
{
    m_strAudioPath = strFilePath;
    readAudioFile(m_strAudioPath.toLocal8Bit().data());
}
