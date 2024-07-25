#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "State/PlayerState/PlayerStateBase.h"
#include "State/PlayerState/PlayerAttack.h"
#include "State/PlayerState/PlayerDead.h"
#include "State/PlayerState/PlayerIdle.h"
#include "State/PlayerState/PlayerWalk.h"
#include "State/PlayerState/PlayerSelect.h"
#include "UserInterface/InGame/Joystick.h"
#include "Skills/SkillBase.h"
#include "Skills/SkillFactory.h"
#include "Inventory/Inventory.h"
#include "Inventory/SubInventory.h"
#include "Inventory/UpgradeInventory.h"
#include "BaseCharacter/BaseCharacter.h"

#include "Observer/Subject.h"
#include "Observer/Observer.h"

USING_NS_CC;
class Game;
class GameMap;
class Player : public BaseCharacter, public Subject
{
public:
    static Player* createPlayer();
    ~Player();
    struct PlayerData {
        float positionX;
        float positionY;
        float exp;
        float health;
        float mana;
        float expToConsume;
        int currentQuest;
        int currentMap;
        int prevMap;
    };
    struct PlayerUsingEquipment {
        std::tuple<std::string, int, std::string> weaponNLevel = std::make_tuple("", 0, "");
        std::tuple<std::string, int, std::string> armorNLevel = std::make_tuple("", 0, "");
        std::tuple<std::string, int, std::string> shieldNLevel = std::make_tuple("", 0, "");
        std::tuple<std::string, int, std::string> shoeNLevel = std::make_tuple("", 0, "");
        std::tuple<std::string, int, std::string> helmetNLevel = std::make_tuple("", 0, "");
        std::tuple<std::string, int, std::string> ringNLevel = std::make_tuple("", 0, "");
    };
    std::vector<std::pair<std::string, int>> usingItem;

    int equip_item_sound;
    int pickup_item_sound;
    int drop_item_sound;
    int use_potion_sound;
    int currentMap = 0;
    int prevMap = 0;
    PlayerData getPlayerData();
    std::vector<Node*> listOfCollision;
    std::vector<Node*> listOfSafeZone;
    std::vector<Node*> listOfHideObject;
    std::vector<Node*> listOfShowObjectRoom;
    Vec2 movementDirection;
    Joystick* _joystick;
    PlayerStateBase* currentState;
    PlayerStateBase* attackState;
    PlayerStateBase* walkState;
    PlayerStateBase* idleState;
    PlayerStateBase* deadState;
    PlayerStateBase* selectState;

    // Arm Attack
    Animate* armAttack_E;  // 1
    Animate* armAttack_NE; // 3
    Animate* armAttack_N;  // 5
    Animate* armAttack_NW; // 7
    Animate* armAttack_W;  // 9
    Animate* armAttack_SW; // 11
    Animate* armAttack_S;  // 13
    Animate* armAttack_SE; // 15

    // Arm Walk
    Animate* armWalk_E;  // 1
    Animate* armWalk_NE; // 3
    Animate* armWalk_N;  // 5
    Animate* armWalk_NW; // 7
    Animate* armWalk_W;  // 9
    Animate* armWalk_SW; // 11
    Animate* armWalk_S;  // 13
    Animate* armWalk_SE; // 15

    // Default Walk
    Animate* defaultWalk_E;  // 1
    Animate* defaultWalk_NE; // 3
    Animate* defaultWalk_N;  // 5
    Animate* defaultWalk_NW; // 7
    Animate* defaultWalk_W;  // 9
    Animate* defaultWalk_SW; // 11
    Animate* defaultWalk_S;  // 13
    Animate* defaultWalk_SE; // 15

    // Special Dead
    Animate* specialDeath_E;  // 1
    Animate* specialDeath_NE; // 3
    Animate* specialDeath_N;  // 5
    Animate* specialDeath_NW; // 7
    Animate* specialDeath_W;  // 9
    Animate* specialDeath_SW; // 11
    Animate* specialDeath_S;  // 13
    Animate* specialDeath_SE; // 15

    // Special Select
    Animate* specialSelect_E;  // 1
    Animate* specialSelect_NE; // 3
    Animate* specialSelect_N;  // 5
    Animate* specialSelect_NW; // 7
    Animate* specialSelect_W;  // 9
    Animate* specialSelect_SW; // 11
    Animate* specialSelect_S;  // 13
    Animate* specialSelect_SE; // 15

    // Armed Idle
    Animate* armIdle_E;  // 1
    Animate* armIdle_NE; // 3
    Animate* armIdle_N;  // 5
    Animate* armIdle_NW; // 7
    Animate* armIdle_W;  // 9
    Animate* armIdle_SW; // 11
    Animate* armIdle_S;  // 13
    Animate* armIdle_SE; // 15

    virtual bool init();

    void loadAnimate();

    void setAttackSpeed(float attackSpeed);

