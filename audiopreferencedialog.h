#ifndef AUDIOPREFERENCEDIALOG_H
#define AUDIOPREFERENCEDIALOG_H

#include "audiodevicebase.h"
#include "synthesizer.h"
#include "audioplayer.h"
#include "waveform.h"
#include "microphone.h"
#include "amplitudemonitor.h"
#include "globalvolumn.h"
#include <QDialog>
#include <QPointer>

#include "CustomComponent/doubleslider.h"

namespace Ui {
    class AudioPreferenceDialog;
}

class AudioPreferenceDialog : public QDialog
{
    Q_OBJECT

public:
    AudioPreferenceDialog(QWidget *parent = 0, AudioDeviceBase* audioBase = NULL);
    virtual ~AudioPreferenceDialog();

signals:

public slots:
    void StartAudioTest(bool bStartTest);
    void UpdateInputDevice(int nSelectedItem);
    void UpdateOutputDevice(int nSelectedItem);
    void UpdateSamplingRate(int nSelectedItem);
    void UpdateBufferSize(int nSelectedItem);
    //Test module
    void UpdateTestModule(int nCurrentTab);
    //Synthesizer
    void UpdateFrequency(double dblFreq);
    void UpdateWaveformType(int nType);
    //Microphone
    void UpdateMicVolumn(int nVolumn);
    //global volumn
    void UpdateGlobalVolumn(int nVolumn);

private:
    Ui::AudioPreferenceDialog   *ui;
    QString                     m_strWindowTitle;
    AudioDeviceBase             *m_pAudioDeviceBase;
    bool                        m_bIsTesting;

    //test module
    Synthesizer                 *m_pSynth;
    AudioPlayer                 *m_pAudioPlayer;
    Waveform                    *m_pWaveform;
    Microphone                  *m_pMic;
    AmplitudeMonitor            *m_pAmplitudeMonitor;
    GlobalVolumn                *m_pGlobalVolumn;
    DoubleSlider                *m_pFrequencySlider;

    void RetriveInformation();
    void Connect();
};

#endif // AUDIOPREFERENCEDIALOG_H
