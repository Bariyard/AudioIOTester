#ifndef SYNTHESIZERVIEW_H
#define SYNTHESIZERVIEW_H

#include <QWidget>
#include "audiodevicebase.h"
#include "synthesizer.h"
#include "oscillator.h"
#include "filter.h"
#include "lfo.h"
#include "CustomComponent/doubleslider.h"

namespace Ui {
class SynthesizerView;
}

enum Test{
    eFrequencyChange
};

class SynthesizerView : public QWidget
{
    Q_OBJECT

public:
    SynthesizerView(AudioDeviceBase* pAudioDeviceBase);
    ~SynthesizerView();
    void Eneble();
    void Disable();

public slots:
    //Osc1
    void UpdateOsc1WaveformType(int nType);
    void UpdateOsc1Frequency(double dblFreq);
    void UpdateOsc1Gain(int nGain);
    //Osc2
    void UpdateOsc2Eneble(bool bIsEneble);
    void UpdateOsc2WaveformType(int nType);
    void UpdateOsc2Frequency(double dblFreq);
    void UpdateOsc2Gain(int nGain);
    //Filter
    void UpdateFilterEnable(bool bIsEnable);
    void UpdateFilterType(int nType);
    void UpdateFilterCutOffFrequency(int nFreq);
    void UpdateFilterResonance(int nRes);
    //LFO
    void UpdateLFOEnable(bool bIsEnable);
    void UpdateLFOType(int nType);
    void UpdateLFOFrequency(int nFreq);

private:
    Ui::SynthesizerView         *ui;
    AudioDeviceBase             *m_pAudioDeviceBase;
    Synthesizer                 *m_pSynthesizer;
    Oscillator                  *m_pOscillator;
    Oscillator                  *m_pOscillator2;
    Filter                      *m_pFilter;
    LFO                         *m_pLFO;

    DoubleSlider                *m_pFrequencySlider;
    DoubleSlider                *m_pFrequencySlider2;

};

#endif // SYNTHESIZERVIEW_H
