#ifndef __SUBJECT_H__
#define __SUBJECT_H__

#include "cocos2d.h"
USING_NS_CC;
class Observer;

class Subject
{
public:

    virtual ~Subject() {}
    virtual void registerObserver(Observer* obsever) = 0;
    virtual void removeObserver(Observer* obsever) = 0;
    virtual void notifyObservers() = 0;

};

#endif
