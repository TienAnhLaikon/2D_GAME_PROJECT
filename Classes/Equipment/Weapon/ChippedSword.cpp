#include "./ChippedSword.h"

// ChippedSword *ChippedSword::createChippedSword()
// {
//     auto chippedSword = new (std::nothrow) ChippedSword();
//     if (chippedSword && chippedSword->init())
//     {
//         chippedSword->autorelease();
//         return chippedSword;
//     }
//     else
//     {
//         delete chippedSword;
//         return nullptr;
//     }
// }

bool ChippedSword::init()
{
    if (!BaseEquipment::init())
    {
        return false;
    }
    // this->setEquipmentSprite(Sprite::create("sprites/equipment/Sword 2.png"));
    // this->getEquipmentSprite()->retain();
    this->setTexture("sprites/equipment/a_sword_white.png");
    this->setDamage(20);
    this->setSkillDamage(5);
    this->setEquipmentName("Chipped Sword");
    this->setTypeName("Weapon");
    this->setEquipmentType(BaseEquipment::Type::STANDARD);

    this->setEquipmentDesc("A basic sword with visible chips\nand wear. It offers standard\nattack capabilities but is far\nfrom optimal for serious combat");
    this->setEquipmentStory("this is story");
    this->setElement(BaseEquipment::Element::NONE);
    lootIndicator = Sprite::create("res/loot_indicator_white.png");
    this->addChild(lootIndicator, 1);
    lootIndicator->setVisible(false);

    // this->addChild(this->getEquipmentSprite());
    // this->equipmentDetails->hide();
    // equipmentDetails = WeaponDetails::create(this->getTexture()->getPath(), this->getEquipmentName(), this->getEquipmentDesc(), "", "", this->getTypeName(), this->getLevel(), false, false);
    // this->addChild(equipmentDetails);
    // equipmentDetails->hide();
    // EquipmentDetails::create(path, name, "dung rat oke\ndung rat oke\ndung rat oke", "12", "- damage: 20\n- armor: 10", equipment->getTypeName(), equipment->getLevel(), isPlayerUsing, false);



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

void ChippedSword::showLootIndicator() {
    lootIndicator->setVisible(true);
}