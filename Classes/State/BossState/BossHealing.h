#ifndef __BOSS_HEALING_H__
#define __BOSS_HEALING_H__

#include "cocos2d.h"
#include "BossStateBase.h"
USING_NS_CC;

class BossHealing : public BossStateBase
{
public:
    BossHealing(Boss *crt) : BossStateBase(crt) {}
    virtual void EnterState() override;
    virtual void UpdateState() override;
    virtual void ExitState() override;

private:
    int lastDir;
};

#endif
