#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H
#include "testmodule.h"
#include "audiodevicebase.h"

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
    void                set_AudioFilePath(QString strFilePath);

private:
    AudioDeviceBase     *m_pAudioDeviceBase;
    bool                m_bIsModuleEnable;

    float               *m_pAudioData;
    unsigned long       m_nNumFrame;
    float               *m_pStartFrame;
    float               *m_pEndFrame;
    unsigned long       m_nCurrentFrame;
    bool                m_bEndOfFile;

    void                readAudioFile(char *pFilename);
    QString             m_strAudioPath;


};

#endif // AUDIOPLAYER_H
