#include "./HealthPotion.h"
#include "Game/Game.h"
bool HealthPotion::init()
{
    if (!BaseItem::init())
    {
        return false;
    }
    itemName = "Health Potion";
    type = BaseItem::Type::POTION_ITEM;
    itemLevel = BaseItem::ItemLevel::ENHANCED;
    des = "A bright red potion that\nsignificantly boosts health\nregeneration.\n (Increases HP regen for a\nshort duration)";
    effectTime = 8.0f;
    cooldown = 13;
    increaseAmount = 6;
    currentCooldown = 0;
    itemSprite = Sprite::create("sprites/potion/a_health_potion.png");
    this->addChild(itemSprite, 2);

    lootIndicator = Sprite::create("res/loot_indicator_green.png");
    this->addChild(lootIndicator, 1);
    lootIndicator->setVisible(false);

    titleIndicator = Sprite::create("res/ab2_main_frame2.png");
    itemNameIndicator = Label::createWithTTF(itemName, "fonts/Diablo Light.ttf", 15);
    itemNameIndicator->setScale(0.18);
    titleIndicator->setPosition(Vec2(0, itemNameIndicator->getContentSize().height * itemNameIndicator->getScaleY()));
    if (itemLevel == BaseItem::ItemLevel::ENHANCED) {
        itemNameIndicator->setTextColor(Color4B::GREEN);
    }
    else if (itemLevel == BaseItem::ItemLevel::RARE) {
        itemNameIndicator->setTextColor(Color4B::MAGENTA);
    }
    else {
        itemNameIndicator->setTextColor(Color4B::WHITE);
    }
    itemNameIndicator->setPosition(titleIndicator->getPosition());
    titleIndicator->setScale(itemNameIndicator->getContentSize().width * itemNameIndicator->getScaleX() / titleIndicator->getContentSize().width, itemNameIndicator->getContentSize().height * itemNameIndicator->getScaleY() / titleIndicator->getContentSize().height);
    this->addChild(titleIndicator, 3);
    this->addChild(itemNameIndicator, 5);
    titleIndicator->setVisible(false);
    itemNameIndicator->setVisible(false);

    Scene* currentScene = Director::getInstance()->getRunningScene();
    game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
    this->setUser(game->getPlayer());
    timeCooldown = Label::createWithTTF(std::to_string(currentCooldown), "fonts/Diablo Light.ttf", 20);
    timeCooldown->setPosition(itemSprite->getPosition());
    timeCooldown->setVisible(true);
    return true;
}

void HealthPotion::use() {
    if (_user) {
        if (currentCooldown <= 0) {
            _user->usePotion(this->itemName);
            currentCooldown = cooldown;
            isActive = true;
            this->scheduleOnce(CC_SCHEDULE_SELECTOR(SpeedPotion::updateCooldown), 1.0f); // C?p nh?t sau m?i giy
        }
        else {
            timeCooldown->setVisible(true);
        }
    }
}

void HealthPotion::updateCooldown(float dt) {
    if (isActive) {
        currentCooldown--;
        timeCooldown->setString(std::to_string(currentCooldown));
    }
    if (currentCooldown < 0 && timeCooldown->isVisible() == true) {
        isActive = false;
        timeCooldown->setVisible(false);
    }
}