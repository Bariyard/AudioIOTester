#ifndef GLOBALVOLUMN_H
#define GLOBALVOLUMN_H
#include "testmodule.h"
#include "audiodevicebase.h"

class GlobalVolumn :public TestModule
{
public:
    GlobalVolumn(AudioDeviceBase * pAudioDeviceBase);
    virtual ~GlobalVolumn();

    virtual void        reset();
    virtual void        process(const void *inputBuffer, void *outputBuffer, const unsigned long framesPerBuffer);

    virtual void        eneble();
    virtual void        disable();
    virtual bool        isEnabled();

    void                set_GlobalVolumn(double dblVolumn);
    double              get_GlobalVolumn();
private:
    AudioDeviceBase     *m_pAudioDeviceBase;
    bool                m_bIsModuleEnable;
    double              m_dblGlobalVolumn;
};

#endif // GLOBALVOLUMN_H
