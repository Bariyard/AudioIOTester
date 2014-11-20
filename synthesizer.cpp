#include "synthesizer.h"
#include <math.h>

#ifndef M_PI
#define M_PI (3.14159265)
#endif

Synthesizer::Synthesizer(AudioDeviceBase* s):
    m_dblAudioFrequency(440.00),
    m_dblDefaultAudioFrequency(440.00)
{
    m_AudioDeviceBase = s;
    SetData();
    AdjustDataTable();
    m_AudioDeviceBase->put_DataTable(m_SWaveData);
}


Synthesizer::~Synthesizer()
{

}

void Synthesizer::SetData()
{
    m_SWaveData = new AudioData;
    m_SWaveData->data       = new float[m_AudioDeviceBase->get_SamplingRate()];
    m_SWaveData->left_phase = 0;
    m_SWaveData->right_phase = 0;
    m_SWaveData->framesToGo = 0;
}

void Synthesizer::AdjustDataTable()
{
    int samplingRate = m_AudioDeviceBase->get_SamplingRate();
    for(int i=0; i<samplingRate; i++ )
    {
        m_SWaveData->data[i] = (double) 0.6* sin(((double)i * (M_PI * 2.0) * m_dblAudioFrequency )/(double)samplingRate);
    }
}

void Synthesizer::put_AudioFrequency(double dblFrequency)
{
    m_dblAudioFrequency = dblFrequency;
    AdjustDataTable();
    m_AudioDeviceBase->put_DataTable(m_SWaveData);
}
