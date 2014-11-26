#ifndef MICROPHONE_H
#define MICROPHONE_H

#include "amplitudemonitor.h"
#include "testmodule.h"
#include "audiodevicebase.h"

class AmplitudeMonitor;


class Microphone : public TestModule
{
public:
    Microphone(AudioDeviceBase* s);
    virtual ~Microphone();

    virtual void        process(const void *inputBuffer, void *outputBuffer, const unsigned long framesPerBuffer);

    virtual void        eneble();
    virtual void        disable();
    virtual bool        isEnabled();

    void  put_MicrophoneVolumn(float fVolumn);
    float get_MicrophoneVolumn();

    void registerAmplitudeMonitor(AmplitudeMonitor * amp);

private:
    AudioDeviceBase     *m_AudioDeviceBase;
    bool                m_bIsModuleEnable;
    float               m_fMicVolumn;

    AmplitudeMonitor    *m_AmpMonitor;

};

#endif // MICROPHONE_H
