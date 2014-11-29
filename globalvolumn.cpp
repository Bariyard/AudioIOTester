#include "globalvolumn.h"

GlobalVolumn::GlobalVolumn(AudioDeviceBase* s)
{
    m_AudioDeviceBase = s;
    m_bIsModuleEnable = true;
    m_dblGlobalVolumn = 0.7;
    m_AudioDeviceBase->registerTestModule(this);

}

GlobalVolumn::~GlobalVolumn()
{

}

void GlobalVolumn::reset()
{

}


void GlobalVolumn::process(const void */*inputBuffer*/, void *outputBuffer, const unsigned long framesPerBuffer)
{
    if(m_bIsModuleEnable)
    {
            float *out = (float*)outputBuffer;

            for (unsigned int i = 0; i < framesPerBuffer; i++) {
                *out++ = (*out) * m_dblGlobalVolumn;
                *out++ = (*out) * m_dblGlobalVolumn;
            }
    }
}

void GlobalVolumn::eneble()
{
    qDebug() << "Audio Player is Enable";
    m_bIsModuleEnable = true;
}

void GlobalVolumn::disable()
{
    qDebug() << "Audio Player is Disable";
    m_bIsModuleEnable = false;
}

bool GlobalVolumn::isEnabled()
{
    return m_bIsModuleEnable;
}

void GlobalVolumn::set_GlobalVolumn(double dblVolumn)
{
    m_dblGlobalVolumn = dblVolumn;
}

double GlobalVolumn::get_GlobalVolumn()
{
    return m_dblGlobalVolumn;
}