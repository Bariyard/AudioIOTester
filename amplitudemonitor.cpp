#include "amplitudemonitor.h"
#include <QPainter>

AmplitudeMonitor::AmplitudeMonitor(Microphone *microphone)
{
    setBackgroundRole(QPalette::Base);

    m_Mic = microphone;
    m_fVolumn = m_Mic->get_MicrophoneVolumn();
    m_Mic->registerAmplitudeMonitor(this);
}

AmplitudeMonitor::~AmplitudeMonitor()
{

}


void AmplitudeMonitor::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);
    painter.setPen(Qt::black);
    painter.fillRect(QRect(painter.viewport().left(),
                            painter.viewport().top(),
                            painter.viewport().right(),
                            painter.viewport().bottom()),Qt::black);

    //amp
    painter.fillRect(QRect(painter.viewport().left(),
                            painter.viewport().top(),
                            m_amplevel*width(),
                            painter.viewport().bottom()),Qt::cyan);
}

void AmplitudeMonitor::setAmpLevel(float *data, int size)
{
    float *in_data = (float*)data;
    float maxVal = 0.0;
    for( int i = 0; i< size; i++){
        if(in_data[i] >= 0 && maxVal < in_data[i]){ //find max amp
            maxVal = in_data[i];
        }
    }
    m_amplevel = maxVal;
    update();
    //dB = 20 * log10(amplitude)
}





