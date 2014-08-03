#include "observer.h"

//Observer
Observer::Observer(){

}

Observer::~Observer(){

}

//Subject
Subject::Subject(){
    _observers = new std::list<Observer*>;
}

Subject::~Subject(){
    delete _observers;
}

void Subject::Attach(Observer * o)
{
    _observers->push_back(o);
}

void Subject::Detach(Observer * o){
    _observers->remove(o);
}

void Subject::Notify(){
    for(std::list<Observer*>::iterator it = _observers->begin(); it != _observers->end(); it++)
    {
        (*it)->Update(this);
    }
}
