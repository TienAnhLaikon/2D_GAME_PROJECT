#ifndef __SHADOW_SENTINELS_H__
#define __SHADOW_SENTINELS_H__

#include "cocos2d.h"
#include "NormalMonster.h"
USING_NS_CC;

class ShadowSentinels : public NormalMonster
{
public:
    virtual bool init();
    void loadAnimate();
    void setAttackSpeed(float attackSpeed);
    CREATE_FUNC(ShadowSentinels);

private:
};

#endif
