
#ifndef __HEALTH_BAR__
#define __HEALTH_BAR__

#include "cocos2d.h"
#include "Inventory/SubInventory.h"
USING_NS_CC;


class HealthBar : public Node {
public:
    static HealthBar* create(const std::string& borderImage, const std::string& insideImage, float hp, float mp) {
        HealthBar* healthBar = new HealthBar();
        if (healthBar && healthBar->initWithImages(borderImage, insideImage, hp, mp)) {
            healthBar->autorelease();
            return healthBar;
        }
        CC_SAFE_DELETE(healthBar);
        return nullptr;
    }

    bool initWithImages(const std::string& borderImage, const std::string& insideImage, float hp, float mp) {
        if (!Node::init()) {
            return false;
        }
        currentHealth = hp;
        maxHealth = hp;
        currentMana = mp;
        maxMana = mp;


        healthLabel = Label::createWithTTF("HP - " + std::to_string(static_cast<int>(currentHealth)) + "/" + std::to_string(static_cast<int>(maxHealth)), "fonts/Diablo Light.ttf", 15);
        manaLabel = Label::createWithTTF("MP - " + std::to_string(static_cast<int>(currentMana)) + "/" + std::to_string(static_cast<int>(maxMana)), "fonts/Diablo Light.ttf", 15);
        this->addChild(healthLabel);
        this->addChild(manaLabel);
        healthLabel->setVisible(false);
        manaLabel->setVisible(false);
        updateHealthBar(currentHealth, currentMana, maxHealth, maxMana);

        return true;
    }

    //SubInventory* getSubInventory() const { return subInventory; };

    //void increaseHealth(float amount) {
    //    if (currentHealth <= currentHealth + amount)
    //        currentHealth = currentHealth + amount;
    //    else
    //        currentHealth = maxHealth;
    //    maxHealthLabel->setString(std::to_string(static_cast<int>(currentHealth)) + " / " + std::to_string(static_cast<int>(maxHealth)));

    //}

    //void decreaseHealth(float amount) {
    //    if (currentHealth - amount >= 0)
    //        currentHealth = currentHealth - amount;
    //    else
    //        currentHealth = 0;
    //    maxHealthLabel->setString(std::to_string(static_cast<int>(currentHealth)) + " / " + std::to_string(static_cast<int>(maxHealth)));

    //}

    //void increaseMana(float amount) {
    //    if (currentMana <= currentMana + amount)
    //        currentMana = currentMana + amount;
    //    else
    //        currentMana = maxMana;
    //    //maxHealthLabel->setString(std::to_string(static_cast<int>(m_currentHealth)) + " / " + std::to_string(static_cast<int>(m_maxHealth)));

    //}

    //void decreaseMana(float amount) {
    //    if (currentMana - amount >= 0)
    //        currentMana = currentMana - amount;
    //    else
    //        currentMana = 0;
    //    //maxHealthLabel->setString(std::to_string(static_cast<int>(m_currentHealth)) + " / " + std::to_string(static_cast<int>(m_maxHealth)));

    //}
    //void increaseMaxHealth(float amount) {
    //    maxHealth += amount;

    //}
    //void increaseMaxMana(float amount) {
    //    maxMana += amount;

    //}
    float getHeight() const { if (healthBar) return healthBar->getContentSize().height; else return 0; };
    float getHealth() const { return currentHealth; };

    void updateHealthBar(float currentHealth, float currentMana, float maxHealth, float maxMana) {
        if (healthBarAndEnergy && healthBarAndEnergy->getChildByTag(101)) {
            healthBarAndEnergy->removeChildByTag(101);
        }
        if (healthBarAndEnergy && healthBarAndEnergy->getChildByTag(102)) {
            healthBarAndEnergy->removeChildByTag(102);
        }
        if (healthBarAndEnergy && healthBarAndEnergy->getChildByTag(103)) {
            healthBarAndEnergy->removeChildByTag(103);
        }
        if (healthBarAndEnergy && healthBarAndEnergy->getChildByTag(104)) {
            healthBarAndEnergy->removeChildByTag(104);
        }
        

        healthProgress = currentHealth / maxHealth;
        energyProgress = currentMana / maxMana;

        healthBarAndEnergy = Node::create();
        healthBarAndEnergy->setPosition(Vec2(0, 8));
        overlayHealthBar = Sprite::create("res/ab2_bonus_frame_globe_overlay_effects.png");
        overlayHealthBar->setTag(104);
        healthBarAndEnergy->addChild(overlayHealthBar, 3);
        this->addChild(healthBarAndEnergy, 5);

        progressBarBorder = Sprite::create("res/ab2_bonus_frame_globe_borders.png");
        progressBarBorder->setPosition(Vec2(0, 0));
        progressBarBorder->setTag(101);
        healthBarAndEnergy->addChild(progressBarBorder, 2);


        healthBar = Sprite::create("res/ab2_bonus_frame_globe_fill_red.png");
        healthBar->setPosition(Vec2(-(healthBar->getContentSize().width / 2), 0));
        healthBar->setTag(102);
        healthBarAndEnergy->addChild(healthBar, 1);
        energyBar = Sprite::create("res/ab2_bonus_frame_globe_fill_blue.png");
        energyBar->setPosition(Vec2(healthBar->getContentSize().width - (healthBar->getContentSize().width / 2), 0));
        energyBar->setTag(103);
        healthBarAndEnergy->addChild(energyBar, 1);
        healthBarSize = healthBar->getContentSize();
        energyBarSize = energyBar->getContentSize();
        healthBarHeight = healthBar->getContentSize().height;
        energyBarHeight = energyBar->getContentSize().height;
        healthOffsetY = (healthBarHeight - (healthBarHeight * healthProgress)) / 2;
        energyOffsetY = (energyBarHeight - (energyBarHeight * energyProgress)) / 2;
        healthBar->setPosition(healthBar->getPositionX(), healthBar->getPositionY() - healthOffsetY);
        energyBar->setPosition(energyBar->getPositionX(), energyBar->getPositionY() - energyOffsetY);
        healthBar->setTextureRect(Rect(0, healthBarSize.height * (1 - healthProgress), healthBarSize.width, healthBarSize.height * healthProgress));
        energyBar->setTextureRect(Rect(0, energyBarSize.height * (1 - energyProgress), energyBarSize.width, energyBarSize.height * energyProgress));
    }
    float getMaxHealth() const { return maxHealth; };
private:
    SubInventory* subInventory;
    Size healthBarSize;
    Size energyBarSize;
    float healthBarHeight;
    float energyBarHeight;
    float healthOffsetY;
    float energyOffsetY;
    Sprite* progressBarBorder;
    Sprite* healthBar;
    Sprite* energyBar;
    Sprite* bottomInventory;
    float healthProgress;
    float energyProgress;
    Label* maxHealthLabel;
    Label* healthLabel;
    Label* manaLabel;
    float currentHealth;
    float maxHealth;
    float currentMana;
    float maxMana;
    Node* healthBarAndEnergy;
    Sprite* overlayHealthBar;


};

#endif // __HEALTH_BAR__