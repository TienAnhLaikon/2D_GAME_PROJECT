#include "ItemFactory.h"

BaseItem* ItemFactory::createItem(std::string type)
{
    if (type == "crystal") {
        return Crystal::create();
    }
    else if (type == "diamond") {
        return Diamond::create();
    }
    else if (type == "emerald") {
        return Emerald::create();
    }
    else if (type == "topaz") {
        return Topaz::create();
    }
    else if (type == "Health Potion") {
        return HealthPotion::create();
    }
    else if (type == "Mana Potion") {
        return ManaPotion::create();
    }
    else if (type == "Speed Potion") {
        return SpeedPotion::create();
    }
    else if (type == "Gold") {
        return Gold::create();
    }
    else if (type == "Scroll Paper") {
        return ScrollPaper::create();
    }
    return nullptr;
}