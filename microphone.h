#ifndef MICROPHONE_H
#define MICROPHONE_H

#include "audiodevicebase.h"
#include "testmodule.h"
#include "amplitudemonitor.h"

class AmplitudeMonitor;

class Microphone : public TestModule
{
public:
    Microphone(AudioDeviceBase* s);
    virtual ~Microphone();

    virtual void        reset();
    virtual void        process(const void *inputBuffer, void *outputBuffer, const unsigned long framesPerBuffer);

    virtual void        eneble();
    virtual void        disable();
    virtual bool        isEnabled();

    void                put_MicrophoneVolumn(float fVolumn);
    float               get_MicrophoneVolumn();

    void                RegisterAmplitudeMonitor(AmplitudeMonitor * pAmplitudeMonitor);

private:
    AudioDeviceBase     *m_pAudioDeviceBase;
    bool                m_bIsModuleEnable;
    float               m_fMicVolumn;

    AmplitudeMonitor    *m_pAmpMonitor;

};

#endif // MICROPHONE_H
