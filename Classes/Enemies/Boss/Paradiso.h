#ifndef __PARADISO_H__
#define __PARADISO_H__

#include "cocos2d.h"
#include "Boss.h"

USING_NS_CC;

class Paradiso : public Boss
{
public:
    virtual bool init();

    void loadAnimate();

    void setAttackSpeed(float attackSpeed);

    CREATE_FUNC(Paradiso);
};

#endif
