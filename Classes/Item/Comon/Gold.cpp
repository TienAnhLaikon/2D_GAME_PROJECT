#include "./Gold.h"

bool Gold::init()
{
    if (!BaseItem::init())
    {
        return false;
    }
    itemName = "Gold";
    type = BaseItem::Type::COMON;
    itemLevel = BaseItem::ItemLevel::STANDARD;
    itemSprite = Sprite::create("res/Gold.png");
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
    return true;
}