#ifndef __PLAYER_SELECT_H__
#define __PLAYER_SELECT_H__

#include "cocos2d.h"
#include "PlayerStateBase.h"
USING_NS_CC;

class PlayerSelect : public PlayerStateBase
{
public:
    PlayerSelect(Player* crt) : PlayerStateBase(crt) {}
    virtual void EnterState() override;
    virtual void UpdateState() override;
    virtual void ExitState() override;

private:
    int lastDir;
};

#endif 
