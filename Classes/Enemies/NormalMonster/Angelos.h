#ifndef __ANGELOS_H__
#define __ANGELOS_H__

#include "cocos2d.h"
#include "NormalMonster.h"
USING_NS_CC;

class Angelos : public NormalMonster
{
public:
    virtual bool init();
    void loadAnimate();
    void setAttackSpeed(float attackSpeed);
    CREATE_FUNC(Angelos);

private:
};

#endif
