#ifndef AUDIOPREFERENCEDIALOG_H
#define AUDIOPREFERENCEDIALOG_H

#include "observer.h"
#include "audiodevicebase.h"
#include <QDialog>
#include <QPointer>

namespace Ui {
class AudioPreferenceDialog;
}

class AudioPreferenceDialog :
        public QDialog,
        public Observer
{
    Q_OBJECT

public:
    AudioPreferenceDialog(QWidget *parent = 0, AudioDeviceBase* s = NULL);
    virtual ~AudioPreferenceDialog();

    virtual void Update(Subject * theChangedSubject);
        //Overide observer operation

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
    AudioDeviceBase     *_subject;

    bool                m_bIsTesting;

    void RetriveInformation();
    void Connect();
};

#endif // AUDIOPREFERENCEDIALOG_H
