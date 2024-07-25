#ifndef __QUEST_H__
#define __QUEST_H__

#include "cocos2d.h"
#include "Item/BaseItem.h"
USING_NS_CC;
class NPC;

class Quest : public Node
{
public:
    struct QuestRewards {
        int gold;
        float exp;
        std::string itemName;

        /*QuestRewards(const int g, const float e, const std::string& n) : gold(g), exp(e), itemName(n) {}*/
    };
    // nh ngha cc hng s c tn  i din cho cc trng thi ca nhim v
    const int NOT_RECEIVED = 0;
    const int RECEIVED = 1;
    const int COMPLETED = 2;
    const int COMPLETE_PENDING = 3;
    //Quest type
    const int KILL_ENEMIES_QUEST = 0;
    const int COLLECT_ITEM_QUEST = 1;
    struct Enemy {
        std::string name;
        int killed;
        int required;

        Enemy(const std::string& n, int k, int r) : name(n), killed(k), required(r) {}
    };

    struct Item {
        std::string name;
        int collected;
        int required;
        
        Item(const std::string& n, int c, int r) : name(n), collected(c), required(r) {}
    };
    struct QuestData {
        std::string questName = "";
        std::vector<Enemy*> enemies;
        std::vector<Item*> items;
        int questStatus;
        QuestData(const std::string& n = "", std::vector<Enemy*> e = std::vector<Enemy*>(), std::vector<Item*> i = std::vector<Item*>(), int s = 0) : questName(n), enemies(e), items(i), questStatus(s) {}

    };

    Quest(const std::string questName, const std::string description, std::vector<Item*>items, 
        std::vector<Enemy*> enemies, const int questType, int questStatus, const QuestRewards rewards, NPC* npc);

    Quest();
    ~Quest();
    static Quest* create(const std::string questName, const std::string description, std::vector<Item*>items,
        std::vector<Enemy*> enemies, const int questType, const QuestRewards rewards, NPC* npc);

    virtual bool init(const std::string questName, const std::string description, std::vector<Item*>items,
        std::vector<Enemy*> enemies, const int questType, const QuestRewards rewards, NPC* npc);

    std::vector<std::string> questDialogue;
    std::vector<std::string> processingQuestDialogue;
    std::vector<std::string> completedQuestDialogue;
    
    std::string splitLines(const std::string& str) {
        std::string result = "";
        std::stringstream ss(str); // To stringstream t chui u vo
        std::string word;
        int count = 0;

        while (ss >> word) {
            if (count == 7) {
                word += "\n";
                count = 0;
            }
            result += word + " ";
            count++;
        }

        return result;
    }
    //////////// MAIN INTERFACE//////////
    std::string questName;
    std::vector<Label*> progressLabelList;
    std::string questDescription;
    
    //Other Props
    QuestRewards reward;
    int questStatus;
    NPC* npc;
    std::vector<Item*> items;
    std::vector<Enemy*> enemies;

    //Common Props

    int questType;
    ///////////
    //Quest Detail Sprite
    Sprite* questUI;
    Label* nameLabel;
    Label* descriptionLabel;
    Label* questInformationLabel;
    Label* objectiveLabel;
};

#endif // __QUEST_H__



