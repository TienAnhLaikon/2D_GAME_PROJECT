#include "BaseEquipment.h"
#include "Weapon/ChippedSword.h"
#include "Weapon/LongSword.h"
#include "Weapon/HunterSword.h"
#include "Weapon/SwordOfKnight.h"
#include "Weapon/SoulSlayerSword.h"
#include "Shoe/ChippedShoe.h"
#include "Shoe/Sneaker.h"
#include "Shoe/SoulShoe.h"
#include "Helmet/ChippedHelmet.h"
#include "Helmet/HelmetOfKnight.h"
#include "Helmet/SoulHelmet.h"
#include "Armor/ChippedArmor.h"
#include "Armor/ArmorOfKnight.h"
#include "Armor/HunterArmor.h"
#include "Armor/SoulArmor.h"
#include "Ring/ChippedRing.h"
#include "Ring/HunterRing.h"
#include "Ring/SoulRing.h"
#include "Shield/ChippedShield.h"
#include "Shield/KnightShield.h"
#include "Shield/SoulShield.h"

// BaseEquipment *BaseEquipment::createEquipment()
// {
//     auto equipment = new (std::nothrow) BaseEquipment();
//     if (equipment && equipment->init())
//     {
//         equipment->autorelease();
//         return equipment;
//     }
//     else
//     {
//         delete equipment;
//         return nullptr;
//     }
// }

bool BaseEquipment::init()
{
    if (!Sprite::init())
    {
        return false;
    }

    if (dynamic_cast<ChippedSword*>(this) == this || dynamic_cast<ChippedArmor*>(this) == this || dynamic_cast<ChippedHelmet*>(this) == this || dynamic_cast<ChippedShoe*>(this) == this || dynamic_cast<ChippedShield*>(this) == this || dynamic_cast<ChippedRing*>(this) == this) {
        LevelUpMaterials level1;
        level1.materialsItem.push_back(std::make_tuple("crystal", 1));
        level1.money = 2;
        LevelUpMaterials level2;
        level2.materialsItem.push_back(std::make_tuple("crystal", 3));
        level2.money = 3;
        LevelUpMaterials level3;
        level3.materialsItem.push_back(std::make_tuple("crystal", 5));
        level3.money = 5;
        LevelUpMaterials level4;
        level4.materialsItem.push_back(std::make_tuple("crystal", 6));
        level4.money = 6;
        LevelUpMaterials level5;
        level5.materialsItem.push_back(std::make_tuple("crystal", 8));
        level5.money = 8;
        materialsToUpgrade.push_back(level1);
        materialsToUpgrade.push_back(level2);
        materialsToUpgrade.push_back(level3);
        materialsToUpgrade.push_back(level4);
        materialsToUpgrade.push_back(level5);
    }
    else if (dynamic_cast<LongSword*>(this) == this || dynamic_cast<SwordOfKnight*>(this) == this || dynamic_cast<KnightShield*>(this) == this || dynamic_cast<Sneaker*>(this) == this || dynamic_cast<HelmetOfKnight*>(this) == this || dynamic_cast<ArmorOfKnight*>(this) == this) {
        LevelUpMaterials level1;
        level1.materialsItem.push_back(std::make_tuple("emerald", 1));
        level1.money = 2;
        LevelUpMaterials level2;
        level2.materialsItem.push_back(std::make_tuple("emerald", 2));
        level2.money = 3;
        LevelUpMaterials level3;
        level3.materialsItem.push_back(std::make_tuple("emerald", 2));
        level3.money = 5;
        LevelUpMaterials level4;
        level4.materialsItem.push_back(std::make_tuple("emerald", 3));
        level4.money = 6;
        LevelUpMaterials level5;
        level5.materialsItem.push_back(std::make_tuple("emerald", 5));
        level5.money = 8;
        materialsToUpgrade.push_back(level1);
        materialsToUpgrade.push_back(level2);
        materialsToUpgrade.push_back(level3);
        materialsToUpgrade.push_back(level4);
        materialsToUpgrade.push_back(level5);
    }
    else if (dynamic_cast<HunterSword*>(this) == this || dynamic_cast<HunterArmor*>(this) == this || dynamic_cast<HunterRing*>(this) == this) {
        LevelUpMaterials level1;
        level1.materialsItem.push_back(std::make_tuple("topaz", 2));
        level1.money = 2;
        LevelUpMaterials level2;
        level2.materialsItem.push_back(std::make_tuple("topaz", 3));
        level2.money = 3;
        LevelUpMaterials level3;
        level3.materialsItem.push_back(std::make_tuple("topaz", 5));
        level3.money = 5;
        LevelUpMaterials level4;
        level4.materialsItem.push_back(std::make_tuple("topaz", 6));
        level4.money = 6;
        LevelUpMaterials level5;
        level5.materialsItem.push_back(std::make_tuple("topaz", 8));
        level5.money = 8;
        materialsToUpgrade.push_back(level1);
        materialsToUpgrade.push_back(level2);
        materialsToUpgrade.push_back(level3);
        materialsToUpgrade.push_back(level4);
        materialsToUpgrade.push_back(level5);
    }
    else if (dynamic_cast<SoulArmor*>(this) == this || dynamic_cast<SoulHelmet*>(this) == this || dynamic_cast<SoulRing*>(this) == this || dynamic_cast<SoulShield*>(this) == this || dynamic_cast<SoulShoe*>(this) == this || dynamic_cast<SoulSlayerSword*>(this) == this) {
        LevelUpMaterials level1;
        level1.materialsItem.push_back(std::make_tuple("diamond", 1));
        level1.money = 2;
        LevelUpMaterials level2;
        level2.materialsItem.push_back(std::make_tuple("diamond", 2));
        level2.money = 3;
        LevelUpMaterials level3;
        level3.materialsItem.push_back(std::make_tuple("diamond", 3));
        level3.money = 5;
        LevelUpMaterials level4;
        level4.materialsItem.push_back(std::make_tuple("diamond", 5));
        level4.money = 6;
        LevelUpMaterials level5;
        level5.materialsItem.push_back(std::make_tuple("diamond", 8));
        level5.money = 8;
        materialsToUpgrade.push_back(level1);
        materialsToUpgrade.push_back(level2);
        materialsToUpgrade.push_back(level3);
        materialsToUpgrade.push_back(level4);
        materialsToUpgrade.push_back(level5);
    }
    else {
        LevelUpMaterials level1;
        level1.materialsItem.push_back(std::make_tuple("crystal", 1));
        level1.money = 1;
        LevelUpMaterials level2;
        level2.materialsItem.push_back(std::make_tuple("crystal", 2));
        level2.money = 2;
        LevelUpMaterials level3;
        level3.materialsItem.push_back(std::make_tuple("crystal", 3));
        level3.money = 3;
        LevelUpMaterials level4;
        level4.materialsItem.push_back(std::make_tuple("crystal", 4));
        level4.money = 3;
        LevelUpMaterials level5;
        level5.materialsItem.push_back(std::make_tuple("crystal", 5));
        level5.money = 3;
        materialsToUpgrade.push_back(level1);
        materialsToUpgrade.push_back(level2);
        materialsToUpgrade.push_back(level3);
        materialsToUpgrade.push_back(level4);
        materialsToUpgrade.push_back(level5);
    }
   


    return true;
}
void BaseEquipment::setElement(Element element) {
    this->element = element;
}

