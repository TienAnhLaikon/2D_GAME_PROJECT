//#ifndef __COLLECT_ITEM_QUEST_H__
//#define __COLLECT_ITEM_QUEST_H__
//
//#include "cocos2d.h"
//#include "Quest/Quest.h"
//USING_NS_CC;
//
//class CollectItemQuest : public Quest
//{
//public:
//	struct ItemCollected {
//		std::string itemName;
//		int numberItemCollected;
//	}; 
//
//	struct ItemRequired {
//		std::string itemName;
//		int numberItemRequired;
//	};
//
//	struct CollectItemQuestData {
//		std::string questName;
//		std::vector<ItemCollected> items;
//		int status;
//	};
//
//	struct CollectItemQuestBase {
//		std::string questName;
//		std::string questDescription;
//		std::vector<ItemRequired> items;
//
//	};
//	virtual bool init();
//	CollectItemQuest();
//	CollectItemQuest(const std::string& questName, const std::string& questDescription, std::vector<ItemRequired> itemsRequired, std::vector<ItemCollected> itemsCollected, const QuestRewards& rewards, NPC* npc);
//	~CollectItemQuest();
//	/*bool checkQuestCompleted() override {
//		if (collectedCount >= requiredCount) {
//			this->setQuestStatus(COMPLETED);
//			return true;
//		}
//		else return false;
//	}
//
//	int getCollectedCount() const { return this->collectedCount; }
//	void setKilledCount(int newCollectedCount) { this->collectedCount = newCollectedCount; }
//	void increaseCollectedCount() { this->collectedCount++; }
//	int getRequiredCount() const { return this->requiredCount; }
//	void setRequiredCount(int newRequiredCount) { this->requiredCount = newRequiredCount; }*/
//	CollectItemQuestData getCollectItemQuest() const { return collectItemQuest; };
//	CREATE_FUNC(CollectItemQuest);
//private:
//	CollectItemQuestData collectItemQuest;
//	CollectItemQuestBase collectItemQuestBase;
//};
//
//#endif // __COLLECT_ITEM_QUEST_H__