#include "NPCIdle.h"
#include "NPC/NPC.h"

void NPCIdle::EnterState()
{
	// CCLOG("NPC Hello From Idle State");
	if (npc->getCurrentDirection() == 1)
	{
		npc->getCurrentSprite()->runAction(RepeatForever::create(npc->NPC_Idle_E));
	}
	if (npc->getCurrentDirection() == 2)
	{
		npc->getCurrentSprite()->runAction(RepeatForever::create(npc->NPC_Idle_NE));
	}
	if (npc->getCurrentDirection() == 3)
	{
		npc->getCurrentSprite()->runAction(RepeatForever::create(npc->NPC_Idle_N));
	}

	if (npc->getCurrentDirection() == 4)
	{
		npc->getCurrentSprite()->runAction(RepeatForever::create(npc->NPC_Idle_NW));
	}

	if (npc->getCurrentDirection() == 5)
	{
		npc->getCurrentSprite()->runAction(RepeatForever::create(npc->NPC_Idle_W));
	}

	if (npc->getCurrentDirection() == 6)
	{
		npc->getCurrentSprite()->runAction(RepeatForever::create(npc->NPC_Idle_SW));
	}

	if (npc->getCurrentDirection() == 7)
	{
		npc->getCurrentSprite()->runAction(RepeatForever::create(npc->NPC_Idle_S));
	}

	if (npc->getCurrentDirection() == 8)
	{
		npc->getCurrentSprite()->runAction(RepeatForever::create(npc->NPC_Idle_SE));
	}

	lastDir = npc->getCurrentDirection();
}

void NPCIdle::UpdateState()
{
	// CCLOG("UPDATE STATE");
	//  nu Player ng  tm tng tc vi NPC th i trng thi sang talking
	if (lastDir != npc->getCurrentDirection())
	{
		npc->SwitchState(npc->idleState);
	}
	if (npc->isTargetInInteractRange)
	{
		if (npc->isPlayerInteract > 0)
		{
			npc->SwitchState(npc->talkingState);
		}
	}
	else if (!(npc->isTargetInInteractRange))
	{
		npc->SwitchState(npc->idleState);
	}
}

void NPCIdle::ExitState()
{

	if (npc->getCurrentSprite()->getNumberOfRunningActions() > 0)
		npc->getCurrentSprite()->stopAllActions();
}
