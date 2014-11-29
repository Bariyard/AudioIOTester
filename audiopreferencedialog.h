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

//class DoubleSlider;
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
    void ChangeInputDevice(int nSelectedItem);
    void ChangeOutputDevice(int nSelectedItem);
    void ChangeSamplingRate(int nSelectedItem);
    void ChangeBufferSize(int nSelectedItem);

    //Test module
    void ChangeTestModule(int currentTab);
    //Synthesizer
    void ChangeFrequency(double dblFreq);
    void ChangeWaveformType(int nType);

    //Microphone
    void ChangeMicVolumn(int volumn);

    //global volumn
    void ChangeGlobalVolumn(int volumn);

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
    GlobalVolumn        *m_GlobalVolumn;

    bool                m_bIsTesting;

    DoubleSlider        *m_FrequencySlider;

    void RetriveInformation();
    void Connect();
};

#endif // AUDIOPREFERENCEDIALOG_H
