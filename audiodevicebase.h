#ifndef AUDIODEVICEBASE_H
#define AUDIODEVICEBASE_H

#include "portaudio.h"

class AudioDeviceBase
{
public:
    virtual ~AudioDeviceBase() = 0;
    virtual void Initialize() = 0;
    virtual void Start() = 0;
    virtual void Stop() = 0;
    virtual void Destroy() = 0;



};

#endif // AUDIODEVICEBASE_H
