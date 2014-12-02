#include "synthesizerview.h"
#include "ui_synthesizerview.h"

SynthesizerView::SynthesizerView(AudioDeviceBase* pAudioDeviceBase) :
    ui(new Ui::SynthesizerView),
    m_pAudioDeviceBase(pAudioDeviceBase)
{
    ui->setupUi(this);
    m_pOscillator            = new Oscillator(m_pAudioDeviceBase);

    //waveform type
    int nWaveformType;
    nWaveformType = m_pOscillator->get_WaveformType();
    QString *strWaveformType = m_pOscillator->get_WaveformTypeString();
    for(int i = 0; i < 4 ; i ++)
    {
        ui->waveformComboBox->addItem(strWaveformType[i]);
    }

    //Frequency double slider
    m_pFrequencySlider = new DoubleSlider();
    m_pFrequencySlider->setOrientation(Qt::Horizontal);
    m_pFrequencySlider->setRange(0,100);
    m_pFrequencySlider->setScale(16.35,7902.13);
    ui->SynthesizerGridLayout->addWidget(m_pFrequencySlider,0,2);
    ui->FrequencyValueLabel->setScaledContents(false);
//    ui->FrequencyValueLabel->setTextFormat(Qt::TextIncludeTrailingSpaces);
//    ui->FrequencyValueLabel->setTextFormat(Qt::TextIncludeTrailingSpaces);
//    ui->FrequencyValueLabel->setTextInteractionFlags(Qt::TextIncludeTrailingSpaces);
//    ui->FrequencyValueLabel->setTextInteractionFlags(QFlags<Qt::TextIncludeTrailingSpaces>);

    //ui->FrequencyValueLabel->setTextFormat(Qt::PlainText);
    UpdateFrequency(m_pOscillator->get_Frequency());
    //enable oscillator first
    m_pOscillator->eneble();

    //oscillator
    connect(ui->waveformComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateWaveformType(int)));
    connect(m_pFrequencySlider, SIGNAL(doubleValueChanged(double)), this, SLOT(UpdateFrequency(double)));

}

SynthesizerView::~SynthesizerView()
{
    delete ui;
    delete m_pOscillator;
}

//Synthesizer
void SynthesizerView::UpdateFrequency(double dblFreq)
{
        qDebug() << "ChangeFrequency: " << dblFreq;
        m_pOscillator->put_Frequency(dblFreq);
        QString strFreq = QString::number(dblFreq, 'f', 2);
        strFreq = strFreq.rightJustified(8,' ') +" Hz";
        ui->FrequencyValueLabel->setText(strFreq);

}

void SynthesizerView::UpdateWaveformType(int nType){
    if(nType != m_pOscillator->get_WaveformType() -1)
    {
        qDebug() << "Change Frequency Type" << nType;
        m_pOscillator->put_WaveformType(nType);
    }
}
