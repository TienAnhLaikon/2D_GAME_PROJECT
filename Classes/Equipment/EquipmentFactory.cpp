#include "EquipmentFactory.h"
#include "./BaseEquipment.h"
#include "./Weapon/ChippedSword.h"
#include "./Weapon/LongSword.h"
#include "./Weapon/HunterSword.h"
#include "./Weapon/SoulSlayerSword.h"
#include "./Weapon/SwordOfKnight.h"
#include "./Ring/ChippedRing.h"
#include "./Ring/HunterRing.h"
#include "./Ring/SoulRing.h"
#include "./Shoe/ChippedShoe.h"
#include "./Shoe/Sneaker.h"
#include "./Shoe/SoulShoe.h"
#include "./Helmet/ChippedHelmet.h"
#include "./Helmet/HelmetOfKnight.h"
#include "./Helmet/SoulHelmet.h"
#include "./Shield/ChippedShield.h"
#include "./Shield/KnightShield.h"
#include "./Shield/SoulShield.h"
#include "./Armor/ChippedArmor.h"
#include "./Armor/ArmorOfKnight.h"
#include "./Armor/HunterArmor.h"
#include "./Armor/SoulArmor.h"

BaseEquipment *EquipmentFactory::createEquipment(std::string type, int level)
{
    if (type == "Chipped Sword")
    {
        auto rs = ChippedSword::create();
        rs->setLevel(level); 
        return rs;
    }
    else if (type == "Long Sword")
    {
        auto rs = LongSword::create();
        rs->setLevel(level);
        return rs;
    }
    else if (type == "Hunter Sword")
    {
        auto rs = HunterSword::create();
        rs->setLevel(level);
        return rs;
    }
    else if (type == "Soul Slayer Sword")
    {
        auto rs = SoulSlayerSword::create();
        rs->setLevel(level);
        return rs;
    }
    else if (type == "Sword Of Knight")
    {
        auto rs = SwordOfKnight::create();
        rs->setLevel(level);
        return rs;
    }
    else if (type == "Chipped Ring")
    {
        auto rs = ChippedRing::create();
        rs->setLevel(level);
        return rs;
    }
    else if (type == "Soul Ring")
    {
        auto rs = SoulRing::create();
        rs->setLevel(level);
        return rs;
    }
    else if (type == "Chipped Shoe")
    {
        auto rs = ChippedShoe::create();
        rs->setLevel(level);
        return rs;
    }
    else if (type == "Sneaker")
    {
        auto rs = Sneaker::create();
        rs->setLevel(level);
        return rs;
    }
    else if (type == "Chipped Armor")
    {
        auto rs = ChippedArmor::create();
        rs->setLevel(level);
        return rs;
    }
    else if (type == "Armor Of Knight")
    {
        auto rs = ArmorOfKnight::create();
        rs->setLevel(level);
        return rs;
    }
    else if (type == "Hunter Armor")
    {
        auto rs = HunterArmor::create();
        rs->setLevel(level);
        return rs;
    }
    else if (type == "Soul Armor")
    {
        auto rs = SoulArmor::create();
        rs->setLevel(level);
        return rs;
    }
    else if (type == "Chipped Shield")
    {
        auto rs = ChippedShield::create();
        rs->setLevel(level);
        return rs;
    }
    else if (type == "Soul Shield")
    {
        auto rs = SoulShield::create();
        rs->setLevel(level);
        return rs;
    }
    else if (type == "Chipped Helmet")
    {
        auto rs = ChippedHelmet::create();
        rs->setLevel(level);
        return rs;
    }
    else if (type == "Helmet Of Knight")
    {
        auto rs = HelmetOfKnight::create();
        rs->setLevel(level);
        return rs;
    }
    else if (type == "Hunter Ring")
    {
        auto rs = HunterRing::create();
        rs->setLevel(level);
        return rs;
    }
    else if (type == "Soul Shoe")
    {
        auto rs = SoulShoe::create();
        rs->setLevel(level);
        return rs;
    }
    else if (type == "Knight Shield")
    {
        auto rs = KnightShield::create();
        rs->setLevel(level);
        return rs;
    }
    else if (type == "Soul Helmet")
    {
        auto rs = SoulHelmet::create();
        rs->setLevel(level);
        return rs;
    }
    else if (type == "Soul Shoe")
    {
        auto rs = SoulShoe::create();
        rs->setLevel(level);
        return rs;
    }

    return nullptr;
}