#include "audiopreferencedialog.h"
#include "ui_audiopreferencedialog.h"


AudioPreferenceDialog::AudioPreferenceDialog(QWidget *parent,AudioDeviceBase* audioBase) :
    QDialog(parent),
    ui(new Ui::AudioPreferenceDialog),
    m_AudioDeviceBase(audioBase),
    m_bIsTesting(false)
{
    ui->setupUi(this);
    setFixedSize(this->size());
    m_strWindowTitle = "Audio I/O Tester";
    setWindowTitle(m_strWindowTitle);

    m_Synth          = new Synthesizer(m_AudioDeviceBase);
    m_AudioPlayer    = new AudioPlayer(m_AudioDeviceBase);
    m_Waveform       = new Waveform(m_AudioPlayer);
    m_Mic            = new Microphone(m_AudioDeviceBase);
    m_AmplitudeMonitor = new AmplitudeMonitor(m_Mic);

    RetriveInformation();
    Connect();

}

AudioPreferenceDialog::~AudioPreferenceDialog()
{
    delete ui;
    delete m_Synth;
    delete m_AudioPlayer;
    delete m_Waveform;
    delete m_Mic;
    delete m_AmplitudeMonitor;
}

void AudioPreferenceDialog::RetriveInformation()
{
    //Set Audio Device List
    QList<AudioDevice> *device;
    device = m_AudioDeviceBase->get_AudioDeviceList();
    for (int i = 0; i < device->size(); ++i)
    {
        if (device->at(i).isInputOrOutput) //true means input
        {
            ui->AudioInputComboBox->addItem(device->at(i).name, device->at(i).index);
        }
        else
        {
            ui->AudioOutputComboBox->addItem(device->at(i).name, device->at(i).index);
        }
    }

    //Sampling Rate
    double *samplingRate;
    samplingRate = m_AudioDeviceBase->get_AvailableSamplingRate();
    qDebug() << &samplingRate <<"  "<<*samplingRate;
    if(samplingRate)
    {
        for(int i = 0; *samplingRate != -1; i++){
            QString valueAsString = QString::number(*samplingRate);
            qDebug() << valueAsString << ",,,,," << *samplingRate;

            //Put to UI
            ui->SamplingRateComboBox->addItem(valueAsString,*samplingRate);
            if(*samplingRate == m_AudioDeviceBase->get_SamplingRate())
            {
                ui->SamplingRateComboBox->setCurrentIndex(i);
            }
            samplingRate++;
        }


    }

    //Buffer Size
    double *bufferSizeList;
    bufferSizeList = m_AudioDeviceBase->get_AvailableBufferSize();
    qDebug() << &bufferSizeList <<"  "<<*bufferSizeList;
    if(bufferSizeList)
    {
        for(int i = 0; *bufferSizeList != -1; i++){
            QString valueAsString = QString::number(*bufferSizeList)+" Samples";
            qDebug() << valueAsString << ",,,,," << *bufferSizeList;

            //Put to UI
            ui->BufferSizeComboBox->addItem(valueAsString,*bufferSizeList);
            if(*bufferSizeList == m_AudioDeviceBase->get_BufferSize())
            {
                ui->BufferSizeComboBox->setCurrentIndex(i);
            }
            bufferSizeList++;
        }
    }

    //waveform type
    int nWaveformType;
    nWaveformType = m_Synth->get_WaveformType();
    QString strWaveformType[] = {"sin", "saw", "triangle", "square"};
    for(int i = 0; i < 4 ; i ++){
        ui->waveformComboBox->addItem(strWaveformType[i]);
    }
    ui->FrequencySlider->setRange((int)16.35,(int)7902.13);

    //enable oscillator first
    m_Synth->eneble();

    //audio player tab
    ui->AudioPlayerVerticalLayout->addWidget(m_Waveform);
    ui->AudioPlayerTab->acceptDrops();
    m_Waveform->show();

    //microphone tab
    ui->MicrophoneVolumnHorizontalSlider->setRange(0,100);

    ui->MicrophoneGridLayout->addWidget(m_AmplitudeMonitor);
    m_AmplitudeMonitor->show();
}


