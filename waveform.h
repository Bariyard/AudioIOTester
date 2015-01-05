#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <QWidget>
#include "audioplayer.h"
#include "seeker.h"

class Waveform : public QWidget
{
    Q_OBJECT
public:
    Waveform(AudioPlayer * pAudioPlayer);
    ~Waveform();

    void        put_AudioData(const float *audioData);
    void        put_NumFrame(unsigned long numFrame);
    void        put_CurrentNumFrame(unsigned long numFrame);
    void        processAudioData();
    void        analyzeAudioData();

protected:
    void paintEvent(QPaintEvent *event);
    void dragEnterEvent(QDragEnterEvent* event);
    //this event is called when the mouse enters the widgets area during a drag/drop operation
    void dragMoveEvent(QDragMoveEvent* event);
    //this event is called when the mouse moves inside the widgets area during a drag/drop operation
    void dragLeaveEvent(QDragLeaveEvent* event);
    //this event is called when the mouse leaves the widgets area during a drag/drop operation
    void dropEvent(QDropEvent* event);
    //this event is called when the drop operation is initiated at the widget

signals:

public slots:
    void ChangNotify(unsigned long position);

private:
    AudioPlayer     *m_pAudioPlayer;
    const float     *m_pAudioData;
    unsigned long   m_nFrame;
    QList<QPointF>  m_LeftAudioDataPoint;
    QList<QPointF>  m_RightAudioDataPoint;

    Seeker          *m_pSeeker;
};

#endif // WAVEFORM_H
