#include "audiofileplayerview.h"
#include "ui_audiofileplayerview.h"
#include <QFileDialog>

AudioFilePlayerView::AudioFilePlayerView(AudioDeviceBase* pAudioDeviceBase) :
    ui(new Ui::AudioFilePlayerView),
    m_pAudioDeviceBase(pAudioDeviceBase)
{
    ui->setupUi(this);
    m_pAudioPlayer      = new AudioPlayer(m_pAudioDeviceBase);
    m_pWaveform         = new Waveform(m_pAudioPlayer);

    ui->AudioPlayerVerticalLayout->addWidget(m_pWaveform);
    m_pWaveform->show();

    ui->LoopPushButton->setCheckable(true);
    connect(ui->LoopPushButton, SIGNAL(toggled(bool)), this, SLOT(UpdateLooping(bool)));
    connect(ui->FileDialogPushButton, SIGNAL(clicked()), this, SLOT(OpenFileDialog()));
}

AudioFilePlayerView::~AudioFilePlayerView()
{
    delete m_pAudioPlayer;
    delete m_pWaveform;
    delete ui;
}

void AudioFilePlayerView::Eneble()
{
    m_pAudioPlayer->eneble();
}

void AudioFilePlayerView::Disable()
{
    m_pAudioPlayer->disable();
}

void AudioFilePlayerView::OpenFileDialog()
{
    QString filePath = QFileDialog::getOpenFileName(this,
         tr("Open File"), "/", tr("Audio Files (*.wav *.mp3)"));
    qDebug() << "Open file: " << filePath;
    m_pAudioPlayer->set_AudioFilePath(filePath);
    m_pWaveform->put_AudioData(m_pAudioPlayer->get_AudioData());
}

void AudioFilePlayerView::UpdateLooping(bool bLoop)
{
    qDebug() << "Set Looping" << QString(bLoop?"true":"false");
    m_pAudioPlayer->set_Looping(bLoop);
}
