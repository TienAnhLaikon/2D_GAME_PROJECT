//#include "KillMonsterQuest.h"
//
//
//bool KillMonsterQuest::init()
//{
//	
//	return true;
//}
//
//KillMonsterQuest::KillMonsterQuest()
//{
//}
//void KillMonsterQuest::showQuestDetails() {
//
//}
//KillMonsterQuest::KillMonsterQuest(const std::string& name, const std::string& description, std::vector<MonsterRequired> monstersRequired, std::vector<MonsterKilled> monstersKilled, const QuestRewards& rewards, NPC* npc)
//{	
//	killMonsterQuest = KillMonsterQuestData();
//	for (auto monster : monstersKilled) {
//		killMonsterQuest.monsters.push_back(monster);
//	}
//	killMonsterQuest.questName = name;
//	killMonsterQuest.status = NOT_RECEIVED;
//
//	killMonsterQuestBase = KillMonsterQuestBase();
//	for (auto monster : monstersRequired) {
//		killMonsterQuestBase.monsters.push_back(monster);
//	}
//	killMonsterQuestBase.questName = name;
//	killMonsterQuestBase.questDescription = description;
//
//	this->reward = rewards;
//	this->npc = npc;
//
//}
//
//KillMonsterQuest::~KillMonsterQuest()
//{
//}
