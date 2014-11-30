#include "amplitudemonitor.h"
#include <QPainter>

AmplitudeMonitor::AmplitudeMonitor(Microphone *pMicrophone)
{
    setBackgroundRole(QPalette::Base);

    m_pMic = pMicrophone;
    m_fVolumn = m_pMic->get_MicrophoneVolumn();
    m_pMic->RegisterAmplitudeMonitor(this);
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
                            m_fAmpLevel*width(),
                            painter.viewport().bottom()),Qt::cyan);
}

void AmplitudeMonitor::set_AmpLevel(float *fData, int nSize)
{
    float *fIn_data = (float*)fData;
    float fMaxVal = 0.0;
    for( int i = 0; i< nSize; i++){
        if(fIn_data[i] >= 0 && fMaxVal < fIn_data[i]){
            //find max amp
            fMaxVal = fIn_data[i];
        }
    }
    m_fAmpLevel = fMaxVal;
    update();
    //dB = 20 * log10(amplitude)
}





