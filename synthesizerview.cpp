#include "synthesizerview.h"
#include "ui_synthesizerview.h"

SynthesizerView::SynthesizerView(AudioDeviceBase* pAudioDeviceBase) :
    ui(new Ui::SynthesizerView),
    m_pAudioDeviceBase(pAudioDeviceBase)
{
    ui->setupUi(this);

    m_pSynthesizer  = new Synthesizer(m_pAudioDeviceBase);
    m_pOscillator   = new Oscillator(m_pAudioDeviceBase);
    m_pOscillator2  = new Oscillator(m_pAudioDeviceBase);
    m_pFilter       = new Filter(m_pAudioDeviceBase);
    m_pLFO          = new LFO(m_pAudioDeviceBase);

    m_pAudioDeviceBase->RegisterTestModule(m_pSynthesizer);

    SetUpOsc1();
    SetUpOsc2();
    SetUpFilter();
    SetUpLFO();
    Routing();

    //enable oscillator first
    m_pOscillator->eneble();
    m_pOscillator2->eneble();
    ui->Osc2CheckBox->setChecked(m_pOscillator2->isEnabled());
    //m_pFilter->eneble();
    ui->FilterCheckBox->setChecked(m_pFilter->isEnabled());
    ui->LFOCheckBox->setChecked(m_pLFO->isEnabled());
    m_pSynthesizer->eneble();
    ui->tabWidget->removeTab(4);
//    ui->tab_4->setVisible(false);
}

SynthesizerView::~SynthesizerView()
{
    delete ui;
    delete m_pOscillator;
    delete m_pOscillator2;
    delete m_pFilter;
    delete m_pSynthesizer;
}

void SynthesizerView::Eneble()
{
    m_pSynthesizer->eneble();
}

void SynthesizerView::Disable()
{
    m_pSynthesizer->disable();
}
//Oscillator 1
void SynthesizerView::UpdateOsc1Frequency(double dblFreq)
{
        qDebug() << "ChangeFrequency: " << dblFreq;
        m_pOscillator->put_Frequency(dblFreq);
        QString strFreq = QString::number(dblFreq, 'f', 2);
        strFreq = strFreq.rightJustified(8,' ') +" Hz";
        ui->Osc1FrequencyValueLabel->setText(strFreq);

}

void SynthesizerView::UpdateOsc1WaveformType(int nType){
    if(nType+1 != m_pOscillator->get_WaveformType())
    {
        qDebug() << "Change Frequency Type" << nType;
        m_pOscillator->put_WaveformType(nType +1);
    }
}

void SynthesizerView::UpdateOsc1Gain(int nGain)
{
    if(nGain != m_pOscillator->get_Gain()*100)
    {
        qDebug() << "Change Gain Osc1" << nGain;
        m_pOscillator->put_Gain((double)nGain/100.0);
    }
}

//Oscillator 2
void SynthesizerView::UpdateOsc2Frequency(double dblFreq)
{
        qDebug() << "ChangeFrequency: " << dblFreq;
        m_pOscillator2->put_Frequency(dblFreq);
        QString strFreq = QString::number(dblFreq, 'f', 2);
        strFreq = strFreq.rightJustified(8,' ') +" Hz";
        ui->Osc2FrequencyValueLabel->setText(strFreq);
}

void SynthesizerView::UpdateOsc2WaveformType(int nType){
    if(nType + 1 != m_pOscillator2->get_WaveformType())
    {
        qDebug() << "Change Frequency Type" << nType;
        m_pOscillator2->put_WaveformType(nType + 1);
    }
}

void SynthesizerView::UpdateOsc2Gain(int nGain)
{
    if(nGain != m_pOscillator2->get_Gain()*100)
    {
        qDebug() << "Change Gain Osc1" << nGain;
        m_pOscillator2->put_Gain((double)nGain/100.0);
    }
}

void SynthesizerView::UpdateOsc2Eneble(bool bIsEneble)
{
    if(bIsEneble)
        m_pOscillator2->eneble();
    else
        m_pOscillator2->disable();
}

void SynthesizerView::UpdateFilterEnable(bool bIsEnable)
{
    if(bIsEnable)
        m_pFilter->eneble();
    else
        m_pFilter->disable();
}

void SynthesizerView::UpdateFilterType(int nType)
{
    m_pFilter->put_FilterType(FilterType(nType));
}

void SynthesizerView::UpdateFilterCutOffFrequency(int nFreq)
{
    m_pFilter->put_Frequency(nFreq);
    QString strFreq = QString::number(nFreq);
    strFreq = strFreq.rightJustified(8,' ') +" %";
    ui->FilterCutOffValueLabel->setText(strFreq);
}

void SynthesizerView::UpdateFilterResonance(int nRes)
{
    m_pFilter->put_Resonance(nRes);
    QString strRes = QString::number(nRes);
    strRes = strRes.rightJustified(8,' ') +" %";
    ui->FilterResonanceValueLabel->setText(strRes);
}


