#ifndef __PLAYER_IDLE_H__
#define __PLAYER_IDLE_H__

#include "cocos2d.h"
#include "PlayerStateBase.h"
USING_NS_CC;

class PlayerIdle : public PlayerStateBase
{
public:
    PlayerIdle(Player* crt) : PlayerStateBase(crt) {}

    virtual void EnterState() override;
    virtual void UpdateState() override;
    virtual void ExitState() override;

private:
    int lastDir;
};

#endif 
