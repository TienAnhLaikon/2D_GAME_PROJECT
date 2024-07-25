#ifndef __NPC_STATE_BASE_H__
#define __NPC_STATE_BASE_H__

#include "cocos2d.h"
// class StateManager;
class NPC;
USING_NS_CC;

class NPCStateBase
{
public:
    NPCStateBase(NPC *crt) : npc(crt) {}

    virtual void EnterState() = 0;
    virtual void UpdateState() = 0;
    virtual void ExitState() = 0;

protected:
    NPC *npc;
};

#endif // __STATE_BASE_H__