void AudioPreferenceDialog::Connect()
{
    connect(ui->AudioInputComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(ChangeInputDevice(int)));
    connect(ui->AudioOutputComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(ChangeOutputDevice(int)));
    connect(ui->SamplingRateComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(ChangeSamplingRate(int)));
    connect(ui->BufferSizeComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(ChangeBufferSize(int)));

    //connect(ui->BitResolutionComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(ChangeBitResolution(int)));

    //oscillator
    connect(ui->TestingTabWidget, SIGNAL(currentChanged(int)), this, SLOT(ChangeTestModule(int)));
    connect(ui->waveformComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeWaveformType(int)));
    connect(ui->FrequencySlider,SIGNAL(valueChanged(int)),this,SLOT(ChangeFrequency(int)));

    //audio player
    connect(ui->TestPushButton,SIGNAL(clicked(bool)),this,SLOT(StartAudioTest(bool)));

    //microphone
    connect(ui->MicrophoneVolumnHorizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(ChangeMicVolumn(int)));
}

void AudioPreferenceDialog::StartAudioTest(bool bStartTest)
{
    qDebug() << "StartTest: " << bStartTest;
    if(!m_bIsTesting)
        m_AudioDeviceBase->StartStream();
    else
        m_AudioDeviceBase->StopStream();
    m_bIsTesting = !m_bIsTesting;
}
void AudioPreferenceDialog::ChangeInputDevice(int nSelectedItem)
{
    QVariant varDevice;
    int nDevice;

    varDevice = ui->AudioInputComboBox->itemData(nSelectedItem);
    nDevice = varDevice.toInt();
    qDebug() << "ChangeInputDevice: " << nDevice;
    m_AudioDeviceBase->put_InputDevice(nDevice,m_bIsTesting);
}

void AudioPreferenceDialog::ChangeOutputDevice(int nSelectedItem)
{
    QVariant varDevice;
    int nDevice;

    varDevice = ui->AudioOutputComboBox->itemData(nSelectedItem);
    nDevice = varDevice.toInt();
    qDebug() << "ChangeOutputDevice: " << nDevice;
    m_AudioDeviceBase->put_OutputDevice(nDevice,m_bIsTesting);
}

void AudioPreferenceDialog::ChangeSamplingRate(int nSelectedItem)
{
    qDebug() << "ChangeSamplingRate: "<< nSelectedItem;
    m_AudioDeviceBase->put_SamplingRate(ui->SamplingRateComboBox->itemData(nSelectedItem).toInt()
                                        ,m_bIsTesting);
}

void AudioPreferenceDialog::ChangeBufferSize(int nSelectedItem)
{
//    int nBufferSize = ui->BufferSizeSlider->value();
    m_AudioDeviceBase->put_BufferSize(ui->BufferSizeComboBox->itemData(nSelectedItem).toInt(),
                                      m_bIsTesting);
}


void AudioPreferenceDialog::ChangeTestModule(int currentTab)
{
    qDebug() << "Set tab: " << currentTab;
    if(m_bIsTesting){
         m_AudioDeviceBase->StopStream();
         m_bIsTesting = !m_bIsTesting;
    }
    switch (currentTab) {
    case 0:{
        m_Synth->eneble();
        m_AudioPlayer->disable();
        m_Mic->disable();
        break;}
    case 1:{
        m_Synth->disable();
        m_AudioPlayer->eneble();
        m_Mic->disable();
        break;}
    case 2:{
        m_Synth->disable();
        m_AudioPlayer->disable();
        m_Mic->eneble();
        break;
    }
    default:
        break;
    }

}

//Synthesizer
void AudioPreferenceDialog::ChangeFrequency(int nFreq)
{
    qDebug() << "ChangeFrequency: " << nFreq;
    //m_AudioDeviceBase->put_AudioFrequency(nFreq);
    m_Synth->put_AudioFrequency(nFreq);
    ui->FrequencyValueLabel->setText(QString::number(nFreq));
}

void AudioPreferenceDialog::ChangeWaveformType(int nType){
    qDebug() << "Change Frequency Type" << nType;
    m_Synth->put_WaveformType(nType + 1);   //hard coded for array index value
}

//microphone
void AudioPreferenceDialog::ChangeMicVolumn(int volumn)
{
    qDebug() << "ChangeMicVolumn: " << volumn;
    //scale from 0, 100 to 0 to 1
    m_Mic->put_MicrophoneVolumn((float)volumn/100.0);

}
