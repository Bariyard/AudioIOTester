#include "audioplayer.h"
#include "sndfile.h"

#include "waveform.h"

AudioPlayer::AudioPlayer(AudioDeviceBase* s)
{
    m_AudioDeviceBase = s;
    m_bIsModuleEnable = false;
    m_bEndOfFile = true;

    m_AudioPath = "";

    //QString filepath = "/Users/wrbally/Downloads/LOSS.mp3";//"/Users/wrbally/Downloads/Revolution (Acapella).wav""/Users/wrbally/Documents/Sound Bank/Bassline 4x4 UK Garage/Drum_Loops_140bpm/Wav/Drum_Loop_140bpm_004_PL.wav"

    ///Users/bariyard/Documents/Sound Bank/Bassline 4x4 UK Garage/Drum_Loops_140bpm/Wav "/Users/bariyard/Documents/Sound Bank/voice/acclivity__i-am-female 3.wav"
    //
    //readAudioFile((char*)"/Users/bariyard/Documents/Sound Bank/Bassline 4x4 UK Garage/Drum_Loops_140bpm/Wav/Drum_Loop_140bpm_007_PL.wav");
    m_AudioDeviceBase->registerTestModule(this);
}


AudioPlayer::~AudioPlayer()
{

}

void AudioPlayer::reset()
{

}


void AudioPlayer::process(const void */*inputBuffer*/, void *outputBuffer, const unsigned long framesPerBuffer)
{
    if(m_bEndOfFile){
        qDebug() << "EOF" ;
        return;
    }
    if(m_bIsModuleEnable)
    {
            float *out = (float*)outputBuffer;

            for (unsigned int i = 0; i < framesPerBuffer; i++) {
                if(!m_bEndOfFile){
                    *out++ += *m_dblAudioData++;
                    if(m_dblAudioData == m_dblEndFrame) m_bEndOfFile = true;
                    *out++ += *m_dblAudioData++;
                    if(m_dblAudioData == m_dblEndFrame) m_bEndOfFile = true;
                }else{
                    *out++ += 0.0;
                    *out++ += 0.0;
                }
            }
    }
}

void AudioPlayer::eneble()
{
    qDebug() << "Audio Player is Enable";
    m_bIsModuleEnable = true;
}

void AudioPlayer::disable()
{
    qDebug() << "Audio Player is Disable";
    m_bIsModuleEnable = false;
}

bool AudioPlayer::isEnabled()
{
    return m_bIsModuleEnable;
}

void AudioPlayer::readAudioFile(char* filename)
{
    SF_INFO sf_info;
    //my_sf_info.format = 0;
    qDebug() << "Read: " <<filename;
    SNDFILE* sndfile = sf_open(filename, SFM_READ, &sf_info);
    if (NULL == sndfile)
    {
        qDebug() << "can't read sound file " <<sf_strerror(sndfile);
    }

    m_dblAudioData = (float*)malloc(sf_info.frames * sf_info.channels * sizeof(float)); // TODO: is this correct?

    sf_count_t num_frames_read = sf_readf_float(sndfile, m_dblAudioData, sf_info.frames); // request all the frames
    qDebug() << "Num frame read: " << num_frames_read;
    m_nNumFrame = (sf_info.frames * sf_info.channels);
    qDebug() << "Calculate num frame: "<< m_nNumFrame;
    m_dblStartFrame = &m_dblAudioData[0];
    m_dblEndFrame = &m_dblAudioData[m_nNumFrame-1];
    m_nCurrentFrame = 0;
    m_bEndOfFile = false;
    sf_close(sndfile);

}


const float* AudioPlayer::get_AudioData()
{
    return m_dblStartFrame;
}

unsigned long AudioPlayer::get_NumberOfSample()
{
    return m_nNumFrame;
}


void AudioPlayer::set_AudioFilePath(QString path)
{
    m_AudioPath = path;

    //if(m_dblAudioData)
    //    delete m_dblAudioData;
    readAudioFile(m_AudioPath.toLocal8Bit().data());
}
