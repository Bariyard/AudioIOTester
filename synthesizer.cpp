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
    //-------------wavetable oscillator----------------------
    m_fReadIndex = 0.0;
    m_fIncreament = 1024 * 220.0/m_AudioDeviceBase->get_SamplingRate();
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

    m_AudioDeviceBase->registerTestModule(this);


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
    //for(int i=0; i<samplingRate; i++ )
   // {
        //m_SWaveData->data[i] = (double) 0.6* sin(((double)i * (M_PI * 2.0) * m_dblAudioFrequency )/(double)samplingRate);.
       // m_SWaveData->data[i] = (double) 0.6* m_SinArray[(int)m_fReadIndex];
//        switch(m_eOscType){
//            case Undefined:
//            case Sinusoid : m_SWaveData->data[i] = m_SinArray[(int)m_fReadIndex] ;break;
//            case Sawtooth : m_SWaveData->data[i] = m_SawToothArray[(int)m_fReadIndex];break;
//            case Triangle : m_SWaveData->data[i] = m_TriangleArray[(int)m_fReadIndex];break;
//            case Square   : m_SWaveData->data[i] =  m_SquareArray[(int)m_fReadIndex];break;
//            default:
//                m_SWaveData->data[i] = (double) 0.6* sin(((double)i * (M_PI * 2.0) * m_dblAudioFrequency )/(double)samplingRate);
//                break;
//        }
      //  m_fReadIndex += m_fIncreament;
   // }
}

void Synthesizer::put_AudioFrequency(double dblFrequency)
{
    m_dblAudioFrequency = dblFrequency;
    //-------------wavetable oscillator----------------------
    //cook frequency
    m_fReadIndex = 0.0;
    m_fIncreament = WAVETABLE_SAMPLE_RATE * dblFrequency/(float)m_AudioDeviceBase->get_SamplingRate();


    AdjustDataTable();
    m_AudioDeviceBase->put_DataTable(m_SWaveData);
}


void Synthesizer::process(const void *inputBuffer, void *outputBuffer, const unsigned long framesPerBuffer)
{

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




    float *out = (float*)outputBuffer;
    for (int i = 0; i < framesPerBuffer; i++) {
        //         switch(m_eOscType){
        //             case Undefined:
        //             case Sinusoid : out[i] += m_SinArray[(int)m_fReadIndex] ;break;
        //             case Sawtooth : out[i] += m_SawToothArray[(int)m_fReadIndex];break;
        //             case Triangle : out[i] += m_TriangleArray[(int)m_fReadIndex];break;
        //             case Square   : out[i] += m_SquareArray[(int)m_fReadIndex];break;
        //             default:
        //                 out[i] += (double) 0.6* sin(((double)i * (M_PI * 2.0) * m_dblAudioFrequency )/m_AudioDeviceBase->get_SamplingRate());
        //                 break;
        //         }
        float fOutSample = 0.0;

        int nReadIndex = (int)m_fReadIndex;
        float fFrac = m_fReadIndex - nReadIndex;   //fractional part
        int nReadIndexNext = nReadIndex + 1 >1023 ? 0 : nReadIndex +1;
        fOutSample = (float)linear_interpolation(0,1,m_TriangleArray[nReadIndex], m_TriangleArray[nReadIndexNext],fFrac);
        *out++ = 0.5* fOutSample;
//        if(true && i+1 != framesPerBuffer){ //chanel is stereo
//            i++;
            *out++ = 0.5* fOutSample;
//        }
        m_fReadIndex += m_fIncreament;
        if(m_fReadIndex > 1024)m_fReadIndex = m_fReadIndex - 1024;
    }
}
