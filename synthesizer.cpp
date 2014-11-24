#include "synthesizer.h"
#include <math.h>

#ifndef M_PI
#define M_PI (3.14159265)
#endif

Synthesizer::Synthesizer(AudioDeviceBase* s):
    m_dblAudioFrequency(220.00),
    m_dblDefaultAudioFrequency(220.00)
{
    m_AudioDeviceBase = s;
    m_bIsModuleEnable = true;
    m_dblAudioFrequency = m_dblDefaultAudioFrequency;

    //-------------wavetable oscillator----------------------
    m_fReadIndex = 0.0;
    m_fIncreament = WAVETABLE_SAMPLE_RATE * m_dblAudioFrequency/m_AudioDeviceBase->get_SamplingRate();
    m_eOscType = Sinusoid;

    //slope and y-intercept value for the
    //Triangle Wave
    //rising edge 1
    float mt1 = 1.0/256.0;
    float bt1 = 0.0;

    //rising edge 2
    float mt2 = 1.0/256.0;
    float bt2 = -1.0;

    //falling edge
    float mtf2 = -2.0/512.0;
    float btf2 = +1.0;

    //sawtooth
    //rising edge1
    float ms1 = 1.0/512.0;
    float bs1 = 0.0;

    //rising edge2
    float ms2 = 1.0/512.0;
    float bs2 = -1.0;

    //set up wave table
    for(int i = 0; i < 1024; i++){
        //sample the sinusoid 1024 point
        //sin(wnT) = sin(2pi*i/1024)
        m_SinArray[i] = sin((double)i/1024.0 * (M_PI*2.));


        //Sawtooth
        m_SawToothArray[i] = i < 512 ? ms1* i + bs1 : ms2*(i-511) +bs2;

        //Triangle
        if(i < 256)
            m_TriangleArray[i] = mt1*i + bt1; // rising edge1
        else if( i >= 256 && i < 768)
            m_TriangleArray[i] = mtf2*(i-256) + btf2; // falling edge
        else
            m_TriangleArray[i] = mt2 *(i-768) + bt2; //rising edge2

        //square
        m_SquareArray[i] = i < 512? +1.0 : -1.0;
    }

    //register testing module to audiodevicebase
    m_AudioDeviceBase->registerTestModule(this);
}

Synthesizer::~Synthesizer()
{

}

void Synthesizer::put_AudioFrequency(double dblFrequency)
{
    m_dblAudioFrequency = dblFrequency;
    m_fReadIndex = 0.0;
    m_fIncreament = WAVETABLE_SAMPLE_RATE * m_dblAudioFrequency/(float)m_AudioDeviceBase->get_SamplingRate();
}


void Synthesizer::process(const void *inputBuffer, void *outputBuffer, const unsigned long framesPerBuffer)
{
    //lambda function for calculate linear interpolation
    auto linear_interpolation = [](float x1, float x2, float y1, float y2, float x)
    {
        float denom = x2 - x1;
        if(denom == 0)
            return y1; // should never happen
        // calculate decimal position of x;
        float dx = (x - x1)/(x2 - x1);
        // use weighted sum method of interpolating
        float result = dx*y2 + (1-dx)*y1;
        return result;
    };

    if(m_bIsModuleEnable)
    {
        float *out = (float*)outputBuffer;
        float *in  = (float*)inputBuffer;

        for (unsigned int i = 0; i < framesPerBuffer; i++) {
            float fOutSample = 0.0;
            int nReadIndex = (int)m_fReadIndex;
            float fFrac = m_fReadIndex - nReadIndex;   //fractional part
            int nReadIndexNext = nReadIndex + 1 >1023 ? 0 : nReadIndex +1;

            switch (m_eOscType) {
            case Sinusoid:
                fOutSample = (float)linear_interpolation(0, 1, m_SinArray[nReadIndex],
                                                         m_SinArray[nReadIndexNext], fFrac);
                break;
            case Sawtooth:
                fOutSample = (float)linear_interpolation(0, 1, m_SawToothArray[nReadIndex],
                                                         m_SawToothArray[nReadIndexNext], fFrac);
                break;
            case Triangle:
                fOutSample = (float)linear_interpolation(0, 1, m_TriangleArray[nReadIndex],
                                                         m_TriangleArray[nReadIndexNext], fFrac);
                break;
            case Square:
                fOutSample = (float)linear_interpolation(0, 1, m_SquareArray[nReadIndex],
                                                         m_SquareArray[nReadIndexNext], fFrac);
                break;
            default:
                fOutSample = 0.0;
                break;
            }

            //Hardcoded for stereo 2 channel
            *out++ += fOutSample;
            *out++ += fOutSample;
            m_fReadIndex += m_fIncreament;
            if(m_fReadIndex > WAVETABLE_SAMPLE_RATE)m_fReadIndex = m_fReadIndex - WAVETABLE_SAMPLE_RATE;
        }
    }
}

bool Synthesizer::isEnabled()
{
    return m_bIsModuleEnable;
}

int Synthesizer::get_WaveformType()
{
    return m_eOscType;
}

void Synthesizer::put_WaveformType(int nType)
{
    m_eOscType = OscillatorType(nType);
}
