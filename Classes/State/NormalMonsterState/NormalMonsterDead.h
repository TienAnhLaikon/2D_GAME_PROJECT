#ifndef __NormalMonster_DEAD_H__
#define __NormalMonster_DEAD_H__

#include "cocos2d.h"
#include "NormalMonsterStateBase.h"
USING_NS_CC;

class NormalMonsterDead : public NormalMonsterStateBase
{
public:
    NormalMonsterDead(NormalMonster* crt) : NormalMonsterStateBase(crt) {}
    virtual void EnterState() override;
    virtual void UpdateState() override;
    virtual void ExitState() override;

private:
    int lastDir;
};

#endif 
