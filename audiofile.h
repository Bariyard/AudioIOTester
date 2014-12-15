#ifndef AUDIOFILE_H
#define AUDIOFILE_H
#include <QObject>

class AudioFile: public QObject
{
    Q_OBJECT
public:
    AudioFile();
    virtual ~AudioFile();
    void                Open(char *pFilename);
    void                Close();

    bool                IsEndOfFile();
    void                set_EndOfFile(bool isEndOfFile);

    QString             get_AudioFilePath();
    float*              get_StartFrame();
    float*              get_EndFrame();
    unsigned long       get_NumFrame();

private:
    float               *m_pAudioData;
    unsigned long       m_nNumFrame;
    float               *m_pStartFrame;
    float               *m_pEndFrame;
    bool                m_bEndOfFile;
    QString             m_strAudioPath;
};

#endif // AUDIOFILE_H
