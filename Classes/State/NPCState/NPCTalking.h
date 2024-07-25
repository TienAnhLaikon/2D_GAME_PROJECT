#ifndef __NPC_TALKING_H__
#define __NPC_TALKING_H__

#include "cocos2d.h"
#include "NPCStateBase.h"
USING_NS_CC;

class NPCTalking : public NPCStateBase
{
public:
    NPCTalking(NPC* crt) : NPCStateBase(crt) {}
    virtual void EnterState() override;
    virtual void UpdateState() override;
    virtual void ExitState() override;

private:
    int lastDir;
};

#endif 
