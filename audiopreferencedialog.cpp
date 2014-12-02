#include "audiopreferencedialog.h"
#include "ui_audiopreferencedialog.h"


AudioPreferenceDialog::AudioPreferenceDialog(QWidget *parent, AudioDeviceBase* pAudioDeviceBase) :
    QDialog(parent),
    ui(new Ui::AudioPreferenceDialog),
    m_pAudioDeviceBase(pAudioDeviceBase)
{

    ui->setupUi(this);
    setFixedSize(this->size());
    m_strWindowTitle    = "Audio I/O Tester";
    setWindowTitle(m_strWindowTitle);

    m_bIsTesting        = false;
    m_pAudioPlayer      = new AudioPlayer(m_pAudioDeviceBase);
    m_pWaveform         = new Waveform(m_pAudioPlayer);
    m_pMic              = new Microphone(m_pAudioDeviceBase);
    m_pAmplitudeMonitor = new AmplitudeMonitor(m_pMic);
    m_pGlobalVolumn     = new GlobalVolumn(m_pAudioDeviceBase);
    m_pSynth             = new SynthesizerView(m_pAudioDeviceBase);


    RetriveInformation();
    Connect();

}

AudioPreferenceDialog::~AudioPreferenceDialog()
{
    delete ui;
    delete m_pAudioPlayer;
    delete m_pWaveform;
    delete m_pMic;
    delete m_pAmplitudeMonitor;
    delete m_pSynth;
}

void AudioPreferenceDialog::RetriveInformation()
{
    //Set Audio Device List
    QList<AudioDevice> *pDevice;
    pDevice = m_pAudioDeviceBase->get_AudioDeviceList();
    for (int i = 0; i < pDevice->size(); ++i)
    {
        if (pDevice->at(i).bIsInputOrOutput) //true means input
        {
            ui->AudioInputComboBox->addItem(pDevice->at(i).strName, pDevice->at(i).nIndex);
        }
        else
        {
            ui->AudioOutputComboBox->addItem(pDevice->at(i).strName, pDevice->at(i).nIndex);
        }
    }

    //Sampling Rate List
    double *pSamplingRateList;
    pSamplingRateList = m_pAudioDeviceBase->get_AvailableSamplingRate();
    qDebug() << &pSamplingRateList << "  " <<*pSamplingRateList;
    if(pSamplingRateList)
    {
        for(int i = 0; *pSamplingRateList != -1; i++)
        {
            QString valueAsString = QString::number(*pSamplingRateList);
            qDebug() << valueAsString << "," << *pSamplingRateList;

            //Put to UI
            ui->SamplingRateComboBox->addItem(valueAsString,*pSamplingRateList);
            if(*pSamplingRateList == m_pAudioDeviceBase->get_SamplingRate())
            {
                ui->SamplingRateComboBox->setCurrentIndex(i);
            }
            pSamplingRateList++;
        }
    }

    //Buffer Size List
    double *pBufferSizeList;
    pBufferSizeList = m_pAudioDeviceBase->get_AvailableBufferSize();
    qDebug() << &pBufferSizeList << " " << *pBufferSizeList;
    if(pBufferSizeList)
    {
        for(int i = 0; *pBufferSizeList != -1; i++)
        {
            QString valueAsString = QString::number(*pBufferSizeList)+" Samples";
            qDebug() << valueAsString << ", " << *pBufferSizeList;

            //Put to UI
            ui->BufferSizeComboBox->addItem(valueAsString,*pBufferSizeList);
            if(*pBufferSizeList == m_pAudioDeviceBase->get_BufferSize())
            {
                ui->BufferSizeComboBox->setCurrentIndex(i);
            }
            pBufferSizeList++;
        }
    }

    ui->SyntheSizerGridLayout->addWidget(m_pSynth);

    //audio player tab
    ui->AudioPlayerVerticalLayout->addWidget(m_pWaveform);
    ui->AudioPlayerTab->acceptDrops();
    m_pWaveform->show();

    //microphone tab
    ui->MicrophoneVolumnHorizontalSlider->setRange(0,100);
    ui->MicrophoneVolumnHorizontalSlider->setValue(m_pMic->get_MicrophoneVolumn()*100);
    ui->MicrophoneGridLayout->addWidget(m_pAmplitudeMonitor);
    m_pAmplitudeMonitor->show();

    //global volumn
    ui->TestVolumnHorizontalSlider->setValue(m_pGlobalVolumn->get_GlobalVolumn()*100);
}

