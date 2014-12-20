#include "synthesizerview.h"
#include "ui_synthesizerview.h"

SynthesizerView::SynthesizerView(AudioDeviceBase* pAudioDeviceBase) :
    ui(new Ui::SynthesizerView),
    m_pAudioDeviceBase(pAudioDeviceBase)
{
    ui->setupUi(this);

    m_pSynthesizer  = new Synthesizer(m_pAudioDeviceBase);
    m_pOscillator   = new Oscillator(m_pAudioDeviceBase);
    m_pOscillator2   = new Oscillator(m_pAudioDeviceBase);

    m_pSynthesizer->RegisterSoundModule(m_pOscillator);
    m_pSynthesizer->RegisterSoundModule(m_pOscillator2);
    m_pAudioDeviceBase->RegisterTestModule(m_pSynthesizer);



    //Oscillator 1
    //waveform type
    int nWaveformType;
    nWaveformType = m_pOscillator->get_WaveformType();
    QString *strWaveformType = m_pOscillator->get_WaveformTypeString();
    for(int i = 0; i < 4 ; i ++)
    {
        ui->Osc1WaveformComboBox->addItem(strWaveformType[i]);
    }
    //Frequency double slider
    m_pFrequencySlider = new DoubleSlider();
    m_pFrequencySlider->setOrientation(Qt::Horizontal);
    m_pFrequencySlider->setRange(0,100);
    m_pFrequencySlider->setScale(16.35,7902.13);
    ui->Osc1SynthesizerGridLayout->addWidget(m_pFrequencySlider,0,2);
    ui->Osc1FrequencyValueLabel->setScaledContents(false);
    UpdateFrequencyOsc1(m_pOscillator->get_Frequency());
    ui->Osc1GainHorizontalSlider->setRange(0,100);
    ui->Osc1GainHorizontalSlider->setValue(m_pOscillator->get_Gain()*100);

    connect(ui->Osc1WaveformComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateWaveformTypeOsc1(int)));
    connect(m_pFrequencySlider, SIGNAL(doubleValueChanged(double)), this, SLOT(UpdateFrequencyOsc1(double)));
    connect(ui->Osc1GainHorizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(UpdateGainOsc1(int)));

    //Oscillator 2
    //waveform type
    nWaveformType = m_pOscillator2->get_WaveformType();
    strWaveformType = m_pOscillator2->get_WaveformTypeString();
    for(int i = 0; i < 4 ; i ++)
    {
        ui->Osc2WaveformComboBox->addItem(strWaveformType[i]);
    }
    m_pFrequencySlider2 = new DoubleSlider();
    m_pFrequencySlider2->setOrientation(Qt::Horizontal);
    m_pFrequencySlider2->setRange(0,100);
    m_pFrequencySlider2->setScale(16.35,7902.13);
    ui->Osc2SynthesizerGridLayout->addWidget(m_pFrequencySlider2,2,2);
    ui->Osc2FrequencyValueLabel->setScaledContents(false);
    UpdateFrequencyOsc2(m_pOscillator->get_Frequency());
    ui->Osc2GainHorizontalSlider->setRange(0,100);
    ui->Osc2GainHorizontalSlider->setValue(m_pOscillator->get_Gain()*100);

    connect(ui->Osc2WaveformComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateWaveformTypeOsc2(int)));
    connect(m_pFrequencySlider2, SIGNAL(doubleValueChanged(double)), this, SLOT(UpdateFrequencyOsc2(double)));
    connect(ui->Osc2GainHorizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(UpdateGainOsc2(int)));
    connect(ui->Osc2CheckBox, SIGNAL(toggled(bool)), this, SLOT(UpdateEnebleOsc2(bool)));


    //enable oscillator first
    m_pOscillator->eneble();
    m_pOscillator2->eneble();
    ui->Osc2CheckBox->setChecked(m_pOscillator2->isEnabled());
    m_pSynthesizer->eneble();

}

SynthesizerView::~SynthesizerView()
{
    delete ui;
    delete m_pOscillator;
    delete m_pOscillator2;
    delete m_pSynthesizer;
}

//Oscillator 1
void SynthesizerView::UpdateFrequencyOsc1(double dblFreq)
{
        qDebug() << "ChangeFrequency: " << dblFreq;
        m_pOscillator->put_Frequency(dblFreq);
        QString strFreq = QString::number(dblFreq, 'f', 2);
        strFreq = strFreq.rightJustified(8,' ') +" Hz";
        ui->Osc1FrequencyValueLabel->setText(strFreq);

}

void SynthesizerView::UpdateWaveformTypeOsc1(int nType){
    if(nType != m_pOscillator->get_WaveformType() -1)
    {
        qDebug() << "Change Frequency Type" << nType;
        m_pOscillator->put_WaveformType(nType);
    }
}

void SynthesizerView::UpdateGainOsc1(int nGain)
{
    if(nGain != m_pOscillator->get_Gain()*100)
    {
        qDebug() << "Change Gain Osc1" << nGain;
        m_pOscillator->put_Gain((double)nGain/100.0);
    }
}

//Oscillator 2
void SynthesizerView::UpdateFrequencyOsc2(double dblFreq)
{
        qDebug() << "ChangeFrequency: " << dblFreq;
        m_pOscillator2->put_Frequency(dblFreq);
        QString strFreq = QString::number(dblFreq, 'f', 2);
        strFreq = strFreq.rightJustified(8,' ') +" Hz";
        ui->Osc2FrequencyValueLabel->setText(strFreq);
}

void SynthesizerView::UpdateWaveformTypeOsc2(int nType){
    if(nType != m_pOscillator2->get_WaveformType() -1)
    {
        qDebug() << "Change Frequency Type" << nType;
        m_pOscillator2->put_WaveformType(nType);
    }
}

void SynthesizerView::UpdateGainOsc2(int nGain)
{
    if(nGain != m_pOscillator2->get_Gain()*100)
    {
        qDebug() << "Change Gain Osc1" << nGain;
        m_pOscillator2->put_Gain((double)nGain/100.0);
    }
}

void SynthesizerView::UpdateEnebleOsc2(bool bIsEneble)
{
    if(bIsEneble)
        m_pOscillator2->eneble();
    else
        m_pOscillator2->disable();
}

void SynthesizerView::Eneble()
{
    m_pSynthesizer->eneble();
}

void SynthesizerView::Disable()
{
    m_pSynthesizer->disable();
}
