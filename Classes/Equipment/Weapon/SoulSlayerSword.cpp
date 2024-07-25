#include "./SoulSlayerSword.h"

bool SoulSlayerSword::init()
{
    if (!BaseEquipment::init())
    {
        return false;
    }
    // this->setEquipmentSprite(Sprite::create("sprites/equipment/Sword 2.png"));
    // this->getEquipmentSprite()->retain();
    this->setTexture("sprites/equipment/a_sword_orange.png");
    this->setDamage(180);
    this->setSkillDamage(200);
    this->setMP(300);
    this->setEquipmentName("Soul Slayer Sword");
    this->setTypeName("Weapon");
    this->setEquipmentType(BaseEquipment::Type::LEGENDARY);
    this->setElement(BaseEquipment::Element::THUNDER);
    this->setEquipmentDesc("This legendary sword is imbued\nwith the power of souls,\nmaking it incredibly powerful\nand deadly. It is the ultimate weapon\nfor those who seek to vanquish\ntheir foes");
    this->setEquipmentStory("this is soul slayer story");
    lootIndicator = Sprite::create("res/loot_indicator_yellow.png");
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