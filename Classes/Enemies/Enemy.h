#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"
#include "BaseCharacter/BaseCharacter.h"

#include "Observer/Subject.h"
#include "Observer/Observer.h"

#include "Quest/QuestManager.h"

USING_NS_CC;

class Enemy : public BaseCharacter, public Subject
{
public:
    virtual bool init();
    void takeDamage(float amountDamage) {
        float damageActuallyTake = amountDamage * (100 / (100 + this->baseArmor));
        if (this->currentHP <= damageActuallyTake) {
            currentHP = 0;
        }
        else {
            currentHP -= damageActuallyTake;
        }
        healthBar->setPercent((currentHP / HP) * 100);
        //Lable
        auto dmgTakeLable = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 10);
        dmgTakeLable->setColor(Color3B::RED);
        this->addChild(dmgTakeLable);
        int dmgToInt = std::floor(damageActuallyTake);
        dmgTakeLable->setString(StringUtils::format("-%d", dmgToInt));
        Vec2 pos = Vec2(0, characterCurrentSprite->getBoundingBox().size.height / 2);
        auto moveTo = MoveTo::create(1.0f, pos);
        auto fadeOut = FadeOut::create(1.0f);
        auto spawn = Spawn::create(fadeOut, moveTo, nullptr);
        auto sqe = Sequence::create(spawn, RemoveSelf::create(), nullptr);
        dmgTakeLable->runAction(sqe);
    }
    void setAAcanDealDamage(bool isCan) {
        AAcanDealDamage = isCan;
    }
    bool getAAcanDealDamage() const {
        return this->AAcanDealDamage;
    }
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

            observer->update(this->getName());
        }
    }
    void setMessage(const std::string& newMessage) {
        message = newMessage;
        notifyObservers();
    }

    void setLevel(int newLevel) {
        this->level = newLevel;
        for (int i = 0; i < level; i++) {
            this->levelUp();
        }
        this->levelLabel->setString(std::to_string(newLevel));
    }

    bool isDead = false;
    CREATE_FUNC(Enemy);

protected:
    //Observer props
    std::vector<Observer*> observers;
    std::string message;

    bool AAcanDealDamage;
};

#endif 
