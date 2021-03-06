#ifndef FILTER_H
#define FILTER_H
#include "audiodevicebase.h"
#include "testmodule.h"




enum  FilterType{
    LowPass  = 0,
    HighPass = 1,
    BandPass = 2,
    Notch    = 3
};

class Filter : public SoundModule
{
public:
    Filter(AudioDeviceBase *pAudioDeviceBase);
    virtual ~Filter();

    virtual void        reset();
    virtual void        process(const void *inputBuffer, void *outputBuffer, const unsigned long framesPerBuffer);

    virtual void        eneble();
    virtual void        disable();
    virtual bool        isEnabled();

    //filter parameter
    QString*            get_FilterTypeString();
    FilterType          get_FilterType();
    void                put_FilterType(FilterType eType);
    double              get_Frequency();
    void                put_Frequency(double dblFreq);
    double              get_Resonance();
    void                put_Resonance(double dblRes);

private:
    AudioDeviceBase     *m_pAudioDeviceBase;
    double              m_nSampleRate;
    bool                m_bIsModuleEnable;
    FilterType          m_eFilterType;

    double              doFilter(const double input);
    void                doFilter2();
    float               doBiquad(float input);

    double              m_dblCutOffFrequency;
    double              m_dblResonance;
    //filter algorithm variable
    float              a0, a1, a2, b1, b2, c;
    float              in1, in2, out1, out2, output;
    float              pi;
};

#endif // FILTER_H
