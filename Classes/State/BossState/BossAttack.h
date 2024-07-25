#ifndef __BOSS_ATTACK_H__
#define __BOSS_ATTACK_H__

#include "cocos2d.h"
#include "BossStateBase.h"
USING_NS_CC;

class BossAttack : public BossStateBase
{
public:
    BossAttack(Boss* crt) : BossStateBase(crt) {}
    virtual void EnterState() override;
    virtual void UpdateState() override;
    virtual void ExitState() override;

private:
    int lastDir;
};

#endif 
