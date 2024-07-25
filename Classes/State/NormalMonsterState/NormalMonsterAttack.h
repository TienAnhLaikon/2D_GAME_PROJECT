#ifndef __NORMAL_MONSTER_ATTACK_H__
#define __NORMAL_MONSTER_ATTACK_H__

#include "cocos2d.h"
#include "NormalMonsterStateBase.h"
USING_NS_CC;

class NormalMonsterAttack : public NormalMonsterStateBase
{
public:

    NormalMonsterAttack(NormalMonster* crt) : NormalMonsterStateBase(crt) {}
    virtual void EnterState() override;
    virtual void UpdateState() override;
    virtual void ExitState() override;

private:
    int lastDir;
};

#endif 
