#ifndef __BOSS_STATE_BASE_H__
#define __BOSS_STATE_BASE_H__

#include "cocos2d.h"
//class StateManager;
class Boss;
USING_NS_CC;

class BossStateBase
{
public:
    BossStateBase(Boss* crt) : boss(crt) {}

    virtual void EnterState() = 0;
    virtual void UpdateState() = 0;
    virtual void ExitState() = 0;

protected:
    Boss* boss;
};

#endif // __STATE_BASE_H__

