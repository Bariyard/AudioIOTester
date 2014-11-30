#include "microphone.h"

Microphone::Microphone(AudioDeviceBase* pAudioDeviceBase)
{
    m_pAudioDeviceBase   = pAudioDeviceBase;
    m_bIsModuleEnable   = false;
    m_fMicVolumn        = 0.7;
    m_pAudioDeviceBase->RegisterTestModule(this);
}

Microphone::~Microphone()
{

}

void Microphone::reset()
{

}

void Microphone::process(const void *inputBuffer, void *outputBuffer, const unsigned long framesPerBuffer)
{
    if(m_bIsModuleEnable)
    {
        float *out = (float*)outputBuffer;
        float *in  = (float*)inputBuffer;
        for (unsigned int i = 0; i < framesPerBuffer; i++)
        {
            *out++ += (float)m_fMicVolumn * (*in++);
            *out++ += (float)m_fMicVolumn * (*in++);
        }
        if(m_pAmpMonitor)
              m_pAmpMonitor->set_AmpLevel(in, framesPerBuffer);
    }
}

void Microphone::eneble()
{
    m_bIsModuleEnable = true;
}

void Microphone::disable()
{
    m_bIsModuleEnable = false;
}

bool Microphone::isEnabled()
{
    return m_bIsModuleEnable;
}

void Microphone::put_MicrophoneVolumn(float fVolumn)
{
    m_fMicVolumn = fVolumn;
}

float Microphone::get_MicrophoneVolumn()
{
    return m_fMicVolumn;
}

void Microphone::RegisterAmplitudeMonitor(AmplitudeMonitor * pAmplitudeMonitor)
{
    m_pAmpMonitor = pAmplitudeMonitor;
}
