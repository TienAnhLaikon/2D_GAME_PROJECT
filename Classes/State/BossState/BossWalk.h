#ifndef __BOSS_WALK_H__
#define __BOSS_WALK_H__

#include "cocos2d.h"
#include "BossStateBase.h"
USING_NS_CC;

class BossWalk : public BossStateBase
{
public:
    BossWalk(Boss* crt) : BossStateBase(crt) {}
    virtual void EnterState() override;
    virtual void UpdateState() override;
    virtual void ExitState() override;

private:
    int lastDir;
};

#endif 
