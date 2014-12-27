#include "audioplayer.h"
#include "sndfile.h"
#include "waveform.h"

AudioPlayer::AudioPlayer(AudioDeviceBase* pAudioDeviceBase)
{
    m_pAudioDeviceBase  = pAudioDeviceBase;
    m_bIsModuleEnable   = false;
    m_bIsLooping        = false;
    m_nCurrentNumFrame  = 0;
    m_pAudioFile        = new AudioFile();
    m_pAudioDeviceBase->RegisterTestModule(this);
}

AudioPlayer::~AudioPlayer()
{
    delete m_pAudioFile;
}

void AudioPlayer::reset()
{

}

void AudioPlayer::process(const void */*inputBuffer*/, void *outputBuffer, const unsigned long framesPerBuffer)
{
    if(m_pAudioFile->IsEndOfFile())
        return;

    if(m_bIsModuleEnable)
    {
        float *out = (float*)outputBuffer;
        for (unsigned int i = 0; i < framesPerBuffer; i++) {
            if(m_pAudioFile && !m_pAudioFile->IsEndOfFile()){
                *out++ += *m_pCurrentFrame++; m_nCurrentNumFrame++;
                CheckCurrentFrame();
                if(m_pAudioDeviceBase->get_OutputNumChanel() == AudioChannelType::Stereo){
                    *out++ += *m_pCurrentFrame++; m_nCurrentNumFrame++;
                    CheckCurrentFrame();
                }
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

const float* AudioPlayer::get_AudioData()
{
    return m_pAudioFile->get_StartFrame();
}

unsigned long AudioPlayer::get_NumberOfSample()
{
    return m_pAudioFile->get_NumFrame();
}

unsigned long AudioPlayer::get_CurrentNumFrame()
{
    return m_pCurrentFrame;
}

void AudioPlayer::set_AudioFilePath(QString strFilePath)
{
    if(m_pAudioFile->get_AudioFilePath() != strFilePath)
    {
        AudioFile *audioFile = new AudioFile();
        audioFile->Open(strFilePath.toLocal8Bit().data());
        m_pAudioFile = audioFile;
        m_pCurrentFrame = m_pAudioFile->get_StartFrame();
    }
}

void AudioPlayer::set_Looping(bool bIsLoop)
{
    m_bIsLooping = bIsLoop;

    qDebug() << "Set looping: " << QString(bIsLoop?"true":"false");
    if(m_pAudioFile->IsEndOfFile())
    {
        m_pCurrentFrame = m_pAudioFile->get_StartFrame();
        m_pAudioFile->set_EndOfFile(false);
    }
}

void AudioPlayer::CheckCurrentFrame()
{
    if(m_pCurrentFrame == m_pAudioFile->get_EndFrame())
    {
        if(m_bIsLooping)
            m_pCurrentFrame = m_pCurrentFrame = m_pAudioFile->get_StartFrame();
        else
            m_pAudioFile->set_EndOfFile(true);
    }
}
