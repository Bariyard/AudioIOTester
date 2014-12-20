#ifndef SYNTHESIZERVIEW_H
#define SYNTHESIZERVIEW_H

#include <QWidget>
#include "audiodevicebase.h"
#include "synthesizer.h"
#include "oscillator.h"
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
    void UpdateWaveformTypeOsc1(int nType);
    void UpdateFrequencyOsc1(double dblFreq);
    void UpdateGainOsc1(int nGain);
    //Osc2
    void UpdateWaveformTypeOsc2(int nType);
    void UpdateFrequencyOsc2(double dblFreq);
    void UpdateGainOsc2(int nGain);
    void UpdateEnebleOsc2(bool bIsEneble);
private:
    Ui::SynthesizerView         *ui;
    AudioDeviceBase             *m_pAudioDeviceBase;
    Synthesizer                 *m_pSynthesizer;
    Oscillator                  *m_pOscillator;
    Oscillator                  *m_pOscillator2;

    DoubleSlider                *m_pFrequencySlider;
    DoubleSlider                *m_pFrequencySlider2;

};

#endif // SYNTHESIZERVIEW_H
