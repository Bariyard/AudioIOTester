#include "audioplayer.h"
#include "sndfile.h"

AudioPlayer::AudioPlayer(AudioDeviceBase* s)
{
    m_AudioDeviceBase = s;
    m_bIsModuleEnable = true;

    QString filepath = "/Users/wrbally/Downloads/LOSS.mp3";//"/Users/wrbally/Downloads/Revolution (Acapella).wav""/Users/wrbally/Documents/Sound Bank/Bassline 4x4 UK Garage/Drum_Loops_140bpm/Wav/Drum_Loop_140bpm_004_PL.wav"
    readAudioFile((char*)"/Users/wrbally/Music/iTunes/iTunes Media/Music/Bariyard/Unknown Album/The Revolution.aif");
    m_AudioDeviceBase->registerTestModule(this);
}


AudioPlayer::~AudioPlayer()
{

}

void AudioPlayer::process(const void *inputBuffer, void *outputBuffer, const unsigned long framesPerBuffer)
{
    if(m_bEndOfFile)
        qDebug() << "EOF" ;

    if(m_bIsModuleEnable )
    {
            float *out = (float*)outputBuffer;
            float *in  = (float*)inputBuffer;

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

bool AudioPlayer::isEnabled()
{

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
    m_nNumFrame = (sf_info.frames * sf_info.channels);
    m_dblStartFrame = &m_dblAudioData[0];
    m_dblEndFrame = &m_dblAudioData[m_nNumFrame-1];
    m_nCurrentFrame = 0;
    m_bEndOfFile = false;
    sf_close(sndfile);

}


