#ifndef __SKILL_BASE_H__
#define __SKILL_BASE_H__

#include "cocos2d.h"
#include "UserInterface/InGame/SkillButton.h"
#include "Engine/Engine.h"
#include "2d/CCLabel.h"
#include "NotificationManager/NotificationManager.h"

USING_NS_CC;
class SkillBase : public Node
{
public:
    PhysicsBody* physicsBody = nullptr;
    Sequence* performEffect;
    struct SkillTalent {
        std::string skillName;
        std::string iconPath;
        int unlockPoint;
        int upgradePoint;
        std::string description;
        std::string skillType;
        SkillBase::SkillTalent* parent;
        std::vector<SkillBase::SkillTalent*> children;
        int levelRequired = 1;
        bool isLock = true;
    };
    bool init();
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void performSkill(Vec2 target);
    int getUnlockScore() const { return this->unlockScore; };
    void unlock() {
        if (isLocked) {
            isLocked = false;
            this->removeChildByName("overlay");
        }
        /*...*/
    };
    bool isLock() const { return isLocked; };
    void setIsLock(bool isLock) {
        this->isLocked = isLock;
    }
    int getSkillIndex() const {
        return this->index;
    }
    void setSkillIndex(int newIndex) {
        this->index = newIndex;
    }
    std::string getSkillName() const { return this->skillName; };
    void setSkillIconSprite(Sprite* spr) { _iconSprite = spr; };
    Sprite* getSkillSprite() const { return this->_skillSprite; };
    Sprite* getSkillIconSprite() const { return this->_iconSprite; };
    Size getSkillIconSize() const { return _iconSprite->getContentSize(); };
    SkillTalent* getSkillTalent() { return skillTalent; };
    //Behaviors
    //About Stats
    int getCurrentLevelSkill() const {
        return this->currentlevelSkill;
    }
    int getMaxLevelSkill() const {
        return this->maxLevelSkill;
    }
    float getSkillDamage() const {
        return this->skillDamage;
    }
    float getSkillDamageBonus() const {
        return this->skillDamage_bonus;
    }
    float getSkillCooldown() const {
        return this->skillCooldown;
    }
    float getSkillCooldownReduct()const {
        return this->skillCooldown_reduct;
    }
    float getManaCost() const {
        return this->manaCost;
    }
    float getHPExtra()const {
        return this->hpExtra;
    }
    float getMPExtra()const {
        return this->mpExtra;
    }
    float getArmorExtra()const {
        return this->armorExtra;
    }
    float getDamageExtra()const {
        return this->dmgExtra;
    }
    //Level Up
    bool levelUpSkill() {
        if (currentlevelSkill == maxLevelSkill) {
            CCLOG("This Skill was Max Level already");
            return false;
        }
        this->currentlevelSkill++;
        //Gain All Stats of Skill: 
        gainAllStatsOfSkill();
        return true;
    }
    void gainAllStatsOfSkill() {
        //AttackSkill
        skillDamage += skillDamage_bonus;
        //Base
        skillCooldown -= skillCooldown_reduct;
        //BuffSkill
        hpExtra += hpExtra_bonus;
        mpExtra += mpExtra_bonus;
        dmgExtra += dmgExtra_bonus;
        armorExtra += armorExtra_bonus;

    }
    SkillButton* _skillButton;
    EventListenerTouchOneByOne* _touchListener;
    Sprite* _overlaySprite;
protected:
    int index;
    int levelRequired;
    SkillTalent* skillTalent;
    ////About SkillTree
    SkillBase* parent;
    std::vector<SkillBase*> children;
    //protected:
    Label* coolDownCountLable;

    Sprite* _iconSprite;
    Sprite* _aoeSprite;
    Sprite* _skillSprite;


    Animate* _skillAnimate;
    Sprite* _skillEffectSprite;
    Animate* _skillEffectAnimate;

    Vec2 lastTarget;
    Vec2 cursorPos;
    float range;
    bool isLocked = true;
    int unlockScore;
    std::string skillName;
    std::string iconSpritePath;

    float effectTime = 0;

    //Skill LevelManager/////////
    //Base Stats
    float manaCost;
    int currentlevelSkill = 0;
    int maxLevelSkill;
    float currentSkillCoolDown;
    float skillCooldown;
    //-Attack Skill
    float skillDamage;
    float damageRaitoOfPlayer;
    //-Buff Skill
    float hpExtra;
    float mpExtra;
    float dmgExtra;
    float armorExtra;

    //Stats Bonus when Level up///////
    //Base Stat
    float skillCooldown_reduct;
    //-Attack Skill
    float skillDamage_bonus;
    //-Buff Skill
    float hpExtra_bonus;
    float mpExtra_bonus;
    float dmgExtra_bonus;
    float armorExtra_bonus;

};

#endif 
