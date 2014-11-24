#include "audiopreferencedialog.h"
#include "ui_audiopreferencedialog.h"


AudioPreferenceDialog::AudioPreferenceDialog(QWidget *parent,AudioDeviceBase* audioBase, Synthesizer *synth) :
    QDialog(parent),
    ui(new Ui::AudioPreferenceDialog),
    m_AudioDeviceBase(NULL),
    m_bIsTesting(false)
{
    ui->setupUi(this);
    setFixedSize(this->size());
    m_strWindowTitle = "Audio I/O Tester";
    setWindowTitle(m_strWindowTitle);
    //if(s){
        m_AudioDeviceBase = audioBase;
        m_Synth         = synth;
        //s->Attach(this);
    //}
    RetriveInformation();
    Connect();

}

AudioPreferenceDialog::~AudioPreferenceDialog()
{
    delete ui;
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
        while(*samplingRate != -1){
            QString valueAsString = QString::number(*samplingRate);
            qDebug() << valueAsString << ",,,,," << *samplingRate;

            //Put to UI
            ui->SamplingRateComboBox->addItem(valueAsString,*samplingRate);
            samplingRate++;
        }
    }

    //Buffer Size
    int min_BufferSize = 0;
    int max_BufferSize = 0;
    m_AudioDeviceBase->get_AvailableBufferSize(min_BufferSize, max_BufferSize);
    if(min_BufferSize != 0 && max_BufferSize != 0)
        ui->BufferSizeSlider->setRange(min_BufferSize, max_BufferSize);

    int default_bufferSize = 0 ;
    m_AudioDeviceBase->get_DefaultBufferSize(default_bufferSize);
    QString valueAsString = QString::number(default_bufferSize);
    ui->BufferSizeValueLabel->setText(valueAsString);
    ui->BufferSizeApplyPushButton->setVisible(false);

    //Bit Resolution
    int *bitResolution;
    bitResolution = m_AudioDeviceBase->get_BitResolution();
    while(*bitResolution != -1){
        QString valueAsString = QString::number(*bitResolution);
        valueAsString += " bit";
        //Put to UI
        ui->BitResolutionComboBox->addItem(valueAsString,*bitResolution);
        bitResolution++;
    }

    ui->FrequencySlider->setRange((int)16.35,(int)7902.13);
}


void AudioPreferenceDialog::Connect()
{
    connect(ui->AudioInputComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(ChangeInputDevice(int)));
    connect(ui->AudioOutputComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(ChangeOutputDevice(int)));
    connect(ui->SamplingRateComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(ChangeSamplingRate(int)));
    connect(ui->BufferSizeSlider,SIGNAL(valueChanged(int)),this,SLOT(ChangeBufferSizeSlider(int)));
    connect(ui->BufferSizeApplyPushButton,SIGNAL(clicked(bool)),this,SLOT(ChangeBufferSize()));

    connect(ui->BitResolutionComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(ChangeBitResolution(int)));
    connect(ui->FrequencySlider,SIGNAL(valueChanged(int)),this,SLOT(ChangeFrequency(int)));

    connect(ui->TestPushButton,SIGNAL(clicked(bool)),this,SLOT(StartAudioTest(bool)));
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
    m_AudioDeviceBase->put_SamplingRate(nSelectedItem,m_bIsTesting);
}

void AudioPreferenceDialog::ChangeBufferSizeSlider(int nBufferSize)
{
    if(!ui->BufferSizeApplyPushButton->isVisible())
        ui->BufferSizeApplyPushButton->setVisible(true);

    ui->BufferSizeValueLabel->setText(QString::number(nBufferSize));
}

void AudioPreferenceDialog::ChangeBufferSize()
{
    int nBufferSize = ui->BufferSizeSlider->value();
    qDebug() << "ChangeBufferSize: "<< nBufferSize;
    ui->BufferSizeApplyPushButton->setVisible(false);
    m_AudioDeviceBase->put_BufferSize(nBufferSize,m_bIsTesting);
}

//Synthesizer
void AudioPreferenceDialog::ChangeBitResolution(int nBitRes)
{
    qDebug() << "ChangeBitResolution: " << nBitRes;
    m_AudioDeviceBase->put_BitResoulution(nBitRes);

}

void AudioPreferenceDialog::ChangeFrequency(int nFreq)
{
    qDebug() << "ChangeFrequency: " << nFreq;
    //m_AudioDeviceBase->put_AudioFrequency(nFreq);
    m_Synth->put_AudioFrequency(nFreq);
    ui->FrequencyValueLabel->setText(QString::number(nFreq));
}

