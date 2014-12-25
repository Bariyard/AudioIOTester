#ifndef VOLUMN_H
#define VOLUMN_H
#include "testmodule.h"
#include "audiodevicebase.h"

class Volumn :public TestModule
{
public:
    Volumn(AudioDeviceBase * pAudioDeviceBase);
    virtual ~Volumn();

    virtual void        reset();
    virtual void        process(const void *inputBuffer, void *outputBuffer, const unsigned long framesPerBuffer);

    virtual void        eneble();
    virtual void        disable();
    virtual bool        isEnabled();

    void                put_Volumn(double dblVolumn);
    double              get_Volumn();
private:
    AudioDeviceBase     *m_pAudioDeviceBase;
    bool                m_bIsModuleEnable;
    double              m_dblVolumn;
};

#endif // VOLUMN_H