void AudioPreferenceDialog::Connect()
{
    connect(ui->AudioInputComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(UpdateInputDevice(int)));
    connect(ui->AudioOutputComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(UpdateOutputDevice(int)));
    connect(ui->SamplingRateComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(UpdateSamplingRate(int)));
    connect(ui->BufferSizeComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(UpdateBufferSize(int)));

    connect(ui->TestingTabWidget, SIGNAL(currentChanged(int)), this, SLOT(UpdateTestModule(int)));

    //audio player
    connect(ui->TestPushButton,SIGNAL(clicked(bool)),this,SLOT(StartAudioTest(bool)));
    //microphone
    connect(ui->MicrophoneVolumnHorizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(UpdateMicVolumn(int)));
    //global volumn
    connect(ui->TestVolumnHorizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(UpdateGlobalVolumn(int)));
}

void AudioPreferenceDialog::StartAudioTest(bool bStartTest)
{
    qDebug() << "StartTest: " << bStartTest;
    if(!m_bIsTesting)
        m_pAudioDeviceBase->StartStream();
    else
        m_pAudioDeviceBase->StopStream();
    m_bIsTesting = !m_bIsTesting;
}

void AudioPreferenceDialog::UpdateInputDevice(int nSelectedItem)
{
    QVariant varDevice;
    int nDevice;
    varDevice = ui->AudioInputComboBox->itemData(nSelectedItem);
    nDevice = varDevice.toInt();
    qDebug() << "ChangeInputDevice: " << nDevice;
    m_pAudioDeviceBase->put_InputDevice(nDevice, m_bIsTesting);
}

void AudioPreferenceDialog::UpdateOutputDevice(int nSelectedItem)
{
    QVariant varDevice;
    int nDevice;
    varDevice = ui->AudioOutputComboBox->itemData(nSelectedItem);
    nDevice = varDevice.toInt();
    qDebug() << "ChangeOutputDevice: " << nDevice;
    m_pAudioDeviceBase->put_OutputDevice(nDevice, m_bIsTesting);
}

void AudioPreferenceDialog::UpdateSamplingRate(int nSelectedItem)
{
    int nSamplingRate = ui->SamplingRateComboBox->itemData(nSelectedItem).toInt();
    if(nSamplingRate != m_pAudioDeviceBase->get_SamplingRate())
    {
        qDebug() << "ChangeSamplingRate: "<< nSelectedItem;
        m_pAudioDeviceBase->put_SamplingRate(nSamplingRate, m_bIsTesting);
    }
}

void AudioPreferenceDialog::UpdateBufferSize(int nSelectedItem)
{
    int nBufferSize = ui->BufferSizeComboBox->itemData(nSelectedItem).toInt();
    if(nBufferSize != m_pAudioDeviceBase->get_BufferSize())
    {
        qDebug() << "ChangeBufferSize: "<< nBufferSize;
        m_pAudioDeviceBase->put_BufferSize(nBufferSize, m_bIsTesting);
    }
}
void AudioPreferenceDialog::UpdateTestModule(int nCurrentTab)
{
    qDebug() << "Set tab: " << nCurrentTab;
    if(m_bIsTesting)
    {
        m_pAudioDeviceBase->StopStream();
        m_bIsTesting = !m_bIsTesting;
    }

    switch (nCurrentTab)
    {
    case 0:{
        //m_pOscillator->eneble();
        m_pAudioPlayer->disable();
        m_pMic->disable();
        break;}
    case 1:{
        //m_pOscillator->disable();
        m_pAudioPlayer->eneble();
        m_pMic->disable();
        break;}
    case 2:{
        //m_pOscillator->disable();
        m_pAudioPlayer->disable();
        m_pMic->eneble();
        break;
    }
    default:
        break;
    }
}



//microphone
void AudioPreferenceDialog::UpdateMicVolumn(int nVolumn)
{
    qDebug() << "ChangeMicVolumn: " << nVolumn;
    //scale from 0, 100 to 0 to 1
    m_pMic->put_MicrophoneVolumn((float)nVolumn / 100.0);

}

//global volumn
void AudioPreferenceDialog::UpdateGlobalVolumn(int nVolumn)
{
    qDebug() << "ChangeMicVolumn: " << nVolumn;
    //scale from 0, 100 to 0 to 1
    m_pGlobalVolumn->set_GlobalVolumn((double)nVolumn / 100.0);
}

