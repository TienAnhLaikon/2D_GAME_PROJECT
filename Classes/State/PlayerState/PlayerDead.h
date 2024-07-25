#ifndef __PLAYER_DEAD_H__
#define __PLAYER_DEAD_H__

#include "cocos2d.h"
#include "PlayerStateBase.h"
USING_NS_CC;

class PlayerDead : public PlayerStateBase
{
public:
    PlayerDead(Player* crt) : PlayerStateBase(crt) {}

    virtual void EnterState() override;
    virtual void UpdateState() override;
    virtual void ExitState() override;
private:
    int lastDir;
};

#endif 
