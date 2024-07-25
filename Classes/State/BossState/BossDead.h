#ifndef __BOSS_DEAD_H__
#define __BOSS_DEAD_H__

#include "cocos2d.h"
#include "BossStateBase.h"
USING_NS_CC;

class BossDead : public BossStateBase
{
public:
    BossDead(Boss* crt) : BossStateBase(crt) {}
    virtual void EnterState() override;
    virtual void UpdateState() override;
    virtual void ExitState() override;

private:
    int lastDir;
};

#endif 
