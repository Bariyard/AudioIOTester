#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H
#include "audiodevicebase.h"
#include "testmodule.h"

#define WAVETABLE_SAMPLE_RATE 1024

enum OscillatorType{
    Undefined = 0,
    Sinusoid = 1,
    Sawtooth = 2,
    Triangle = 3,
    Square   = 4
};


class Synthesizer : public TestModule
{
public:
    Synthesizer(AudioDeviceBase* s);
    virtual ~Synthesizer();

    virtual void        reset();
    virtual void        process(const void *inputBuffer, void *outputBuffer, const unsigned long framesPerBuffer);

    virtual void        eneble();
    virtual void        disable();
    virtual bool        isEnabled();

    void                put_Frequency(double dblFrequency);
    float               get_Frequency();
    int                 get_WaveformType();
    void                put_WaveformType(int nType);


private:
    AudioDeviceBase     *m_AudioDeviceBase;
    double              m_dblAudioFrequency;
    double              m_dblDefaultAudioFrequency;

    //-------------wavetable oscillator------------------- ---
    float               m_SinArray[WAVETABLE_SAMPLE_RATE];            //sine
    float               m_SawToothArray[WAVETABLE_SAMPLE_RATE];       //saw
    float               m_TriangleArray[WAVETABLE_SAMPLE_RATE];       //Triangle
    float               m_SquareArray[WAVETABLE_SAMPLE_RATE];         //Square

    float               m_fReadIndex;                                 //index to read wave data
    float               m_fIncreament;                                //increament value
    OscillatorType      m_eOscType;
    bool                m_bIsModuleEnable;

};

#endif // SYNTHESIZER_H
