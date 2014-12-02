#ifndef MICROPHONEVIEW_H
#define MICROPHONEVIEW_H

#include "audiodevicebase.h"
#include "microphone.h"
#include "amplitudemonitor.h"
#include <QWidget>

namespace Ui {
class MicrophoneView;
}

class MicrophoneView : public QWidget
{
    Q_OBJECT

public:
    MicrophoneView(AudioDeviceBase *pAudioDeviceBase);
    ~MicrophoneView();
    //Microphone
    void UpdateMicVolumn(int nVolumn);
    void Eneble();
    void Disable();
private:
    Ui::MicrophoneView      *ui;
    AudioDeviceBase         *m_pAudioDeviceBase;
    Microphone              *m_pMic;
    AmplitudeMonitor        *m_pAmplitudeMonitor;
};

#endif // MICROPHONEVIEW_H
