//#ifndef __KILL_BOSS_QUEST_H__
//#define __KILL_BOSS_QUEST_H__
//
//#include "cocos2d.h"
//#include "Quest/Quest.h"
//USING_NS_CC;
//
//class KillBossQuest : public Quest
//{
//public:
//	struct KillBossQuestData {
//		std::string questName;
//		bool isBossDefeated;
//		int status;
//	};
//	struct KillBossQuestBase {
//		std::string questName;
//		std::string questDescription;
//		std::string bossName;
//	};
//	virtual bool init();
//	KillBossQuest();
//	KillBossQuest(const std::string& name, const std::string& description, const QuestRewards& rewards, const std::string& bossType, NPC* npc);
//	~KillBossQuest() {};
//	std::string getBoss() const { return killBossQuestBase.bossName; }
//	void setBoss(std::string newBoss) { killBossQuestBase.bossName = newBoss; }
//	bool checkQuestCompleted() override {
//		if (killBossQuestData.isBossDefeated && this->getQuestStatus() == RECEIVED) {
//			this->setQuestStatus(COMPLETED);
//			return true;
//		}
//		else return false;
//	}
//
//	bool getIsBossDefeated() const { return killBossQuestData.isBossDefeated; }
//	void setIsBossDefeated(bool checkBossDie) { killBossQuestData.isBossDefeated = checkBossDie; }
//	KillBossQuestData getBossQuest() const { return killBossQuestData; };
//	CREATE_FUNC(KillBossQuest);
//private:
//	KillBossQuestData killBossQuestData;
//	KillBossQuestBase killBossQuestBase;
//};
//
//#endif // __KILL_MONSTER_QUEST_H__