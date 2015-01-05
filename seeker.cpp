#include "seeker.h"
#include <QPainter>

Seeker::Seeker(QWidget *parent) : QWidget(parent)
{
    m_nCurrentFrame     = 0;
    m_dblIncrementer    = 0;

}

Seeker::~Seeker()
{

}

void Seeker::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);

    painter.setPen(Qt::cyan);
    //float incrementer = (float)width()/(float)m_LeftAudioDataPoint.size();

    //draw current frame line
    painter.setPen(Qt::red);
    painter.drawLine(m_nCurrentFrame * m_dblIncrementer,
                     0,
                     m_nCurrentFrame * m_dblIncrementer,
                     height());

}


void Seeker::put_CurrentNumFrame(unsigned long numFrame)
{
    m_nCurrentFrame = numFrame;
}

void Seeker::put_Incrementer(double dblIncrementer)
{
    m_dblIncrementer = dblIncrementer;
}
