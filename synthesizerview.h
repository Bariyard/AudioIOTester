#ifndef SYNTHESIZERVIEW_H
#define SYNTHESIZERVIEW_H

#include <QWidget>
#include "audiodevicebase.h"
#include "oscillator.h"
#include "CustomComponent/doubleslider.h"

namespace Ui {
class SynthesizerView;
}

class SynthesizerView : public QWidget
{
    Q_OBJECT

public:
    SynthesizerView(AudioDeviceBase* pAudioDeviceBase);
    ~SynthesizerView();
    void Eneble();
    void Disable();

public slots:
    //Synthesizer
    void UpdateFrequency(double dblFreq);
    void UpdateWaveformType(int nType);

private:
    Ui::SynthesizerView         *ui;
    AudioDeviceBase             *m_pAudioDeviceBase;
    Oscillator                  *m_pOscillator;
    DoubleSlider                *m_pFrequencySlider;

};

#endif // SYNTHESIZERVIEW_H
