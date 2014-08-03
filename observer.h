#ifndef OBSERVER_H
#define OBSERVER_H

#include <list>

class Subject;

class Observer
{
public:
    virtual ~Observer();
    virtual void Update(Subject* theChangeSubject) = 0;

protected:
    Observer();
};

class Subject
{

public:
    virtual ~Subject();

    virtual void Attach(Observer*);
    virtual void Detach(Observer*);
    virtual void Notify();

protected:
    Subject();
private:
    std::list<Observer*> *_observers;
};


#endif // OBSERVER_H
