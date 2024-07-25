#ifndef __CHIPPED_RING_H__
#define __CHIPPED_RING_H__

#include "cocos2d.h"
#include "../BaseEquipment.h"
USING_NS_CC;

class ChippedRing : public BaseEquipment
{
public:
    // static ChippedSword *createChippedSword();
    virtual bool init();
    CREATE_FUNC(ChippedRing);
};

#endif // __CHIPPED_RING_H__