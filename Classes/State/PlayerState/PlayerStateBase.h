#ifndef __PLAYER_STATE_BASE_H__
#define __PLAYER_STATE_BASE_H__

#include "cocos2d.h"
//class StateManager;
class Player;
USING_NS_CC;

class PlayerStateBase
{
public:
    PlayerStateBase(Player* crt) : player(crt) {}

    virtual void EnterState() = 0;
    virtual void UpdateState() = 0;
    virtual void ExitState() = 0;

protected:
    Player* player;
};

#endif // __STATE_BASE_H__

