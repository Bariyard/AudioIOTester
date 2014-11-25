#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <QWidget>

class Waveform : public QWidget
{
    Q_OBJECT
public:
    Waveform(QWidget *parent = 0);
    ~Waveform();


    void put_AudioData(const float *audioData);
    void put_NumFrame(unsigned long numFrame);
    void analyzeAudioData();
signals:

protected:
    void paintEvent(QPaintEvent *event);
    void dropEvent(QDropEvent *event);

public slots:


private:



    const float *m_AudioData;
    unsigned long m_nFrame;
    QList<QPointF> m_LeftAudioDataPoint;
    QList<QPointF> m_RightAudioDataPoint;

    QList<QPointF> m_LeftWorldAudioDataPoint;
    QList<QPointF> m_RightWorldAudioDataPoint;



};

#endif // WAVEFORM_H
