#include "./HelmetOfKnight.h"

bool HelmetOfKnight::init()
{
    if (!BaseEquipment::init())
    {
        return false;
    }
    // this->setEquipmentSprite(Sprite::create("sprites/equipment/Sword 2.png"));
    // this->getEquipmentSprite()->retain();
    this->setTexture("sprites/equipment/a_helmet_purple.png");
    this->setArmor(40);
    this->setCDR(8);
    this->setEquipmentName("Helmet Of Knight");
    this->setTypeName("Helmet");
    this->setEquipmentType(BaseEquipment::Type::RARE);
    this->setEquipmentDesc("Crafted for the brave knights, \nthis helmet offers enhanced protection\nand a noble appearance. It symbolizes\ncourage and strength");
    this->setEquipmentStory("this is story");
    // this->addChild(this->getEquipmentSprite());
    lootIndicator = Sprite::create("res/loot_indicator_blue.png");
    this->addChild(lootIndicator, 1);
    lootIndicator->setVisible(false);
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