    void Initalize(PlayerStateBase* startingState)
    {
        currentState = startingState;
        currentState->EnterState();
    }

    void SwitchState(PlayerStateBase* newState)
    {
        /*if (currentState == newState) {
            CCLOG("Not change to new State!");
            return;
        }*/
        currentState->ExitState();
        currentState = newState;
        currentState->EnterState();
    }
    void setCurrentHP(float newCurrentHP);
    void takeDamage(float amountDamage);
    //Level Manager
    void levelUp() override;
    void levelUpFromData();
    float calculateAngle(const Vec2& vectorA, const Vec2& vectorB);

    void setVisibleSkillButton(bool Visible);
    void handleMove(float deltaTime);
    void regenStats(float dt);

    void update(float dt);
    void resetSkill() { skills.clear(); };
    //void hideSkill() { skills.clear(); };

    bool isSkillBeUnlock(std::string skillName) {
        for (auto skill : skills) {
            if (skill->getSkillTalent()->skillName == skillName) {
                CCLOG("Skill be added !");
                return true;
            }
        }
        return false;
    }
    void addSkill(std::string skillName);
    void setSkill(SkillBase* skill);
    void resetSkillSlotOrder() {
        Size size = Director::getInstance()->getVisibleSize();
        float scale = size.width / size.height * 2;
        auto sizeWD4 = size.width / 4;
        auto sizeHD4 = size.height / 4;
        for (auto skill : skills) {

            auto offsetW = skill->_skillButton->skillButtonBorder->getContentSize().width / 2;
            auto offsetH = skill->_skillButton->skillButtonBorder->getContentSize().height / 2;
            if (skill->getSkillIndex() != 0) {
                if (!skill->isVisible()) {
                    skill->setVisible(true);
                    skill->_touchListener->setEnabled(true);
                }
            }
            else {
                if (skill->isVisible()) {
                    skill->setVisible(false);
                    skill->_touchListener->setEnabled(false);
                }
            }
            // normal attack
            if (skill->getSkillIndex() == 5) {
                if (!skill->_skillButton->isVisible())skill->_skillButton->setVisible(true);
                skill->_skillButton->setScale(1 * Director::getInstance()->getContentScaleFactor());
                skill->_skillButton->setPosition(Vec2(sizeWD4 / scale, -sizeHD4 / scale) + Vec2(168, -30));
                skill->_skillButton->setName("SkillButton5");
            }

            // skill 1
            if (skill->getSkillIndex() == 1) {
                if (!skill->_skillButton->isVisible())skill->_skillButton->setVisible(true);
                skill->_skillButton->setScale(0.75 * Director::getInstance()->getContentScaleFactor());
                skill->_skillButton->setPosition(Vec2(sizeWD4 / scale, -sizeHD4 / scale) + Vec2(80 + 10, -70));
                skill->_skillButton->setName("SkillButton1");
            }
            // skill 2
            if (skill->getSkillIndex() == 2) {
                if (!skill->_skillButton->isVisible())skill->_skillButton->setVisible(true);
                skill->_skillButton->setScale(0.75 * Director::getInstance()->getContentScaleFactor());
                skill->_skillButton->setPosition(Vec2((sizeWD4 + offsetW) / scale - 7, (-sizeHD4 + offsetH) / scale + 8) + Vec2(60 + 10 - 5, -40 - 5));
                skill->_skillButton->setName("SkillButton2");
            }
            // skill 3
            if (skill->getSkillIndex() == 3) {
                if (!skill->_skillButton->isVisible())skill->_skillButton->setVisible(true);
                skill->_skillButton->setScale(0.75 * Director::getInstance()->getContentScaleFactor());
                skill->_skillButton->setPosition(Vec2((sizeWD4 + 2 * offsetW) / scale - 9, (-sizeHD4 + 2 * offsetH) / scale + 10) + Vec2(87 -10, -30 -10));
                skill->_skillButton->setName("SkillButton3");
            }
            // skill 4
            if (skill->getSkillIndex() == 4) {
                if (!skill->_skillButton->isVisible())skill->_skillButton->setVisible(true);
                skill->_skillButton->setScale(0.75 * Director::getInstance()->getContentScaleFactor());
                skill->_skillButton->setPosition(Vec2((sizeWD4 + 3 * offsetW) / scale, (-sizeHD4 + 3 * offsetH) / scale) + Vec2(118 -15, -50 - 10));
                skill->_skillButton->setName("SkillButton4");
            }
            if (skill->getSkillIndex() == 0) {
                if (skill->_skillButton->isVisible())skill->_skillButton->setVisible(false);
                skill->_skillButton->setPosition(Vec2(1000, 1000));
                skill->_skillButton->setName("SkillButton");
            }
        }
    }
    std::vector<SkillBase*> getPlayerSkills() {
        return this->skills;
    }
    void increaseSkillPoint(int pnt)
    {
        this->skillPoint += pnt;
    };
    void decreaseSkillPoint(int pnt)
    {
        this->skillPoint -= pnt;
    };
    int getSkillPoint() const { return skillPoint; };
    void setEquipment(std::string equipmentName, int equipmentLv, std::string equipmentType);
    BaseEquipment* getEquipment(std::string equipmentType);
    float getEquipmentDamage() const { return equipmentDamage; };
    float getEquipmentSkillDamage() const { return equipmentSkillDamage; };
    float getEquipmentHP() const { return equipmentHP; };
    float getEquipmentMP() const { return equipmentMP; };
    float getEquipmentMS() const { return equipmentMS; };
    float getEquipmentCDR() const { return equipmentCDR; };
    float getEquipmentArmor() const { return equipmentArmor; };
    void setEquipmentDamage(float newDmg) { equipmentDamage = newDmg; };
    void setEquipmentSkillDamage(float newDmg) { equipmentSkillDamage = newDmg; };
    void setEquipmentHP(float newHp) { equipmentHP = newHp; };
    void setEquipmentMP(float newMp) { equipmentMP = newMp; };
    void setEquipmentMS(float newMs) { equipmentMS = newMs; };
    void setEquipmentCDR(float newCDR) { equipmentCDR = newCDR; };
    void setEquipmentArmor(float newArm) { equipmentArmor = newArm; };
    void removeEquipment(std::string equipmentType);
    Inventory* getInventory() const { return inventory; };
    UpgradeInventory* getUpgradeInventory() const { return upgradeInventory; };
    SubInventory* getSubInventory() const { return subInventory; };
    void pickUpEquipment(std::string equipmentName, int equipmentLv);
    void pickUpItem(std::string itemName);
    void dropItem(std::string itemName);
    std::pair<std::string, int> getItem(std::string itemName);
    void dropEquipment(std::string itemName);
    std::vector<std::string> getAllItem();
    bool isMoving() const { return moving; };
    void setMoving(bool isMove) { moving = isMove; };
    bool isCanMove(const Vec2& newPosition);
    bool isHideObject(const Vec2& newPosition);
    bool isInHidePlayerZone(const Vec2& newPosition);
    void setTiledMap(GameMap* newMap) { map = newMap; };
    void upgradeEquipment(std::string);
    void increaseStat(float amount, std::string statName);
    void decreaseStat(float amount, std::string statName);
    std::vector<BaseEquipment::EquipmentData> getAllEquipment();
    void usePotion(std::string potionName);
    PlayerUsingEquipment getPlayerUsingEquipmentData() const { return _playerUsingEquipment; };
    PlayerUsingEquipment _playerUsingEquipment;
    void equipWeapon(std::string name, int level);
    void equipShoe(std::string name, int level);
    void equipArmor(std::string name, int level);
    void equipRing(std::string name, int level);
    void equipShield(std::string name, int level);
    void equipHelmet(std::string name, int level);
    void equipItem(std::string name, int quantity);

