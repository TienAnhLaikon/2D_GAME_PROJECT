#include "./LongSword.h"

bool LongSword::init()
{
    if (!BaseEquipment::init())
    {
        return false;
    }
    // this->setEquipmentSprite(Sprite::create("sprites/equipment/Sword 2.png"));
    // this->getEquipmentSprite()->retain();
    this->setTexture("sprites/equipment/a_sword_purple3.png");
    this->setDamage(30);
    this->setSkillDamage(20);
    this->setEquipmentName("Long Sword");
    this->setTypeName("Weapon");
    this->setEquipmentType(BaseEquipment::Type::ENHANCED);
    this->setElement(BaseEquipment::Element::THUNDER);
    this->setEquipmentDesc("A rare long sword known for\nits balance and reach. It is\na versatile weapon, suitable\nfor both offense and defense.");
    this->setEquipmentStory("this is story");
    lootIndicator = Sprite::create("res/loot_indicator_green.png");
    this->addChild(lootIndicator, 1);
    lootIndicator->setVisible(false);
    // this->addChild(this->getEquipmentSprite());
    // equipmentDetails = EquipmentDetails::create("sprites/equipment/Sword 3.png", "Long Sword", "this is sword", "", "", "Weapon", 1, false, false);
    // this->addChild(equipmentDetails);
    // equipmentDetails->hide();


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