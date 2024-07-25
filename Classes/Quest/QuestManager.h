#ifndef __QUEST_MANAGER_H__
#define __QUEST_MANAGER_H__

#include "cocos2d.h"
#include "Quest/Quest.h"
#include "Observer/Observer.h"
#include "NotificationManager/NotificationManager.h"

class Player;
class NPC;
USING_NS_CC;
class QuestManager : public Node, public Observer
{
public:
    bool init(std::vector<NPC*> listNPC);

    static QuestManager* getInstance()
    {
        if (!s_instance)
        {
            s_instance = new QuestManager();
        }
        return s_instance;
    }
    void setNPC(NPC* durin);
    //void setPlayer(Player* player);

    Quest::QuestData getCurrentQuestData() {
        if(currentQuest)
        return Quest::QuestData(currentQuest->questName, currentQuest->enemies, currentQuest->items, currentQuest->questStatus);
        else return Quest::QuestData("No More Quest", std::vector<Quest::Enemy*>(), std::vector<Quest::Item*>(), 0);
    }

    void destroyInstance() {
        if (s_instance != nullptr) {
            if (s_instance->getParent() != nullptr) {
                s_instance->removeFromParentAndCleanup(true);
            }


            s_instance->release(); // Gii phng i tng
            s_instance = nullptr;
        }
    }
    void update(const std::string& message) {
        messageFromSubject = message;
        //Kill Enemies Quest
        Quest::Enemy* enemyResult = nullptr;
        for (auto enemy : currentQuest->enemies) {
            if (enemy->name == message) {
                if (enemy->killed < enemy->required && currentQuest->questStatus == currentQuest->RECEIVED) {
                    enemy->killed++;
                    //std::string processingText = enemy->name + ": "+std::to_string(enemy->killed) + " / " + std::to_string(enemy->required);
                    //Label* progressOfEnemies = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 15);
                    //progressOfEnemies->setColor(Color3B::GRAY);
                    //progressOfEnemies->setName("ProgressEnemies");
                    //progressOfEnemies->setPosition(Vec2(0, 0));
                    //progressOfEnemies->setString(processingText);
                    ////InGameUI::getInstance(player)->addChild(progressOfEnemies, 100);
                }
                enemyResult = enemy;
            }
        }
        if (enemyResult) {
            std::string nameLabel = message + " Label";
            for (auto progressLabel : currentQuest->progressLabelList) {
                if (progressLabel->getName() == nameLabel) {
                    progressLabel->setString(StringUtils::format("%s: %d / %d", enemyResult->name.c_str(), enemyResult->killed, enemyResult->required));
                }
            }
        }
        //Collect Items Quest
        Quest::Item* itemResult = nullptr;
        for (auto item : currentQuest->items) {
            if (item->name == message) {
                if (item->collected < item->required && currentQuest->questStatus == currentQuest->RECEIVED) {
                    item->collected++;
                    //std::string processingText = item->name + ": " + std::to_string(item->collected) + " / " + std::to_string(item->required);
                    //Label* progressOfItems = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 15);
                    //progressOfItems->setColor(Color3B::GRAY);
                    //progressOfItems->setName("ProgressItems");
                    //progressOfItems->setPosition(Vec2(0, 20));
                    //progressOfItems->setString(processingText);
                    ////InGameUI::getInstance(player)->addChild(progressOfItems, 100);
                }
                itemResult = item;
            }
        }
        if (itemResult) {
            std::string nameLabel = message + " Label";
            for (auto progressLabel : currentQuest->progressLabelList) {
                if (progressLabel->getName() == nameLabel) {
                    progressLabel->setString(StringUtils::format("%s: %d / %d", itemResult->name.c_str(), itemResult->collected, itemResult->required));
                    currentProgressLabel = progressLabel->getString();
                }
            }
        }
        isCurrentQuestComplete();

    }
    bool onTouchBegan(Touch* touch, Event* event);
    void changeQuestIconOfNPC(int questStatus);
    void showAndHideNoQuest(bool isShow);
    void resetOrderOfListQuest();
    //int 
    bool isCurrentQuestComplete();
    void increaseCurrentMap() { currentMap++; };
    int getCurrentMap() const { return currentMap; };
    void detailDisplay();
    void generalDisplay();
    int currentQuestIndex = 0;
    Quest* currentQuest = nullptr;
    int indexQuest;
    std::vector<Quest*> listQuest;

    Sprite* questNotReceiveIcon = nullptr;
    Sprite* questProcessingIcon = nullptr;
    Sprite* questPendingIcon = nullptr;
    Label* noQuestLabel = nullptr;
    std::string currentProgressLabel = "";
    bool isShowDetail = false;
    Sprite* questManagerUI = nullptr;
private:
    int currentMap = 0;
    //Construct and Destruct
    QuestManager();
    ~QuestManager();

    //Message from Subject
    std::string messageFromSubject;
    //Props


    NPC* durin = nullptr;

    Player* player = nullptr;

    static QuestManager* s_instance;

};
#endif // __QUEST_H__
