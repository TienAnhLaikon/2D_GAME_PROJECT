#ifndef __CHIPPED_ARMOR_H__
#define __CHIPPED_ARMOR_H__

#include "cocos2d.h"
#include "../BaseEquipment.h"
USING_NS_CC;

class ChippedArmor : public BaseEquipment
{
public:
    // static ChippedSword *createChippedSword();
    virtual bool init();
    CREATE_FUNC(ChippedArmor);
};

#endif // __CHIPPED_ARMOR_H__