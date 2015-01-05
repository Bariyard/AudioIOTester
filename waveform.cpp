#include "waveform.h"

#include <QList>
#include <QPainter>
#include <QMimeData>
#include <QDropEvent>
#include <QDebug>
#include <QDir>

Waveform::Waveform(AudioPlayer * pAudioPlayer)
{
    m_pSeeker       = new Seeker(this);
    m_pSeeker->setFixedSize(this->size());
    m_pAudioPlayer  = pAudioPlayer;
    setBackgroundRole(QPalette::Base);
    setAcceptDrops(true);
    connect(m_pAudioPlayer, SIGNAL(BufferChange(ulong)), this, SLOT(ChangNotify(ulong)));
}

Waveform::~Waveform()
{
    delete m_pAudioData;
}

void Waveform::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);
    painter.setPen(Qt::black);
    painter.fillRect(QRect(painter.viewport().left(),
                            painter.viewport().top(),
                            painter.viewport().right(),
                            painter.viewport().bottom()),Qt::black);

    auto scaleToMiddle =[] (float prevY, float h)
    {
        auto middleY = h/2.0;
        return (prevY*middleY)+middleY;
    };

    if(!m_LeftAudioDataPoint.isEmpty() && !m_RightAudioDataPoint.isEmpty())
    {
        painter.setPen(Qt::cyan);
        float incrementer = (float)width()/(float)m_LeftAudioDataPoint.size();
        float worldX = 0.0;
        for(QList<QPointF>::iterator stlIter = m_LeftAudioDataPoint.begin(); stlIter != m_LeftAudioDataPoint.end()-1; stlIter++)
        {
            painter.drawLine(worldX,
                             scaleToMiddle(QPointF(*stlIter).y(), height()),
                             worldX+incrementer,
                             scaleToMiddle(QPointF(*(stlIter+1)).y(), height()));
            worldX += incrementer;
        }
    }
}

void Waveform::dragEnterEvent(QDragEnterEvent* event)
{
    // if some actions should not be usable, like move, this code must be adopted
    event->acceptProposedAction();
}

void Waveform::dragMoveEvent(QDragMoveEvent* event)
{
    // if some actions should not be usable, like move, this code must be adopted
    event->acceptProposedAction();
}

void Waveform::dragLeaveEvent(QDragLeaveEvent* event)
{
    event->accept();
}

void Waveform::dropEvent(QDropEvent *event)
{
    qDebug() <<"Drop file to waveform widget" ;
    const QMimeData* mimeData = event->mimeData();
    // check for our needed mime type, here a file or a list of files
    if (mimeData->hasUrls())
    {

        //bug drag drop event from xcode6   /.file/id=6571367.2217061
//        QString path = QString(mimeData->text());
//        path = path.section('/',2);
//        qDebug() << "Path: " << path;

//        if (mimeData->hasUrls()) {
//            QList<QUrl> urlList = mimeData->urls();
//            QString text;
//            for (int i = 0; i < urlList.size() && i < 32; ++i) {
//                QString url = urlList.at(i).path();
//            }
//            qDebug() << "text: " << text;
//        }



//        m_pAudioPlayer->set_AudioFilePath(path);
//        m_pAudioData    = m_pAudioPlayer->get_AudioData();
//        m_nFrame        = m_pAudioPlayer->get_NumberOfSample();
//        if(!m_RightAudioDataPoint.isEmpty())
//            m_RightAudioDataPoint.clear();
//        if(!m_LeftAudioDataPoint.isEmpty())
//            m_LeftAudioDataPoint.clear();
//        analyzeAudioData();
//        update();
    }
}

void Waveform::put_AudioData(const float *audioData)
{
    m_pAudioData = audioData;
    if(m_pAudioData)
        processAudioData();
}

void Waveform::put_NumFrame(unsigned long numFrame)
{
    m_nFrame = numFrame;
}

void Waveform::processAudioData()
{
    m_pAudioData    = m_pAudioPlayer->get_AudioData();
    m_nFrame        = m_pAudioPlayer->get_NumberOfSample();
    if(!m_RightAudioDataPoint.isEmpty())
        m_RightAudioDataPoint.clear();
    if(!m_LeftAudioDataPoint.isEmpty())
        m_LeftAudioDataPoint.clear();
    if(m_pAudioData)
        analyzeAudioData();
    update();
}

void Waveform::analyzeAudioData()
{
    if(m_pAudioData)
    {
        qDebug() << "analyze audio data";
        unsigned long frameCounter = 0;
        float incrementer = (float)width()/(m_nFrame/2.0);
        float x1, y1, x2, y2;
        for(double i = 0.0; frameCounter< m_nFrame-1; i+=incrementer){
            //left
            x1 = 0;
            y1 = m_pAudioData[frameCounter];
            m_LeftAudioDataPoint.push_back(QPointF(x1,y1));
            x2 = 0;
            y2 = m_pAudioData[frameCounter+1] ;
            m_RightAudioDataPoint.push_back(QPointF(x2,y2));
            frameCounter+=2;
        }

        //seeker
        m_pSeeker->put_Incrementer((float)width()/m_pAudioPlayer->get_NumberOfSample());
    }
}

void Waveform::ChangNotify(unsigned long position)
{
    m_pSeeker->put_CurrentNumFrame(position);
    m_pSeeker->update();
}
