#ifndef AUDIOPREFERENCEDIALOG_H
#define AUDIOPREFERENCEDIALOG_H

#include "audiodevicebase.h"
#include "synthesizer.h"
#include "audioplayer.h"
#include "waveform.h"
#include "microphone.h"
#include "amplitudemonitor.h"
#include <QDialog>
#include <QPointer>

namespace Ui {
class AudioPreferenceDialog;
}

class AudioPreferenceDialog : public QDialog
{
    Q_OBJECT

public:
    AudioPreferenceDialog(QWidget *parent = 0, AudioDeviceBase* audioBase = NULL);
    virtual ~AudioPreferenceDialog();

public slots:
    void StartAudioTest(bool bStartTest);
    void ChangeInputDevice(int nSelectedItem);
    void ChangeOutputDevice(int nSelectedItem);
    void ChangeSamplingRate(int nSelectedItem);
    void ChangeBufferSize();
    void ChangeBufferSizeSlider(int nBufferSize);

    void ChangeTestModule(int currentTab);
    //Synthesizer
    void ChangeFrequency(int dblFrequency);
    void ChangeBitResolution(int nBitResolution);
    void ChangeWaveformType(int nType);

    //microphone
    void ChangeMicVolumn(int volumn);

private:
    Ui::AudioPreferenceDialog *ui;
    QString             m_strWindowTitle;
    AudioDeviceBase     *m_AudioDeviceBase;

    //test module
    Synthesizer         *m_Synth;
    AudioPlayer         *m_AudioPlayer;
    Waveform            *m_Waveform;
    Microphone          *m_Mic;
    AmplitudeMonitor    *m_AmplitudeMonitor;

    bool                m_bIsTesting;

    void RetriveInformation();
    void Connect();
};

#endif // AUDIOPREFERENCEDIALOG_H
