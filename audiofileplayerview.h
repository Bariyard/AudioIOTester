#ifndef AUDIOFILEPLAYERVIEW_H
#define AUDIOFILEPLAYERVIEW_H

#include "audiodevicebase.h"
#include "audioplayer.h"
#include "waveform.h"
#include <QWidget>

namespace Ui {
class AudioFilePlayerView;
}

class AudioFilePlayerView : public QWidget
{
    Q_OBJECT

public:
    AudioFilePlayerView(AudioDeviceBase *pAudioDeviceBase);
    ~AudioFilePlayerView();
    void Eneble();
    void Disable();

public slots:
    void OpenFileDialog();
    void UpdateLooping(bool bLoop);
//    void Play();
//    void Stop();
//    void Loop();

private:
    Ui::AudioFilePlayerView     *ui;
    AudioDeviceBase             *m_pAudioDeviceBase;
    AudioPlayer                 *m_pAudioPlayer;
    Waveform                    *m_pWaveform;

};

#endif // AUDIOFILEPLAYERVIEW_H
