#include "audioplayer.h"
#include "sndfile.h"
#include "waveform.h"

AudioPlayer::AudioPlayer(AudioDeviceBase* pAudioDeviceBase)
{
    m_pAudioDeviceBase  = pAudioDeviceBase;
    m_bIsModuleEnable   = false;
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
                *out++ += *m_pCurrentFrame++;
                if(m_pCurrentFrame == m_pAudioFile->get_EndFrame())
                    m_pAudioFile->set_EndOfFile(true);
                *out++ += *m_pCurrentFrame++;
                if(m_pCurrentFrame == m_pAudioFile->get_EndFrame())
                    m_pAudioFile->set_EndOfFile(true);
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
