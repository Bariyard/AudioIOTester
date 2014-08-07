#include "audiopreferencedialog.h"
#include "ui_audiopreferencedialog.h"


AudioPreferenceDialog::AudioPreferenceDialog(QWidget *parent,AudioDeviceBase* s) :
    QDialog(parent),
    ui(new Ui::AudioPreferenceDialog),
    _subject(NULL)
{
    ui->setupUi(this);
    setFixedSize(this->size());
    m_strWindowTitle = "Audio I/O Tester";
    setWindowTitle(m_strWindowTitle);
    if(s){
        _subject = s;
        s->Attach(this);
    }
    RetriveInformation();
    _subject->StartStream();
}

AudioPreferenceDialog::~AudioPreferenceDialog()
{
    delete ui;
    if(_subject)
        _subject->Detach(this);
}

void AudioPreferenceDialog::Update(Subject* theChangedSubject)
{
    if(theChangedSubject == _subject){
        //do something
    }
}

void AudioPreferenceDialog::RetriveInformation()
{
    //Set Audio Device List
    QList<AudioDevice> *device;
    device = _subject->get_AudioDeviceList();
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
    samplingRate = _subject->get_AvailableSamplingRate();
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
    _subject->get_AvailableBufferSize(min_BufferSize, max_BufferSize);
    if(min_BufferSize != 0 && max_BufferSize != 0)
        ui->BufferSizeSlider->setRange(min_BufferSize, max_BufferSize);

        int default_bufferSize = 0 ;
        _subject->get_DefaultBufferSize(default_bufferSize);
        QString valueAsString = QString::number(default_bufferSize);
        ui->BufferSizeValueLabel->setText(valueAsString);

    //Bit Resolution
    int *bitResolution;
    bitResolution = _subject->get_BitResolution();
    while(*bitResolution != -1){
        QString valueAsString = QString::number(*bitResolution);
        valueAsString += " bit";
        //Put to UI
        ui->BitResolutionComboBox->addItem(valueAsString,*bitResolution);
        bitResolution++;
    }
}
