#ifndef __BOSS_ROAR_H__
#define __BOSS_ROAR_H__

#include "cocos2d.h"
#include "BossStateBase.h"
USING_NS_CC;

class BossRoar : public BossStateBase
{
public:
    BossRoar(Boss* crt) : BossStateBase(crt) {}
    virtual void EnterState() override;
    virtual void UpdateState() override;
    virtual void ExitState() override;

private:
    int lastDir;
};

#endif 
