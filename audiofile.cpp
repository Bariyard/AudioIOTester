#include "audiofile.h"
#include "sndfile.h"
#include <QDebug>

AudioFile::AudioFile()
{
    m_bEndOfFile        = true;
    m_strAudioPath      = QString("");
}

AudioFile::~AudioFile()
{
    delete m_pAudioData;
}

void AudioFile::Open(char* pFilename)
{
    SF_INFO sf_info;
    qDebug() << "Read: " << pFilename;
    SNDFILE* sndfile = sf_open(pFilename, SFM_READ, &sf_info);
    if (NULL == sndfile)
    {
        qDebug() << "can't read sound file " << sf_strerror(sndfile);
        sf_close(sndfile);
        return;
    }

    m_pAudioData = (float*)malloc(sf_info.frames * sf_info.channels * sizeof(float));
    //request all the frames
    sf_count_t num_frames_read = sf_readf_float(sndfile, m_pAudioData, sf_info.frames);
    qDebug() << "Num frame read: " << num_frames_read;
    //calculate number of frame and channel
    m_nNumFrame = (sf_info.frames * sf_info.channels);
    qDebug() << "Calculate num frame: "<< m_nNumFrame;
    m_pStartFrame   = &m_pAudioData[0];
    m_pEndFrame     = &m_pAudioData[m_nNumFrame-1];
    m_bEndOfFile    = false;

    sf_close(sndfile);
}

void AudioFile::Close()
{

}

bool AudioFile::IsEndOfFile()
{
    return m_bEndOfFile;
}

void AudioFile::set_EndOfFile(bool isEndOfFile)
{
    m_bEndOfFile = isEndOfFile;
}

QString AudioFile::get_AudioFilePath()
{
    return m_strAudioPath;
}

float* AudioFile::get_StartFrame()
{
    return m_pStartFrame;
}

float* AudioFile::get_EndFrame()
{
    return m_pEndFrame;
}

unsigned long AudioFile::get_NumFrame()
{
    return m_nNumFrame;
}
