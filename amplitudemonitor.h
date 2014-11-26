#ifndef AMPLITUDEMONITOR_H
#define AMPLITUDEMONITOR_H

#include "microphone.h"
#include <QWidget>

class Microphone;

class AmplitudeMonitor : public QWidget
{
    Q_OBJECT
public:
    AmplitudeMonitor(Microphone *microphone);
    ~AmplitudeMonitor();

    void setAmpLevel(float *data, int size);
protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:

private:
    Microphone  *m_Mic;
    float       m_fVolumn;
    float       m_amplevel;

};

#endif // AMPLITUDEMONITOR_H
