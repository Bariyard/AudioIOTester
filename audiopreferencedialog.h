#ifndef AUDIOPREFERENCEDIALOG_H
#define AUDIOPREFERENCEDIALOG_H

#include "observer.h"
#include "audiodevicebase.h"
#include <QDialog>
#include <QPointer>

namespace Ui {
class AudioPreferenceDialog;
}

class AudioPreferenceDialog :
        public QDialog,
        public Observer
{
    Q_OBJECT

public:
    AudioPreferenceDialog(QWidget *parent = 0, AudioDeviceBase* s = NULL);
    virtual ~AudioPreferenceDialog();

    virtual void Update(Subject * theChangedSubject);
        //Overide observer operation


private:
    Ui::AudioPreferenceDialog *ui;
    QString m_strWindowTitle;
    AudioDeviceBase *_subject;

    void RetriveInformation();
};

#endif // AUDIOPREFERENCEDIALOG_H
