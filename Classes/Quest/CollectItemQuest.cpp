//#include "CollectItemQuest.h"
//
//bool CollectItemQuest::init()
//{
//	return true;
//}
//
//CollectItemQuest::CollectItemQuest()
//{
//}
//
//CollectItemQuest::CollectItemQuest(const std::string& questName, const std::string& questDescription, std::vector<ItemRequired> itemsRequired, std::vector<ItemCollected> itemsCollected, const QuestRewards& rewards, NPC* npc)
//{
//	collectItemQuest = CollectItemQuestData();
//	for (auto i : itemsCollected) {
//		collectItemQuest.items.push_back(i);
//	}
//	collectItemQuest.status = NOT_RECEIVED;
//	collectItemQuest.questName = questName;
//	this->reward = rewards;
//
//
//	collectItemQuestBase = CollectItemQuestBase();
//	for (auto i : itemsRequired) {
//		collectItemQuestBase.items.push_back(i);
//	}
//	collectItemQuestBase.questName = questName;
//	collectItemQuestBase.questDescription = questDescription;
//	this->npc = npc;
//}
//
//CollectItemQuest::~CollectItemQuest()
//{
//}
