#ifndef TESTMODULE_H
#define TESTMODULE_H

#include <QObject>

class TestModule : public QObject
{
    Q_OBJECT
public:
    TestModule();
    virtual ~TestModule();

    //process data one buffer at a time
    virtual void process(const void *inputBuffer, void *outputBuffer,
                         const unsigned long framesPerBuffer) = 0;
    virtual void eneble() = 0;
    virtual void disable() = 0;
    virtual bool isEnabled() = 0;

signals:

public slots:

};

#endif // TESTMODULE_H
