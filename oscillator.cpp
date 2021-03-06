#include "Oscillator.h"
#include <math.h>
#include "utils.h"

#ifndef M_PI
#define M_PI (3.14159265)
#endif

QString WAVEFORM_TYPE_STRING[] = {"sin", "saw", "triangle", "square"};

Oscillator::Oscillator(AudioDeviceBase* pAudioDeviceBase):
    m_dblAudioFrequency(220.00),
    m_dblDefaultAudioFrequency(220.00),
    m_dblGain(1.0)
{
    m_pAudioDeviceBase  = pAudioDeviceBase;
    m_bIsModuleEnable   = false;
    m_dblAudioFrequency = m_dblDefaultAudioFrequency;

    //-------------wavetable oscillator----------------------
    m_fReadIndex    = 0.0;
    m_fIncreament   = WAVETABLE_SAMPLE_RATE * m_dblAudioFrequency/m_pAudioDeviceBase->get_SamplingRate();
    m_eOscType      = Sinusoid;

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
        m_fSinArray[i] = sin((double)i/1024.0 * (M_PI*2.));


        //Sawtooth
        m_fSawToothArray[i] = i < 512 ? ms1* i + bs1 : ms2*(i-511) +bs2;

        //Triangle
        if(i < 256)
            m_fTriangleArray[i] = mt1*i + bt1; // rising edge1
        else if( i >= 256 && i < 768)
            m_fTriangleArray[i] = mtf2*(i-256) + btf2; // falling edge
        else
            m_fTriangleArray[i] = mt2 *(i-768) + bt2; //rising edge2

        //square
        m_fSquareArray[i] = i < 512? +1.0 : -1.0;
    }

    //m_pAudioDeviceBase->RegisterTestModule(this);
}

Oscillator::~Oscillator()
{

}

float Oscillator::get_Frequency()
{
    return m_dblAudioFrequency;
}

void Oscillator::put_Frequency(double dblFrequency)
{
    m_dblAudioFrequency = dblFrequency;
    reset();
}

float Oscillator::get_MinimumFrequency()
{
    return MINIMUM_FREQUENCY;
}

float Oscillator::get_MaximumFrequency()
{
    return MAXIMUM_FREQUENCY;
}

void Oscillator::reset()
{
    m_fReadIndex = 0.0;
    m_fIncreament = WAVETABLE_SAMPLE_RATE * m_dblAudioFrequency/(float)m_pAudioDeviceBase->get_SamplingRate();
}

void Oscillator::process(const void */*inputBuffer*/, void *outputBuffer, const unsigned long framesPerBuffer)
{
    if(m_bIsModuleEnable)
    {
        float *out = (float*)outputBuffer;
        for (unsigned int i = 0; i < framesPerBuffer; i++)
        {
            float fOutSample = GenerateWaveformSample();
            *out++ += fOutSample * m_dblGain;
            if(m_pAudioDeviceBase->get_OutputNumChanel() == AudioChannelType::Stereo)
                *out++ += fOutSample * m_dblGain;
        }
    }
}

float Oscillator::GenerateWaveformSample()
{
    float fOutSample = 0.0;
    int nReadIndex = (int)m_fReadIndex;
    float fFrac = m_fReadIndex - nReadIndex;   //fractional part
    int nReadIndexNext = nReadIndex + 1 >1023 ? 0 : nReadIndex +1;

    switch (m_eOscType) {
    case Sinusoid:
        fOutSample = (float)linear_interpolation(0, 1, m_fSinArray[nReadIndex],
                                                 m_fSinArray[nReadIndexNext], fFrac);
        break;
    case Sawtooth:
        fOutSample = (float)linear_interpolation(0, 1, m_fSawToothArray[nReadIndex],
                                                 m_fSawToothArray[nReadIndexNext], fFrac);
        break;
    case Triangle:
        fOutSample = (float)linear_interpolation(0, 1, m_fTriangleArray[nReadIndex],
                                                 m_fTriangleArray[nReadIndexNext], fFrac);
        break;
    case Square:
        fOutSample = (float)linear_interpolation(0, 1, m_fSquareArray[nReadIndex],
                                                 m_fSquareArray[nReadIndexNext], fFrac);
        break;
    default:
        fOutSample = 0.0;
        break;
    }

    m_fReadIndex += m_fIncreament;
    if(m_fReadIndex > WAVETABLE_SAMPLE_RATE)m_fReadIndex = m_fReadIndex - WAVETABLE_SAMPLE_RATE;
    return fOutSample;
}

void Oscillator::eneble()
{
    m_bIsModuleEnable = true;
}

void Oscillator::disable()
{
    m_bIsModuleEnable = false;
}

bool Oscillator::isEnabled()
{
    return m_bIsModuleEnable;
}

int Oscillator::get_WaveformType()
{
    return m_eOscType;
}

void Oscillator::put_WaveformType(int nType)
{
    m_eOscType = OscillatorType(nType);
    reset();
}

QString* Oscillator::get_WaveformTypeString()
{
    return WAVEFORM_TYPE_STRING;
}

double Oscillator::get_Gain()
{
    return m_dblGain;
}

void Oscillator::put_Gain(double gain)
{
    m_dblGain = gain;
}
