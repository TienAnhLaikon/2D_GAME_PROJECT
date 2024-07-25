#ifndef __BOSS_RUN_H__
#define __BOSS_RUN_H__

#include "cocos2d.h"
#include "BossStateBase.h"
USING_NS_CC;

class BossRun : public BossStateBase
{
public:
    BossRun(Boss* crt) : BossStateBase(crt) {}
    virtual void EnterState() override;
    virtual void UpdateState() override;
    virtual void ExitState() override;

private:
    int lastDir;
};

#endif 
