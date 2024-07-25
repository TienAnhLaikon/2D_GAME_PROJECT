#ifndef __SOUL_RING_H__
#define __SOUL_RING_H__

#include "cocos2d.h"
#include "../BaseEquipment.h"
USING_NS_CC;

class SoulRing : public BaseEquipment
{
public:
    // static ChippedSword *createChippedSword();
    virtual bool init();
    CREATE_FUNC(SoulRing);
};

#endif // __SOUL_RING_H__