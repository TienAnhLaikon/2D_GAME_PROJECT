#ifndef __SOUL_SLAYER_SWORD_H__
#define __SOUL_SLAYER_SWORD_H__

#include "cocos2d.h"
#include "../BaseEquipment.h"
USING_NS_CC;

class SoulSlayerSword : public BaseEquipment
{
public:
    // static ChippedSword *createChippedSword();
    virtual bool init();
    CREATE_FUNC(SoulSlayerSword);
};

#endif // __SOUL_SLAYER_SWORD_H__