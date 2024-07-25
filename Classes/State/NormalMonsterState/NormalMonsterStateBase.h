#ifndef __NORMAL_MONSTER_STATE_BASE_H__
#define __NORMAL_MONSTER_STATE_BASE_H__

#include "cocos2d.h"
//class StateManager;
class NormalMonster;
USING_NS_CC;

class NormalMonsterStateBase
{
public:
    NormalMonsterStateBase(NormalMonster* crt) : normalMonster(crt) {}

    virtual void EnterState() = 0;
    virtual void UpdateState() = 0;
    virtual void ExitState() = 0;

protected:
    NormalMonster* normalMonster;
};

#endif // __STATE_BASE_H__