BaseEquipment::Element BaseEquipment::getElement() const {
    return this->element;
}
void BaseEquipment::setEquipmentType(Type lv)
{
    this->type = lv;
}

void BaseEquipment::levelUp()
{
    this->setDamage(this->getDamage() * 1.5);
    this->setArmor(this->getArmor() * 1.5);
    this->setCDR(this->getCDR() * 1.5);
    this->setMovementSpeed(this->getMovementSpeed() * 1.5);
    this->setSkillDamage(this->getSkillDamage() * 1.5);
    this->setMP(this->getMP() * 1.5);
    this->setHP(this->getHP() * 1.5);
    stats.currentLv += 1;
}

void BaseEquipment::setLevel(int lv)
{
    stats.currentLv = lv;
    for (int i = 1; i < stats.currentLv; i++)
    {
        this->setDamage(this->getDamage() * 1.5);
        this->setArmor(this->getArmor() * 1.5);
        this->setCDR(this->getCDR() * 1.5);
        this->setMovementSpeed(this->getMovementSpeed() * 1.5);
        this->setSkillDamage(this->getSkillDamage() * 1.5);
        this->setMP(this->getMP() * 1.5);
        this->setHP(this->getHP() * 1.5);
    }
}

BaseEquipment::Type BaseEquipment::getEquipmentType() const
{
    return this->type;
};

void BaseEquipment::setEquipmentDesc(std::string des)
{
    this->description = des;
}

std::string BaseEquipment::getEquipmentDesc() const
{
    return this->description;
}

void BaseEquipment::setEquipmentName(std::string n)
{
    this->name = n;
}

std::string BaseEquipment::getEquipmentName() const
{
    
    return this->name;
}

void BaseEquipment::setEquipmentStory(std::string str)
{
    this->story = str;
}

std::string BaseEquipment::getEquipmentStory() const
{
    return this->story;
}

void BaseEquipment::setDamage(float damage)
{
    this->stats.ad = damage;
}

float BaseEquipment::getDamage() const
{
    return this->stats.ad;
}

void BaseEquipment::setSkillDamage(float damage)
{
    this->stats.ap = damage;
}

float BaseEquipment::getSkillDamage() const
{
    return this->stats.ap;
}

void BaseEquipment::setMovementSpeed(float speed)
{
    this->stats.ms = speed;
}

float BaseEquipment::getMovementSpeed() const
{
    return this->stats.ms;
}

float BaseEquipment::getMP() const
{
    return this->stats.mp;
}
void BaseEquipment::setMP(float newMp)
{
    this->stats.mp = newMp;
}

void BaseEquipment::setArmor(float arm)
{
    this->stats.arm = arm;
}

float BaseEquipment::getArmor() const
{
    return this->stats.arm;
}

// void BaseEquipment::setEquipmentSprite(Sprite *spr)
// {
//     this->equipmentSprite = spr;
// }
// Sprite *BaseEquipment::getEquipmentSprite() const
// {
//     return this->equipmentSprite;
// }

BaseEquipment::~BaseEquipment() {
    this->release();
}