void SynthesizerView::UpdateLFOEnable(bool bIsEnable)
{
    if(bIsEnable)
        m_pLFO->eneble();
    else
        m_pLFO->disable();
}

void SynthesizerView::UpdateLFOType(int nType)
{
    if(nType + 1 != m_pLFO->get_LFOType() )
    {
        qDebug() << "Change Frequency Type" << nType;
        m_pLFO->put_LFOType(OscillatorType(nType+1));
    }
}

void SynthesizerView::UpdateLFOFrequency(int nFreq)
{
    m_pLFO->put_Frequency(nFreq);
    QString strFreq = QString::number(nFreq, 'f', 2);
    strFreq = strFreq.rightJustified(8,' ') +" Hz";
    ui->LFOFrequencyValueLabel->setText(strFreq);
}

void SynthesizerView::SetUpOsc1()
{
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
    UpdateOsc1Frequency(m_pOscillator->get_Frequency());
    ui->Osc1GainHorizontalSlider->setRange(0,100);
    ui->Osc1GainHorizontalSlider->setValue(m_pOscillator->get_Gain()*100);

    connect(ui->Osc1WaveformComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateOsc1WaveformType(int)));
    connect(m_pFrequencySlider, SIGNAL(doubleValueChanged(double)), this, SLOT(UpdateOsc1Frequency(double)));
    connect(ui->Osc1GainHorizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(UpdateOsc1Gain(int)));

}

void SynthesizerView::SetUpOsc2()
{
    //Oscillator 2
    //waveform type
    QString *strWaveformType = m_pOscillator2->get_WaveformTypeString();
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
    UpdateOsc2Frequency(m_pOscillator->get_Frequency());
    ui->Osc2GainHorizontalSlider->setRange(0,100);
    ui->Osc2GainHorizontalSlider->setValue(m_pOscillator->get_Gain()*100);

    connect(ui->Osc2WaveformComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateOsc2WaveformType(int)));
    connect(m_pFrequencySlider2, SIGNAL(doubleValueChanged(double)), this, SLOT(UpdateOsc2Frequency(double)));
    connect(ui->Osc2GainHorizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(UpdateOsc2Gain(int)));
    connect(ui->Osc2CheckBox, SIGNAL(toggled(bool)), this, SLOT(UpdateOsc2Eneble(bool)));
}

void SynthesizerView::SetUpFilter()
{
    //Filter
    FilterType eFilterType = m_pFilter->get_FilterType();
    QString *strFilterType = m_pFilter->get_FilterTypeString();
    for(int i = 0; i < 2 ; i ++)
    {
        ui->FilterTypeComboBox->addItem(strFilterType[i]);
    }
    ui->FilterTypeComboBox->setCurrentIndex(eFilterType);
    ui->FilterCutOffHorizontalSlider->setRange(100,5000);
    ui->FilterCutOffHorizontalSlider->setValue(m_pFilter->get_Frequency());
    ui->FilterResonanceHorizontalSlider->setRange(0,100);
    ui->FilterResonanceHorizontalSlider->setValue(m_pFilter->get_Resonance());
    //UpdateFilterCutOffFrequency(m_pFilter->get_Frequency());
    //UpdateFilterResonance(m_pFilter->get_Resonance());

    connect(ui->FilterCheckBox, SIGNAL(toggled(bool)), this, SLOT(UpdateFilterEnable(bool)));
    connect(ui->FilterTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateFilterType(int)));
    connect(ui->FilterCutOffHorizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(UpdateFilterCutOffFrequency(int)));
    connect(ui->FilterResonanceHorizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(UpdateFilterResonance(int)));
}

void SynthesizerView::SetUpLFO()
{
    //LFO
    int nLFOWaveformType;
    nLFOWaveformType = m_pLFO->get_LFOType();
    QString *strLFOWaveformType = m_pLFO->get_WaveformTypeString();
    for(int i = 0; i < 4 ; i ++)
    {
        ui->LFOWaveformComboBox->addItem(strLFOWaveformType[i]);
    }
    ui->LFOFrequencyHorizontalSlider->setRange(1,20);
    ui->LFOFrequencyHorizontalSlider->setValue(m_pLFO->get_Frequency());
    UpdateLFOFrequency(m_pLFO->get_Frequency());
    UpdateLFOType(m_pLFO->get_LFOType() - 1);

    connect(ui->LFOCheckBox, SIGNAL(toggled(bool)), this, SLOT(UpdateLFOEnable(bool)));
    connect(ui->LFOFrequencyHorizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(UpdateLFOFrequency(int)));
    connect(ui->LFOWaveformComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateLFOType(int)));
}

void SynthesizerView::Routing()
{
    m_pSynthesizer->RegisterSoundModule(m_pOscillator);
    m_pSynthesizer->RegisterSoundModule(m_pOscillator2);
    m_pSynthesizer->RegisterSoundModule(m_pFilter);
    m_pSynthesizer->RegisterSoundModule(m_pLFO);
}
