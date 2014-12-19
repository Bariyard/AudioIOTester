#include "synthesizer.h"

Synthesizer::Synthesizer(AudioDeviceBase* pAudioDeviceBase)
{
    m_pAudioDeviceBase  = pAudioDeviceBase;
    m_bIsModuleEnable   = false;
}

Synthesizer::~Synthesizer()
{

}

void Synthesizer::reset()
{

}

void Synthesizer::process(const void *inputBuffer, void *outputBuffer, const unsigned long framesPerBuffer)
{
    if(!m_bIsModuleEnable)
        return;

    if(!m_pSoundModule.isEmpty())
    {
        //process each test module in the list
        foreach( SoundModule *pMod, m_pSoundModule )
        {
            pMod->process(inputBuffer,outputBuffer,framesPerBuffer);
        }
    }
}

void Synthesizer::eneble()
{
    m_bIsModuleEnable = true;
}

void Synthesizer::disable()
{
    m_bIsModuleEnable = false;
}

bool Synthesizer::isEnabled()
{
    return m_bIsModuleEnable;
}

void Synthesizer::RegisterSoundModule(SoundModule * pSoundModule)
{
    m_pSoundModule.push_back(pSoundModule);
}
