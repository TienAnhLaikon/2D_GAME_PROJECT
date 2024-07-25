#ifndef __SOUL_ARMOR_H__
#define __SOUL_ARMOR_H__

#include "cocos2d.h"
#include "../BaseEquipment.h"
USING_NS_CC;

class SoulArmor : public BaseEquipment
{
public:
    // static ChippedSword *createChippedSword();
    virtual bool init();
    CREATE_FUNC(SoulArmor);
};

#endif // __SOUL_ARMOR_H__