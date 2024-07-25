#ifndef __NORMAL_MONSTER_IDLE_H__
#define __NORMAL_MONSTER_IDLE_H__

#include "cocos2d.h"
#include "NormalMonsterStateBase.h"
USING_NS_CC;

class NormalMonsterIdle : public NormalMonsterStateBase
{
public:
    NormalMonsterIdle(NormalMonster* crt) : NormalMonsterStateBase(crt) {}
    virtual void EnterState() override;
    virtual void UpdateState() override;
    virtual void ExitState() override;

private:
    int lastDir;
};

#endif 
