#include "./SoulShoe.h"

bool SoulShoe::init()
{
    if (!BaseEquipment::init())
    {
        return false;
    }
    // this->setEquipmentSprite(Sprite::create("sprites/equipment/Sword 2.png"));
    // this->getEquipmentSprite()->retain();
    this->setTexture("sprites/equipment/a_shoe_orange.png");
    this->setMovementSpeed(15);
    this->setEquipmentName("Soul Shoe");
    this->setTypeName("Shoe");
    this->setEquipmentType(BaseEquipment::Type::LEGENDARY);
    this->setEquipmentDesc("These legendary shoes are imbued\nwith soul energy, offering unparalleled\nspeed and agility. They are a perfect\nfit for the most elite warriors.");
    this->setEquipmentStory("this is story");
    lootIndicator = Sprite::create("res/loot_indicator_yellow.png");
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