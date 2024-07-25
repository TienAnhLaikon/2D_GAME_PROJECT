#include "./ManaPotion.h"
#include "Game/Game.h"
bool ManaPotion::init()
{
    if (!BaseItem::init())
    {
        return false;
    }
    itemName = "Mana Potion";
    type = BaseItem::Type::POTION_ITEM;
    itemLevel = BaseItem::ItemLevel::ENHANCED;
    des = "A bright blue potion that\nsignificantly boosts mana\nregeneration.\n (Increases MP regen for\na short duration)";
    effectTime = 8.0f;
    cooldown = 13;
    increaseAmount = 6;
    itemSprite = Sprite::create("sprites/potion/a_mana_potion.png");
    this->addChild(itemSprite, 2);
    lootIndicator = Sprite::create("res/loot_indicator_green.png");
    this->addChild(lootIndicator, 1);
    lootIndicator->setVisible(false);
    Scene* currentScene = Director::getInstance()->getRunningScene();
    game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
    this->setUser(game->getPlayer());

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
    return true;
}

void ManaPotion::use() {
    if (_user) {
        _user->usePotion(this->itemName);
    }
}