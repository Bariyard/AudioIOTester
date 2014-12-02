#include "microphoneview.h"
#include "ui_microphoneview.h"

MicrophoneView::MicrophoneView(AudioDeviceBase *pAudioDeviceBase) :
    ui(new Ui::MicrophoneView),
    m_pAudioDeviceBase(pAudioDeviceBase)
{
    ui->setupUi(this);

    m_pMic              = new Microphone(m_pAudioDeviceBase);
    m_pAmplitudeMonitor = new AmplitudeMonitor(m_pMic);

    //microphone tab
    ui->MicrophoneVolumnHorizontalSlider->setRange(0,100);
    ui->MicrophoneVolumnHorizontalSlider->setValue(m_pMic->get_MicrophoneVolumn()*100);
    ui->MicrophoneGridLayout->addWidget(m_pAmplitudeMonitor);
    m_pAmplitudeMonitor->show();

    //microphone
    connect(ui->MicrophoneVolumnHorizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(UpdateMicVolumn(int)));
}

MicrophoneView::~MicrophoneView()
{
    delete m_pMic;
    delete m_pAmplitudeMonitor;
    delete ui;
}

//microphone
void MicrophoneView::UpdateMicVolumn(int nVolumn)
{
    qDebug() << "ChangeMicVolumn: " << nVolumn;
    //scale from 0, 100 to 0 to 1
    m_pMic->put_MicrophoneVolumn((float)nVolumn / 100.0);
}

void MicrophoneView::Eneble()
{
    m_pMic->eneble();
}

void MicrophoneView::Disable()
{
    m_pMic->disable();
}
