#ifndef AMPLITUDEMONITOR_H
#define AMPLITUDEMONITOR_H

#include "microphone.h"
#include <QWidget>

class Microphone;

class AmplitudeMonitor : public QWidget
{
    Q_OBJECT
public:
    AmplitudeMonitor(Microphone *pMicrophone);
    ~AmplitudeMonitor();

    void set_AmpLevel(float *pData, int nSize);
protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:

private:
    Microphone  *m_pMic;
    float       m_fVolumn;
    float       m_fAmpLevel;

};

#endif // AMPLITUDEMONITOR_H
