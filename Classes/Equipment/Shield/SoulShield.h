#ifndef __SOUL_SHIELD_H__
#define __SOUL_SHIELD_H__

#include "cocos2d.h"
#include "../BaseEquipment.h"
USING_NS_CC;

class SoulShield : public BaseEquipment
{
public:
    // static ChippedSword *createChippedSword();
    virtual bool init();
    CREATE_FUNC(SoulShield);
};

#endif // __SOUL_GLOVES_H__