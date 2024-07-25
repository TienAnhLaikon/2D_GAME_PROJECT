#include "NPCTalking.h"
#include "NPC/NPC.h"

void NPCTalking::EnterState()
{
	//CCLOG("NPC Enter STalking State");

	if (npc->getCurrentDirection() == 1)
	{
		npc->getCurrentSprite()->runAction(RepeatForever::create(npc->NPC_Select_E));
	}

	if (npc->getCurrentDirection() == 2)
	{
		npc->getCurrentSprite()->runAction(RepeatForever::create(npc->NPC_Select_NE));
	}

	if (npc->getCurrentDirection() == 3)
	{
		npc->getCurrentSprite()->runAction(RepeatForever::create(npc->NPC_Select_N));
	}

	if (npc->getCurrentDirection() == 4)
	{
		npc->getCurrentSprite()->runAction(RepeatForever::create(npc->NPC_Select_NW));
	}

	if (npc->getCurrentDirection() == 5)
	{
		npc->getCurrentSprite()->runAction(RepeatForever::create(npc->NPC_Select_W));
	}

	if (npc->getCurrentDirection() == 6)
	{
		npc->getCurrentSprite()->runAction(RepeatForever::create(npc->NPC_Select_SW));
	}

	if (npc->getCurrentDirection() == 7)
	{
		npc->getCurrentSprite()->runAction(RepeatForever::create(npc->NPC_Select_S));
	}

	if (npc->getCurrentDirection() == 8)
	{
		npc->getCurrentSprite()->runAction(RepeatForever::create(npc->NPC_Select_SE));
	}
	lastDir = npc->getCurrentDirection();
}

void NPCTalking::UpdateState()
{
	if (lastDir != npc->getCurrentDirection())
	{
		npc->SwitchState(npc->talkingState);
	}
	if (!npc->isTargetInInteractRange)
	{
		if (npc->isPlayerInteract == 0)
		{
			npc->SwitchState(npc->idleState);
		}
	}
	if (npc->isTargetInInteractRange)
	{
		if (npc->isPlayerInteract == 0)
		{
			npc->SwitchState(npc->idleState);
		}
	}
}

void NPCTalking::ExitState()
{
	if (npc->getCurrentSprite()->getNumberOfRunningActions() > 0)
		npc->getCurrentSprite()->stopAllActions();
}
