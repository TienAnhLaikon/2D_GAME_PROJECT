#ifndef __ITEM_FACTORY_H__
#define __ITEM_FACTORY_H__

#include "cocos2d.h"
#include "BaseItem.h"
#include "UpgradeEquipment/Crystal.h"
#include "UpgradeEquipment/Diamond.h"
#include "UpgradeEquipment/Emerald.h"
#include "UpgradeEquipment/Topaz.h"
#include "Potion/HealthPotion.h"
#include "Potion/ManaPotion.h"
#include "Potion/SpeedPotion.h"
#include "Potion/ScrollPaper.h"
#include "Comon/Gold.h"

class ItemFactory
{
public:
    static BaseItem* createItem(std::string);
};

#endif // __ITEM_FACTORY_H__