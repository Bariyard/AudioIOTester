#ifndef LFO_H
#define LFO_H
#include "audiodevicebase.h"
#include "testmodule.h"
#include "oscillator.h"

class LFO: public SoundModule
{
public:
    LFO(AudioDeviceBase *pAudioDeviceBase);
    virtual ~LFO();

    virtual void        reset();
    virtual void        process(const void *inputBuffer, void *outputBuffer, const unsigned long framesPerBuffer);

    virtual void        eneble();
    virtual void        disable();
    virtual bool        isEnabled();

    void                put_LFOType(OscillatorType eType);
    OscillatorType      get_LFOType();
    QString*            get_WaveformTypeString();
    void                put_Frequency(double dblFreq);
    double              get_Frequency();

private:
    AudioDeviceBase     *m_pAudioDeviceBase;
    bool                m_bIsModuleEnable;
    Oscillator          *m_pOscillator;
    double              m_dblFrequency;
    OscillatorType      m_eOscillatorType;
    float               m_fWaveDataArray[WAVETABLE_SAMPLE_RATE*2];

};

#endif // LFO_H
