#ifndef __BOSS_GET_HIT_H__
#define __BOSS_GET_HIT_H__

#include "cocos2d.h"
#include "BossStateBase.h"
USING_NS_CC;

class BossGetHit : public BossStateBase
{
public:
    BossGetHit(Boss* crt) : BossStateBase(crt) {}
    virtual void EnterState() override;
    virtual void UpdateState() override;
    virtual void ExitState() override;

private:
    int lastDir;
};

#endif 
