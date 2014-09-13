#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H
#include "audiodevicebase.h"

enum AudioBitDepth{
    AudioBitDepthunknown    = 0,
    AudioBitDepth8Bit       = 1,
    AudioBitDepth16Bit      = 2,
    AudioBitDepth32Bit      = 3,
    AudioBitDepth64Bit      = 4
};



class Synthesizer
{
public:
    Synthesizer(AudioDeviceBase* s);
    ~Synthesizer();

    void put_AudioFrequency(double dblFrequency);

private:
    AudioData           *m_SWaveData;
    AudioDeviceBase     *m_AudioDeviceBase;
    void SetData();
    void AdjustDataTable();
    double              m_dblAudioFrequency;
    double              m_dblDefaultAudioFrequency;

};

#endif // SYNTHESIZER_H
