#ifndef AUDIOPREFERENCEDIALOG_H
#define AUDIOPREFERENCEDIALOG_H

#include "audiodevicebase.h"
#include "synthesizer.h"
#include <QDialog>
#include <QPointer>

namespace Ui {
class AudioPreferenceDialog;
}

class AudioPreferenceDialog : public QDialog
{
    Q_OBJECT

public:
    AudioPreferenceDialog(QWidget *parent = 0, AudioDeviceBase* audioBase = NULL, Synthesizer *synth = NULL);
    virtual ~AudioPreferenceDialog();

//    virtual void Update(Subject * theChangedSubject);
//        //Overide observer operation

public slots:
    void StartAudioTest(bool bStartTest);
    void ChangeInputDevice(int nSelectedItem);
    void ChangeOutputDevice(int nSelectedItem);
    void ChangeSamplingRate(int nSelectedItem);
    void ChangeBufferSize();
    void ChangeBufferSizeSlider(int nBufferSize);

    //Synthesizer
    void ChangeBitResolution(int nBitResolution);
    void ChangeFrequency(int dblFrequency);

private:
    Ui::AudioPreferenceDialog *ui;
    QString             m_strWindowTitle;
    AudioDeviceBase     *m_AudioDeviceBase;
    Synthesizer         *m_Synth;

    bool                m_bIsTesting;

    void RetriveInformation();
    void Connect();
};

#endif // AUDIOPREFERENCEDIALOG_H
