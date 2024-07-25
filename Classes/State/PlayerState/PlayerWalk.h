#ifndef __PLAYER_WALK_H__
#define __PLAYER_WALK_H__

#include "cocos2d.h"
#include "PlayerStateBase.h"
USING_NS_CC;

class PlayerWalk : public PlayerStateBase
{
public:
    PlayerWalk(Player* crt) : PlayerStateBase(crt) {}
    int footstep_soundId;
    virtual void EnterState() override;
    virtual void UpdateState() override;
    virtual void ExitState() override;

private:
    int lastDir;
};

#endif 
