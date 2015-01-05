#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H
#include "testmodule.h"
#include "audiodevicebase.h"
#include "audiofile.h"

class AudioPlayer : public TestModule
{

public:
    AudioPlayer(AudioDeviceBase* pAudioDeviceBase);
    virtual ~AudioPlayer();

    virtual void        reset();
    virtual void        process(const void *inputBuffer, void *outputBuffer, const unsigned long framesPerBuffer);

    virtual void        eneble();
    virtual void        disable();
    virtual bool        isEnabled();

    const float*        get_AudioData();
    unsigned long       get_NumberOfSample();
    unsigned long       get_CurrentNumFrame();

    void                set_AudioFilePath(QString strFilePath);
    void                set_Looping(bool bIsLoop);

private:
    AudioDeviceBase     *m_pAudioDeviceBase;
    AudioFile           *m_pAudioFile;
    float               *m_pCurrentFrame;
    unsigned long       m_nCurrentNumFrame;
    bool                m_bIsModuleEnable;
    bool                m_bIsLooping;

    void                CheckCurrentFrame();
};

#endif // AUDIOPLAYER_H
