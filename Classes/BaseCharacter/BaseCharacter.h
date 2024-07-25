#ifndef __BASE_CHARACTER_H__
#define __BASE_CHARACTER_H__

#include "cocos2d.h"
#include "../Equipment/BaseEquipment.h"
#include "ui/UILoadingBar.h"
#include "2d/CCLabel.h"
#include "UserInterface/InGame/InGameUI.h"
#include "UserInterface/InGame/ExpBar.h"
#include "NotificationManager/NotificationManager.h"

USING_NS_CC;

class BaseCharacter : public Node
{
public:
    virtual bool init();


    void takeDamage(float amountDamage) {

    }

    void noManaAlert() {
        //Lable
        auto noManaLable = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 24);
        noManaLable->setColor(Color3B::BLUE);
        noManaLable->setScale(0.5);
        this->addChild(noManaLable);
        noManaLable->setString(StringUtils::format("%s", "Not enough Mana"));
        Vec2 pos = Vec2(0, characterCurrentSprite->getBoundingBox().size.height / 2);
        auto moveTo = MoveTo::create(1.0f, pos);
        auto fadeOut = FadeOut::create(1.0f);
        auto spawn = Spawn::create(fadeOut, moveTo, nullptr);
        auto sqe = Sequence::create(spawn, RemoveSelf::create(), nullptr);
        noManaLable->runAction(sqe);
        //
    }

    void dealDamage(Node* target);

    //Direction
    int getCurrentDirection() {
        return this->direction;
    }
    void setCurrentDirection(int newDirection) {
        this->direction = newDirection;
    }
    //AttackSpeed
    float getAttackSpeed() const {
        return this->attackSpeed;
    }
    //AttackRange
    float getAttackRange() const {
        return this->attackRange;
    }
    void setAttackRange(float newAttackRange) {
        this->attackRange = newAttackRange;
    }
    //MovementSpeed
    float getMovementSpeed() const {
        return this->MS;
    }
    void setMovementSpeed(float newMovementSpeed) {
        this->MS = newMovementSpeed;
    }

    float getDamage() const {
        return this->baseDamage;
    }
    void setDamage(float newDmg) {
        this->baseDamage = newDmg;
    }
    float getAPDamage() const {
        return this->AP;
    }
    void setAPDamage(float newAPDmg) {
        this->AP = newAPDmg;
    }
    //HP
    void setHP(float newHP) {
        this->HP = newHP;
    }

    float getHP() const {
        return this->HP;
    }
    void setCurrentHP(float newCurrentHP) {
        if (newCurrentHP >= HP) newCurrentHP = HP;
        this->currentHP = newCurrentHP;
    }
    float getCurrentHP() const {
        return this->currentHP;
    }
    //MP
    float getMP() const {
        return this->MP;
    }
    void setMP(float newMP) {
        this->MP = newMP;
    }
    float getCurrentMP() const {
        return this->currentMP;
    }
    void setCurrentMP(float newCurrentMP) {
        if (currentMP > newCurrentMP) {
            //Lable
            float amount = currentMP - newCurrentMP;
            int amountToInt = std::floor(amount);
            auto noManaLable = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 24);
            noManaLable->setColor(Color3B::BLUE);
            noManaLable->setScale(0.5);
            this->addChild(noManaLable);
            noManaLable->setString(StringUtils::format("%-d", amountToInt));
            Vec2 pos = Vec2(0, characterCurrentSprite->getBoundingBox().size.height / 2);
            auto moveTo = MoveTo::create(1.0f, pos);
            auto fadeOut = FadeOut::create(1.0f);
            auto spawn = Spawn::create(fadeOut, moveTo, nullptr);
            auto sqe = Sequence::create(spawn, RemoveSelf::create(), nullptr);
            noManaLable->runAction(sqe);
        }
        if (currentMP <= newCurrentMP) {
            newCurrentMP = currentMP;
        }
        this->currentMP = newCurrentMP;
    }
    //HP Regen
    void setHPRegen(float newHPRegen) {
        this->HP_regen = newHPRegen;
    }

    float getHPRegen() const {
        return this->HP_regen;
    }
    //MP Regen
    void setMPRegen(float newMPRegen) {
        this->MP_regen = newMPRegen;
    }

    float getMPRegen() const {
        return this->MP_regen;
    }
    //Armor
    float getArmor() const {
        return this->baseArmor;
    }
    void setArmor(float newArm) {
        this->baseArmor = newArm;
    }
    //CDR
    float getCDR() const {
        return this->baseCDR;
    }
    void setCDR(float newCDR) {
        this->baseCDR = newCDR;
    }
    //MS
    float getMS() const {
        return this->MS;
    }
    void setMS(float newMS) {
        this->MS = newMS;
    }
    //EXP gain
    float getExpGain() const {
        return this->expGain;
    }
    ////Buff
    //void gainCurrentHP(float amountHP) {
    //    currentHP += amountHP;
    //}
    //void gainCurrentMP(float amountMP) {
    //    currentMP += amountMP;
    //}
    //void gainArmor(float amountArmor) {
    //    baseArmor += amountArmor;
    //}
    //void gainDamage(float amountDamage) {
    //    baseDamage += amountDamage;
    //}

    //Sprite
    Sprite* getCurrentSprite() const {
        return this->characterCurrentSprite;
    }
    void setSprite(Sprite* newSprite) {
        characterCurrentSprite = newSprite;
    }

    //Level Manager
    virtual void levelUp() {
        if (level < maxLevel) {
            level++;
            //Gain Stats and Recover Full HP and MP
            gainAllStats();
            backToFullHealthAndMana();
            //Gain Exp Required To Level Up
            setExpRequiredToLevelUp();
            
        }
    }
    bool _isGainExp = false;
    
    void setExpRequiredToLevelUp() {
        expRequiredToLevelUp *= 1.5;
    }
    int getLevel() const {
        return this->level;
    }
    float getExpRequiredToLevelUp() const {
        return this->expRequiredToLevelUp;
    }
    float getCurrentExp() const {
        return this->currentExp;
    }
    //Gain All Stats
    void gainAllStats() {
        HP *= HP_bonus;
        MP *= MP_bonus;
        HP_regen *= HP_regen_bonus;
        MP_regen *= MP_regen_bonus;
        baseArmor *= Armor_bonus;
        MS *= MS_bonus;
        attackSpeed *= AS_bonus;
        attackRange *= AR_bonus;
        baseDamage *= AD_bonus;
        AP *= AP_bonus;
        expGain += expGain_bonus;
        skillPoint += 10;
    }
    //Back To Full HP and MP
    void backToFullHealthAndMana() {
        currentHP = HP;
        currentMP = MP;
    }
    float getAllExp() const { return (280 * (pow(1.5, this->getLevel() - 1) - 1) / 0.5) + currentExp; }
    float getExpToConsume() const { return expToConsume; };
    void setExpToConsume(const float amount) { expToConsume = amount; };
    void decreaseExpToConsume(const float amount) { expToConsume -= amount; };
    CREATE_FUNC(BaseCharacter);
protected:
    int skillPoint = 10000;
    //LEVEL MANAGER
    //Exp
    float currentExp;
    float expRemain;
    float expRequiredToLevelUp;
    //Level
    int level;
    int maxLevel;
    float expToConsume = 0;
    //Stats
    float currentHP;
    float HP_regen;
    float MP_regen;
    float currentMP;
    float HP;
    float MP;
    float baseArmor;
    float baseCDR;
    float baseDamage;
    float AP;
    float MS;
    float attackSpeed;
    float attackRange;
    float expGain;

    //Stats Bonus
    float HP_bonus;
    float MP_bonus;
    float HP_regen_bonus;
    float MP_regen_bonus;
    float Armor_bonus;
    float AD_bonus;
    float MS_bonus;
    float AR_bonus;
    float AP_bonus;
    float AS_bonus;
    float expGain_bonus;

    //
    ui::LoadingBar* healthBar;
    ui::LoadingBar* expBar;
    Sprite* backGroundBar;
    Sprite* characterCurrentSprite;
    Label* monsterName;
    Label* levelLabel;
    Sprite* levelSprite;
    int direction;
    //std::vector<Skill*> skills;
};

#endif // __HELLOWORLD_SCENE_H__



