#ifndef __HELL_BEAST_H__
#define __HELL_BEAST_H__

#include "cocos2d.h"
#include "Boss.h"

USING_NS_CC;

class HellBeast : public Boss
{
public:
    virtual bool init();

    void loadAnimate();

    void setAttackSpeed(float attackSpeed);

    CREATE_FUNC(HellBeast);
};

#endif 
