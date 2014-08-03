#include "audiopreferencedialog.h"
#include "ui_audiopreferencedialog.h"


AudioPreferenceDialog::AudioPreferenceDialog(QWidget *parent,AudioDeviceBase* s) :
    QDialog(parent),
    ui(new Ui::AudioPreferenceDialog),
    _subject(NULL)
{
    ui->setupUi(this);
    setFixedSize(this->size());
    if(s){
        _subject = s;
        s->Attach(this);
    }
}

AudioPreferenceDialog::~AudioPreferenceDialog()
{
    delete ui;
    if(_subject)
        _subject->Detach(this);
}

void AudioPreferenceDialog::Update(Subject* theChangedSubject){
    if(theChangedSubject == _subject){
        //do something
    }
}
