#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H
#include "testmodule.h"
#include "audiodevicebase.h"

typedef struct {
        float* data_frame;
        unsigned long num_frame;
        float* start_frame;
        float* end_frame;
}Audio_file;


class AudioPlayer : public TestModule
{
public:
    AudioPlayer(AudioDeviceBase* s);
    virtual ~AudioPlayer();

    virtual void        process(const void *inputBuffer, void *outputBuffer, const unsigned long framesPerBuffer);

    virtual bool        isEnabled();

    void readAudioFile(char *filename);

private:
    AudioDeviceBase     *m_AudioDeviceBase;
    bool                m_bIsModuleEnable;

    float               *m_dblAudioData;
    unsigned long       m_nNumFrame;
    float               *m_dblStartFrame;
    float               *m_dblEndFrame;
    unsigned long        m_nCurrentFrame;
    bool                m_bEndOfFile;


};

#endif // AUDIOPLAYER_H