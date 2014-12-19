#ifndef OSCILLATOR_H
#define OSCILLATOR_H
#include "audiodevicebase.h"
#include "testmodule.h"

#define WAVETABLE_SAMPLE_RATE 1024
#define MINIMUM_FREQUENCY 16.35
#define MAXIMUM_FREQUENCY 7902.13


enum OscillatorType{
    Undefined = 0,
    Sinusoid = 1,
    Sawtooth = 2,
    Triangle = 3,
    Square   = 4
};

class Oscillator : public SoundModule
{
public:
    Oscillator(AudioDeviceBase* pAudioDeviceBase);
    virtual ~Oscillator();

    virtual void        reset();
    virtual void        process(const void *inputBuffer, void *outputBuffer, const unsigned long framesPerBuffer);

    virtual void        eneble();
    virtual void        disable();
    virtual bool        isEnabled();

    void                put_Frequency(double dblFrequency);
    float               get_Frequency();
    float               get_MinimumFrequency();
    float               get_MaximumFrequency();

    int                 get_WaveformType();
    void                put_WaveformType(int nType);
    QString*            get_WaveformTypeString();
    double              get_Gain();
    void                put_Gain(double gain);

private:
    AudioDeviceBase     *m_pAudioDeviceBase;
    double              m_dblAudioFrequency;
    double              m_dblDefaultAudioFrequency;
    double              m_dblGain;

    //-------------wavetable oscillator------------------- ---
    float               m_fSinArray[WAVETABLE_SAMPLE_RATE];            //sine
    float               m_fSawToothArray[WAVETABLE_SAMPLE_RATE];       //saw
    float               m_fTriangleArray[WAVETABLE_SAMPLE_RATE];       //Triangle
    float               m_fSquareArray[WAVETABLE_SAMPLE_RATE];         //Square

    float               m_fReadIndex;                                 //index to read wave data
    float               m_fIncreament;                                //increament value
    OscillatorType      m_eOscType;
    bool                m_bIsModuleEnable;

};

#endif // OSCILLATOR_H
