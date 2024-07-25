#ifndef __OKYANUS_H__
#define __OKYANUS_H__

#include "cocos2d.h"
#include "Boss.h"

USING_NS_CC;

class Okyanus : public Boss
{
public:
    
    virtual bool init();
    void loadAnimate();
    void setAttackSpeed(float attackSpeed);

    CREATE_FUNC(Okyanus);
};

#endif 
