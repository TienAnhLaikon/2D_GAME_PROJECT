#ifndef __CHIPPED_SHIELD_H__
#define __CHIPPED_SHIELD_H__

#include "cocos2d.h"
#include "../BaseEquipment.h"
USING_NS_CC;

class ChippedShield : public BaseEquipment
{
public:
    // static ChippedSword *createChippedSword();
    virtual bool init();
    CREATE_FUNC(ChippedShield);
};

#endif // __CHIPPED_GLOVES_H__