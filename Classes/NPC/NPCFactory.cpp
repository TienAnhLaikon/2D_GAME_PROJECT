#include "NPCFactory.h"

NPC *NPCFactory::createNPC(std::string NPCType)
{
	NPC *result = nullptr;

	if (NPCType == "Robert The Fallen Slayer")
	{
		result = Robert::create();
	}
	else if (NPCType == "Durin The Blacksmith")
	{
		result = Durin::create();
	}
	else if (NPCType == "Thien An The First Slayer") {
		result = ThienAn::create();
	}

	return result;
}