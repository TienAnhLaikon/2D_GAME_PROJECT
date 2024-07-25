#include "./HunterArmor.h"

bool HunterArmor::init()
{
    if (!BaseEquipment::init())
    {
        return false;
    }
    // this->setEquipmentSprite(Sprite::create("sprites/equipment/Sword 2.png"));
    // this->getEquipmentSprite()->retain();
    this->setTexture("sprites/equipment/a_armor_purple.png");
    this->setArmor(40);
    this->setEquipmentName("Hunter Armor");
    this->setTypeName("Armor");
    this->setEquipmentType(BaseEquipment::Type::ENHANCED);
    this->setEquipmentDesc("This armor is designed for\nhunters, providing excellent\nmobility and protection. Its rare\ncraftsmanship ensures durability\n and a sleek appearance");
    this->setEquipmentStory("this is story");
    lootIndicator = Sprite::create("res/loot_indicator_green.png");
    this->addChild(lootIndicator, 1);
    lootIndicator->setVisible(false);
    // this->addChild(this->getEquipmentSprite());
    this->retain();

    titleIndicator = Sprite::create("res/ab2_main_frame2.png");
    itemNameIndicator = Label::createWithTTF(this->getEquipmentName(), "fonts/Diablo Light.ttf", 15);
    titleIndicator->setPosition(Vec2(0, itemNameIndicator->getContentSize().height * itemNameIndicator->getScaleY()));
    if (this->getEquipmentType() == BaseEquipment::Type::ENHANCED) {
        itemNameIndicator->setTextColor(Color4B::GREEN);
    }
    else if (this->getEquipmentType() == BaseEquipment::Type::RARE) {
        itemNameIndicator->setTextColor(Color4B::MAGENTA);
    }
    else if (this->getEquipmentType() == BaseEquipment::Type::LEGENDARY) {
        itemNameIndicator->setTextColor(Color4B::RED);
    }
    else {
        itemNameIndicator->setTextColor(Color4B::WHITE);
    }
    itemNameIndicator->setPosition(titleIndicator->getPosition());
    titleIndicator->setScale(itemNameIndicator->getContentSize().width / titleIndicator->getContentSize().width, itemNameIndicator->getContentSize().height / titleIndicator->getContentSize().height);
    this->addChild(titleIndicator, 3);
    this->addChild(itemNameIndicator, 5);
    titleIndicator->setVisible(false);
    itemNameIndicator->setVisible(false);
    return true;
}