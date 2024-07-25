#include "./SpeedPotion.h"
#include "Player/Player.h"
#include "Game/Game.h"
bool SpeedPotion::init()
{
    if (!BaseItem::init())
    {
        return false;
    }
    itemName = "Speed Potion";
    type = BaseItem::Type::POTION_ITEM;
    itemLevel = BaseItem::ItemLevel::RARE;
    effectTime = 3.0f;
    cooldown = 30; //hp 5.6 mp 7.9
    currentCooldown = 0;
    increaseAmount = 80;    
    des = "A vibrant green potion that\nboosts movement speed.\n(Increases movement speed\nfor a short duration)";
    itemSprite = Sprite::create("sprites/potion/a_speed_potion.png");
    this->addChild(itemSprite, 2);
    lootIndicator = Sprite::create("res/loot_indicator_yellow.png");
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
    timeCooldown = Label::createWithTTF(std::to_string(currentCooldown), "fonts/Diablo Light.ttf", 12);
    timeCooldown->setPosition(itemSprite->getPosition());
    timeCooldown->setVisible(false);
    this->addChild(timeCooldown);

    return true;
}

void SpeedPotion::use() {
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
void SpeedPotion::updateCooldown(float dt) {
    if (isActive) {
        currentCooldown--;
        timeCooldown->setString(std::to_string(currentCooldown));
    }
    if (currentCooldown < 0 && timeCooldown->isVisible() == true) {
        isActive = false;
        timeCooldown->setVisible(false); 
    }
}
