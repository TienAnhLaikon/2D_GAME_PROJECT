#ifndef __HUNTER_RING_H__
#define __HUNTER_RING_H__

#include "cocos2d.h"
#include "../BaseEquipment.h"
USING_NS_CC;

class HunterRing : public BaseEquipment
{
public:
    // static ChippedSword *createChippedSword();
    virtual bool init();
    CREATE_FUNC(HunterRing);
};

#endif // __HUNTER_RING_H__