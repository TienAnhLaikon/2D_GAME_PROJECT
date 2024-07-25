#ifndef __OBSEVER_H__
#define __OBSEVER_H__

#include "cocos2d.h"
USING_NS_CC;

class Observer
{
public:
    virtual ~Observer() {}
    virtual void update(const std::string& message) = 0;

};

#endif
