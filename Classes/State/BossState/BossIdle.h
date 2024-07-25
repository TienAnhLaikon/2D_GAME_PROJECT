#ifndef __BOSS_IDLE_H__
#define __BOSS_IDLE_H__

#include "cocos2d.h"
#include "BossStateBase.h"
USING_NS_CC;

class BossIdle : public BossStateBase
{
public:
    BossIdle(Boss* crt) : BossStateBase(crt) {}
    virtual void EnterState() override;
    virtual void UpdateState() override;
    virtual void ExitState() override;

private:
    int lastDir;
};

#endif 
