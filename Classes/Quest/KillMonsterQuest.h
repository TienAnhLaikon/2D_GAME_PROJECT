//#ifndef __KILL_MONSTER_QUEST_H__
//#define __KILL_MONSTER_QUEST_H__
//
//#include "cocos2d.h"
//#include "Quest/Quest.h"
//USING_NS_CC;
//
//class KillMonsterQuest : public Quest
//{
//public:
//	struct MonsterKilled {
//		std::string monsterName;
//		int monstersKilled;
//	};
//
//	struct KillMonsterQuestData {
//		std::string questName;
//		std::vector<MonsterKilled> monsters;
//		int status;
//	};
//
//	struct MonsterRequired {
//		std::string monsterName;
//		int monstersRequired;
//	};
//
//	struct KillMonsterQuestBase {
//		std::string questName;
//		std::string questDescription;
//		std::vector<MonsterRequired> monsters;
//	};
//	virtual bool init();
//	void showQuestDetails();
//	KillMonsterQuest();
//	KillMonsterQuest(const std::string& name, const std::string& description, std::vector<MonsterRequired> monstersRequired, std::vector<MonsterKilled> monstersKilled, const QuestRewards& rewards, NPC* npc);
//	~KillMonsterQuest();
//	/*bool checkQuestCompleted() override {
//		if (killedCount >= requiredCount && this->getQuestStatus()== RECEIVED) {
//			this->setQuestStatus(COMPLETED);
//
//			return true;
//
//		}
//		else return false;
//	}
//
// 	int getKilledCount() const { return this->killedCount; }
//	void setKilledCount(int newKilledCount) { this->killedCount = newKilledCount; }
//	void increaseKilledCount() { this->killedCount++; }
//	int getRequiredCount() const { return this->requiredCount; }
//	void setRequiredCount(int newRequiredCount) { this->requiredCount = newRequiredCount; }
//	std::string getMonster() const { return this->monster; }*/
//	KillMonsterQuestData getMonsterQuest() const { return killMonsterQuest; };
//	CREATE_FUNC(KillMonsterQuest);
//private:
//	KillMonsterQuestData killMonsterQuest;
//	KillMonsterQuestBase killMonsterQuestBase;
//};
//
//#endif // __KILL_MONSTER_QUEST_H__