    //Observer
    void registerObserver(Observer* observer) override {
        observers.push_back(observer);
    }

    void removeObserver(Observer* observer) override {
        auto it = std::find(observers.begin(), observers.end(), observer);
        if (it != observers.end()) {
            observers.erase(it);
        }

    }
    void notifyObservers() override {
        for (auto observer : observers) {

            observer->update(message);
        }
    }

    void setMessage(const std::string& newMessage) {
        message = newMessage;
        notifyObservers();
    }

    bool isShowObjectRoom(const Vec2& newPosition);

    void regenMP(float amount) {
        auto currentMPResult = currentMP + amount;
        if (currentMPResult >= MP + equipmentMP)currentMPResult = MP;
        currentMP = currentMPResult;
    }

    void regenHP(float amount) {
        auto currentHPResult = currentHP + amount;
        if (currentHPResult >= HP + equipmentHP)currentHPResult = HP;
        currentHP = currentHPResult;
    }
    void gainExp(float amountExp);
    void gainExpFromData(float amountExp);
    //Observer props
    std::vector<Observer*> observers;
    std::string message;
    bool canMovingTutorial = true;
    std::vector<SkillBase*> skills;
    CREATE_FUNC(Player);

protected:
    Game* currentScene = nullptr;
    Sprite* highlight;
    GameMap *map;
    bool moving = false;

    BaseEquipment *weapon;
    BaseEquipment *ring;
    BaseEquipment *armor;
    BaseEquipment *helmet;
    BaseEquipment *shield;
    BaseEquipment *shoe;
    Inventory *inventory;
    UpgradeInventory *upgradeInventory;
    SubInventory* subInventory;
    float equipmentDamage = 0;
    float equipmentSkillDamage = 0;
    float equipmentHP = 0;
    float equipmentMP = 0;
    float equipmentMS = 0;
    float equipmentCDR = 0;
    float equipmentArmor = 0;
};

#endif //
