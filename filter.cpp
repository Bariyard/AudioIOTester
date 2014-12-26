#include "filter.h"
#include "math.h"
#include "utils.h"
#ifndef M_PI
#define M_PI (3.14159265)
#endif

QString FILTER_TYPE_STRING[] = {"Low Pass", "High Pass", "Band Pass", "Notch"};

Filter::Filter(AudioDeviceBase *pAudioDeviceBase)
{
    m_pAudioDeviceBase      = pAudioDeviceBase;
    m_bIsModuleEnable       = false;
    m_eFilterType           = LowPass;
    m_dblCutOffFrequency    = 1000;
    m_dblResonance          = 0.707;
    reset();

}

Filter::~Filter()
{

}

void Filter::reset()
{
    a0 = 0, a1 = 0, a2 = 0, b1 = 0, b2 = 0, c = 0;
    in1 = 0, in2 = 0, out1 = 0, out2 = 0, output = 0;
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
            doFilter2();
            //            float fOutSample = doFilter(*out);
            float fOutSample = doBiquad(*out);
            *out = (float)fOutSample; out++;
            if(m_pAudioDeviceBase->get_OutputNumChanel() == AudioChannelType::Stereo)
            {
                fOutSample = doBiquad(*out);
                *out = (float)fOutSample; out++;
            }
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

//double Filter::doFilter(double input)
//{
//    if (m_dblCutOffFrequency == 0.00) {
//        m_dblCutOffFrequency = 0.01;
//    }
//    m_dblCutOffFrequency = (m_dblCutOffFrequency/100.00)*m_nSampleRate/2.0;
//    m_dblResonance = 1.41421356-((m_dblResonance/100)*(1.41421356-0.1));
//    switch (m_eFilterType) {
//    case LowPass:{
//        c = 1.0 / tan(pi * m_dblCutOffFrequency / m_nSampleRate);
//        a1 = 1.0 / ( 1.0 + ((m_dblResonance * c) + (c * c)));
//        a2 = (2* a1);
//        a3 = a1;
//        b1 = 2.0 * ( 1.0 - (c*c)) * a1;
//        b2 = ( 1.0 - (m_dblResonance * c) + (c * c)) * a1;
//    }break;
//    case HighPass:{
//        c = tan(pi * m_dblCutOffFrequency / m_nSampleRate);
//        a1 = 1.0 / ( 1.0 + ((m_dblResonance * c) + (c * c)));
//        a2 = (-2*a1);
//        a3 = a1;
//        b1 = 2.0 * ( (c*c) - 1.0) * a1;
//        b2 = ( 1.0 - (m_dblResonance * c) + (c * c)) * a1;
//    }break;
//    case BandPass:{
//        //not implement
//        return input;
//    }break;
//    default:
//        return input;
//        break;
//    }

//    output = a1 * input +
//            a2 * in1 +
//            a3 * in2 -
//            b1 * out1 -
//            b2 * out2;

//    in2 = in1;
//    in1 = input;

//    out2 = out1;
//    out1 = output;
//    return output;
//}

void Filter::doFilter2()
{
    // LP HP
    float theta_c, d, fBetaNumerator, fBetaDenominator, fBeta, fGamma, fAlpha;
    theta_c = 2.0*M_PI*m_dblCutOffFrequency/(float)m_nSampleRate;
    //prepare
    switch(m_eFilterType)
    {
        case LowPass:
        case HighPass:{
            d = 1.0/m_dblResonance;

            // intermediat value
            fBetaNumerator    = 1.0 - ((d/2.0)*sin(theta_c));
            fBetaDenominator  = 1.0 + ((d/2.0)*sin(theta_c));

            // beta
            fBeta = 0.5*(fBetaNumerator/fBetaDenominator);

            // gamma
            fGamma = (0.5 + fBeta)*(cos(theta_c));

            // alpha
            fAlpha = 0.0;
        }break;
        case BandPass:
        case Notch :{
            d = 2.0/m_dblResonance;

            // intermediat value
            fBetaNumerator      = 1.0 - tan(theta_c/d);
            fBetaDenominator    = 1.0 + tan(theta_c/d);

            // beta
            fBeta = 0.5 * (fBetaNumerator/fBetaDenominator);

            // gamma
            fGamma = (0.5 + fBeta)*(cos(theta_c));
        }break;

    }

    //calculate biquad factor
    switch (m_eFilterType)
    {
        case LowPass:
        {
            fAlpha = (0.5 + fBeta - fGamma)/2.0;
            a0 = fAlpha;
            a1 = 2.0 * fAlpha;
            a2 = fAlpha;
            b1 = -2.0*fGamma;
            b2 = 2.0*fBeta;
        }break;
        case HighPass:
        {
            fAlpha = (0.5 + fBeta + fGamma)/2.0;
            a0 = fAlpha;
            a1 = -2.0 * fAlpha;
            a2 = fAlpha;
            b1 = -2.0*fGamma;
            b2 = 2.0*fBeta;
        }break;
        case BandPass:
        {
            fAlpha = 0.5 - fBeta;
            a0 = fAlpha;
            a1 = 0.0;
            a2 = -(fAlpha);
            b1 = -2.0*fGamma;
            b2 = 2.0*fBeta;
        }break;
        case Notch:
        {
            fAlpha = 0.5 + fBeta;
            a0 = fAlpha;
            a1 = -2.0*fGamma;
            a2 = fAlpha;
            b1 = -2.0*fGamma;
            b2 = 2.0*fBeta;
        }break;
    }
}

float Filter::doBiquad(float fIn)
{
    output = (a0 * fIn) +
            (a1 * in1) +
            (a2 * in2) -
            (b1 * out1) -
            (b2 * out2);

    in2 = in1;
    in1 = fIn;

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
    doFilter2();
}

double Filter::get_Frequency()
{
    return m_dblCutOffFrequency;
}

void Filter::put_Frequency(double dblFreq)
{
    m_dblCutOffFrequency = dblFreq;
    doFilter2();
}

double Filter::get_Resonance()
{
    return m_dblResonance;
}

void Filter::put_Resonance(double dblRes)
{
    m_dblResonance = scaling(0, 100, 0.5, 20,dblRes);
    doFilter2();
}
