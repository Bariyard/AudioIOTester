#include "synthesizerview.h"
#include "ui_synthesizerview.h"

SynthesizerView::SynthesizerView(AudioDeviceBase* pAudioDeviceBase) :
    ui(new Ui::SynthesizerView),
    m_pAudioDeviceBase(pAudioDeviceBase)
{
    ui->setupUi(this);

    m_pSynthesizer  = new Synthesizer(m_pAudioDeviceBase);
    m_pOscillator   = new Oscillator(m_pAudioDeviceBase);

    m_pSynthesizer->RegisterSoundModule(m_pOscillator);
    m_pAudioDeviceBase->RegisterTestModule(m_pSynthesizer);

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
    UpdateFrequency(m_pOscillator->get_Frequency());
    ui->Osc1GainHorizontalSlider->setRange(0,100);
    ui->Osc1GainHorizontalSlider->setValue(m_pOscillator->get_Gain()*100);

    //enable oscillator first
    m_pOscillator->eneble();
    m_pSynthesizer->eneble();

    //oscillator
    connect(ui->waveformComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateWaveformType(int)));
    connect(m_pFrequencySlider, SIGNAL(doubleValueChanged(double)), this, SLOT(UpdateFrequency(double)));
    connect(ui->Osc1GainHorizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(UpdateGain(int)));

}

SynthesizerView::~SynthesizerView()
{
    delete ui;
    delete m_pOscillator;
    delete m_pOscillator2;
    delete m_pSynthesizer;
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

void SynthesizerView::UpdateGain(int nGain)
{
    if(nGain != m_pOscillator->get_Gain()*100)
    {
        qDebug() << "Change Gain Osc1" << nGain;
        m_pOscillator->put_Gain((double)nGain/100.0);
    }
}

void SynthesizerView::Eneble()
{
    m_pSynthesizer->eneble();
}

void SynthesizerView::Disable()
{
    m_pSynthesizer->disable();
}
