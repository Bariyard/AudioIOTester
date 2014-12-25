#include "lfo.h"
#include "utils.h"

LFO::LFO(AudioDeviceBase *pAudioDeviceBase)
{
    m_pAudioDeviceBase  = pAudioDeviceBase;
    m_bIsModuleEnable   = false;
    m_dblFrequency      = 2;
    m_eOscillatorType   = OscillatorType::Sinusoid;
    m_pOscillator       = new Oscillator(m_pAudioDeviceBase);
    m_pOscillator->put_Frequency(m_dblFrequency);
    m_pOscillator->put_WaveformType(m_eOscillatorType);
    m_pOscillator->eneble();
}

LFO::~LFO()
{
    delete m_pOscillator;
}

void LFO::reset()
{

}

void LFO::process(const void */*inputBuffer*/, void *outputBuffer, const unsigned long framesPerBuffer)
{
    if(m_bIsModuleEnable)
    {
        float *out = (float*)outputBuffer;
        for (unsigned int i = 0; i < framesPerBuffer; i++)
        {
            float fOutSample = m_pOscillator->GenerateWaveformSample();
            fOutSample = scaling(-1, 1, 0, 1 ,fOutSample);
            *out++ *= fOutSample;
            if(m_pAudioDeviceBase->get_OutputNumChanel() == AudioChannelType::Stereo)
                *out++ *= fOutSample;
        }
    }
}

void LFO::eneble()
{
    m_bIsModuleEnable = true;
}

void LFO::disable()
{
    m_bIsModuleEnable   = false;
}

bool LFO::isEnabled()
{
    return m_bIsModuleEnable;
}

void LFO::put_LFOType(OscillatorType eType)
{
    m_eOscillatorType = eType;
    m_pOscillator->put_WaveformType(m_eOscillatorType);
}

OscillatorType LFO::get_LFOType()
{
    return m_eOscillatorType;
}

QString* LFO::get_WaveformTypeString()
{
    return m_pOscillator->get_WaveformTypeString();
}

void LFO::put_Frequency(double dblFreq)
{
    m_dblFrequency = dblFreq;
    m_pOscillator->put_Frequency(m_dblFrequency);
}

double LFO::get_Frequency()
{
    return m_dblFrequency;
}
