#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H
#include "audiodevicebase.h"
#include "testmodule.h"
#include <QList>

class Synthesizer : public TestModule
{
public:
    Synthesizer(AudioDeviceBase *pAudioDeviceBase);
    virtual ~Synthesizer();

    virtual void        reset();
    virtual void        process(const void *inputBuffer, void *outputBuffer, const unsigned long framesPerBuffer);

    virtual void        eneble();
    virtual void        disable();
    virtual bool        isEnabled();

    void                RegisterSoundModule(SoundModule * pSoundModule);

private:
    AudioDeviceBase     *m_pAudioDeviceBase;
    bool                m_bIsModuleEnable;

    QList<SoundModule*> m_pSoundModule;
};

#endif // SYNTHESIZER_H
