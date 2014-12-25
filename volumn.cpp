#include "volumn.h"

Volumn::Volumn(AudioDeviceBase* pAudioDeviceBase)
{
    m_pAudioDeviceBase = pAudioDeviceBase;
    m_bIsModuleEnable = true;
    m_dblVolumn = 0.7;
    m_pAudioDeviceBase->RegisterTestModule(this);
}

Volumn::~Volumn()
{

}

void Volumn::reset()
{

}

void Volumn::process(const void */*inputBuffer*/, void *outputBuffer, const unsigned long framesPerBuffer)
{
    if(m_bIsModuleEnable)
    {
        float *out = (float*)outputBuffer;
        for (unsigned int i = 0; i < framesPerBuffer; i++) {
            *out = (*out) * m_dblVolumn;  out++;
            *out = (*out) * m_dblVolumn;  out++;
        }
    }
}

void Volumn::eneble()
{
    qDebug() << "Audio Player is Enable";
    m_bIsModuleEnable = true;
}

void Volumn::disable()
{
    qDebug() << "Audio Player is Disable";
    m_bIsModuleEnable = false;
}

bool Volumn::isEnabled()
{
    return m_bIsModuleEnable;
}

void Volumn::put_Volumn(double dblVolumn)
{
    m_dblVolumn = dblVolumn;
}

double Volumn::get_Volumn()
{
    return m_dblVolumn;
}
