#ifndef __SOUL_HELMET__
#define __SOUL_HELMET__

#include "cocos2d.h"
#include "../BaseEquipment.h"
USING_NS_CC;

class SoulHelmet : public BaseEquipment
{
public:
    // static ChippedSword *createChippedSword();
    virtual bool init();
    CREATE_FUNC(SoulHelmet);
};

#endif // __SOUL_HELMET__