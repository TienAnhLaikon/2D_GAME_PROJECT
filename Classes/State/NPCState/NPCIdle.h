#ifndef __NPC_IDLE_H__
#define __NPC_IDLE_H__

#include "cocos2d.h"
#include "NPCStateBase.h"
USING_NS_CC;

class NPCIdle : public NPCStateBase
{
public:
    NPCIdle(NPC *crt) : NPCStateBase(crt) {}
    virtual void EnterState() override;
    virtual void UpdateState() override;
    virtual void ExitState() override;

private:
    int lastDir;
};

#endif
