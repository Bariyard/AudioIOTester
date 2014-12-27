#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <QWidget>
#include "audioplayer.h"

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

signals:

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

public slots:

private:
    AudioPlayer     *m_pAudioPlayer;
    const float     *m_pAudioData;
    unsigned long   m_nFrame;
    unsigned long   m_nCurrentFrame;
    QList<QPointF>  m_LeftAudioDataPoint;
    QList<QPointF>  m_RightAudioDataPoint;
};

#endif // WAVEFORM_H
