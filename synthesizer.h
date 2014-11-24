#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H
#include "audiodevicebase.h"
#include "testmodule.h"

#define WAVETABLE_SAMPLE_RATE 1024

enum AudioBitDepth{
    AudioBitDepthunknown    = 0,
    AudioBitDepth8Bit       = 1,
    AudioBitDepth16Bit      = 2,
    AudioBitDepth32Bit      = 3,
    AudioBitDepth64Bit      = 4
};

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
    ~Synthesizer();

    void put_AudioFrequency(double dblFrequency);
    virtual void process(const void *inputBuffer, void *outputBuffer, const unsigned long framesPerBuffer);

private:
    AudioData           *m_SWaveData;
    AudioDeviceBase     *m_AudioDeviceBase;
    void SetData();
    void AdjustDataTable();
    double              m_dblAudioFrequency;
    double              m_dblDefaultAudioFrequency;



    //-------------wavetable oscillator------------------- ---
    float m_SinArray[WAVETABLE_SAMPLE_RATE];            //sine
    float m_SawToothArray[WAVETABLE_SAMPLE_RATE];       //saw
    float m_TriangleArray[WAVETABLE_SAMPLE_RATE];       //Triangle
    float m_SquareArray[WAVETABLE_SAMPLE_RATE];         //Square

    float m_fReadIndex;                                 //index to read wave data
    float m_fIncreament;                                //increament value
    OscillatorType m_eOscType;
    void cookFrequency();



};

#endif // SYNTHESIZER_H
