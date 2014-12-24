#include "filter.h"
#include "math.h"
#ifndef M_PI
#define M_PI (3.14159265)
#endif

QString FILTER_TYPE_STRING[] = {"Low Pass", "High Pass"};

Filter::Filter(AudioDeviceBase *pAudioDeviceBase)
{
    m_pAudioDeviceBase  = pAudioDeviceBase;
    m_bIsModuleEnable   = false;
    m_eFilterType       = LowPass;
    m_dblFrequency      = 99.0;
    m_dblResonance      = 2;
    reset();
}

Filter::~Filter()
{

}

void Filter::reset()
{
    a1 = 0, a2 = 0, a3 = 0, b1 = 0, b2 = 0, c = 0;
    in1 = 0, in2 = 0, out1 = 0, out2 = 0, output = 0, input = 0;
    pi	= M_PI;
    m_nSampleRate = m_pAudioDeviceBase->get_SamplingRate();
}

void Filter::process(const void */*inputBuffer*/, void *outputBuffer, const unsigned long framesPerBuffer)
{
    if(m_bIsModuleEnable)
    {
        float *out = (float*)outputBuffer;
        for (unsigned int i = 0; i < framesPerBuffer; i++)
        {
            float fOutSample = doFilter(*out);
            *out = fOutSample; out++;
            fOutSample = doFilter(*out);
            *out = fOutSample; out++;
        }
    }
}

void Filter::eneble()
{
    m_bIsModuleEnable = true;
}

void Filter::disable()
{
    m_bIsModuleEnable = false;
}

bool Filter::isEnabled()
{
    return m_bIsModuleEnable;
}

double Filter::doFilter(double input)
{
    if (m_dblFrequency == 0.00) {
        m_dblFrequency = 0.01;
    }
    m_dblFrequency = (m_dblFrequency/100.00)*m_nSampleRate/2.0;
    m_dblResonance = 1.41421356-((m_dblResonance/100)*(1.41421356-0.1));
    switch (m_eFilterType) {
    case LowPass:{
        c = 1.0 / tan(pi * m_dblFrequency / m_nSampleRate);
        a1 = 1.0 / ( 1.0 + ((m_dblResonance * c) + (c * c)));
        a2 = (2* a1);
        a3 = a1;
        b1 = 2.0 * ( 1.0 - (c*c)) * a1;
        b2 = ( 1.0 - (m_dblResonance * c) + (c * c)) * a1;
    }break;
    case HighPass:{
        c = tan(pi * m_dblFrequency / m_nSampleRate);
        a1 = 1.0 / ( 1.0 + ((m_dblResonance * c) + (c * c)));
        a2 = (-2*a1);
        a3 = a1;
        b1 = 2.0 * ( (c*c) - 1.0) * a1;
        b2 = ( 1.0 - (m_dblResonance * c) + (c * c)) * a1;
    }break;
    case BandPass:{
        //not implement
        return input;
    }break;
    default:
        return input;
        break;
    }

    output = a1 * input +
            a2 * in1 +
            a3 * in2 -
            b1 * out1 -
            b2 * out2;

    in2 = in1;
    in1 = input;

    out2 = out1;
    out1 = output;
    return output;
}

QString* Filter::get_FilterTypeString()
{
    return FILTER_TYPE_STRING;
}

FilterType Filter::get_FilterType()
{
    return m_eFilterType;
}

void Filter::put_FilterType(FilterType eType)
{
    m_eFilterType = eType;
}

double Filter::get_Frequency()
{
    return m_dblFrequency;
}

void Filter::put_Frequency(double dblFreq)
{
    m_dblFrequency = dblFreq;
}

double Filter::get_Resonance()
{
    return m_dblResonance;
}

void Filter::put_Resonance(double dblRes)
{
    m_dblResonance = dblRes;
}
