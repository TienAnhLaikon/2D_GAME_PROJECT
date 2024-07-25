#ifndef __BOSS_ANGRY_H__
#define __BOSS_ANGRY_H__

#include "cocos2d.h"
#include "BossStateBase.h"
USING_NS_CC;

class BossAngry : public BossStateBase
{
public:
    BossAngry(Boss* crt) : BossStateBase(crt) {}
    virtual void EnterState() override;
    virtual void UpdateState() override;
    virtual void ExitState() override;

private:
    int lastDir;
};

#endif 
