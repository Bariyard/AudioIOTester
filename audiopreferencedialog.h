#ifndef AUDIOPREFERENCEDIALOG_H
#define AUDIOPREFERENCEDIALOG_H

#include "observer.h"
#include "audiodevicebase.h"
#include <QDialog>

namespace Ui {
class AudioPreferenceDialog;
}

class AudioPreferenceDialog :
        public QDialog,
        public Observer
{
    Q_OBJECT

public:
    explicit AudioPreferenceDialog(QWidget *parent = 0, AudioDeviceBase* s = NULL);
    virtual ~AudioPreferenceDialog();

    virtual void Update(Subject * theChangedSubject);
        //Overide observer operation


private:
    Ui::AudioPreferenceDialog *ui;
    AudioDeviceBase *_subject;
};

#endif // AUDIOPREFERENCEDIALOG_H
