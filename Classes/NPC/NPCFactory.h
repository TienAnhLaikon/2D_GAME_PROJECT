#ifndef _NPC_FACTORY_H_
#define _NPC_FACTORY_H_

#include "cocos2d.h"

#include "NPC/NPC.h"
#include "Robert.h"
#include "Durin.h"
#include "ThienAn.h"
USING_NS_CC;

class NPCFactory
{
public:
    static NPC *createNPC(std::string NPCType);
};

#endif
