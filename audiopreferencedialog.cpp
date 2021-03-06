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

    m_pSynth            = new SynthesizerView(m_pAudioDeviceBase);
    m_pFilePlayer       = new AudioFilePlayerView(m_pAudioDeviceBase);
    m_pMic              = new MicrophoneView(m_pAudioDeviceBase);

    m_pVolumn     = new Volumn(m_pAudioDeviceBase);

    RetriveInformation();
    Connect();

}

AudioPreferenceDialog::~AudioPreferenceDialog()
{
    delete m_pSynth;
    delete m_pFilePlayer;
    delete m_pMic;
    delete ui;
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

    UpdateTestModule(ui->TestingTabWidget->currentIndex());
    //Synthesizer tab
    ui->SynthesizerGridLayout->addWidget(m_pSynth);
    //Audio player tab
    ui->AudioPlayerGridLayout->addWidget(m_pFilePlayer);
    ui->AudioPlayerTab->acceptDrops();
    //Microphone tab
    ui->MicrophoneGridLayout->addWidget(m_pMic);

    //global volumn
    ui->TestVolumnHorizontalSlider->setValue(m_pVolumn->get_Volumn()*100);
    m_pVolumn->eneble();
}

void AudioPreferenceDialog::Connect()
{
    connect(ui->AudioInputComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(UpdateInputDevice(int)));
    connect(ui->AudioOutputComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(UpdateOutputDevice(int)));
    connect(ui->SamplingRateComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(UpdateSamplingRate(int)));
    connect(ui->BufferSizeComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(UpdateBufferSize(int)));
    //global volumn
    connect(ui->TestVolumnHorizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(UpdateGlobalVolumn(int)));

    connect(ui->TestingTabWidget, SIGNAL(currentChanged(int)), this, SLOT(UpdateTestModule(int)));
    //audio player
    connect(ui->TestPushButton,SIGNAL(clicked(bool)),this,SLOT(StartAudioTest(bool)));
}

void AudioPreferenceDialog::StartAudioTest(bool bStartTest)
{
    qDebug() << "StartTest: " << (bStartTest ? "true" : "false");
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

//global volumn
void AudioPreferenceDialog::UpdateGlobalVolumn(int nVolumn)
{
    qDebug() << "ChangeMicVolumn: " << nVolumn;
    //scale from 0, 100 to 0 to 1
    m_pVolumn->put_Volumn((double)nVolumn / 100.0);
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
        m_pSynth->Eneble();
        m_pFilePlayer->Disable();
        m_pMic->Disable();
        break;}
    case 1:{
        m_pSynth->Disable();
        m_pFilePlayer->Eneble();
        m_pMic->Disable();
        break;}
    case 2:{
        m_pSynth->Disable();
        m_pFilePlayer->Disable();
        m_pMic->Eneble();
        break;
    }
    default:
        break;
    }
}


