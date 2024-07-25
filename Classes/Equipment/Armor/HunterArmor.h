#ifndef __HUNTER_ARMOR_H__
#define __CHUNTER_ARMOR_H__

#include "cocos2d.h"
#include "../BaseEquipment.h"
USING_NS_CC;

class HunterArmor : public BaseEquipment
{
public:
    // static ChippedSword *createChippedSword();
    virtual bool init();
    CREATE_FUNC(HunterArmor);
};

#endif // __HUNTER_ARMOR_H__