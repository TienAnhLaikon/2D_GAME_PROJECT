#ifndef __LONG_SWORD_H__
#define __LONG_SWORD_H__

#include "cocos2d.h"
#include "../BaseEquipment.h"
USING_NS_CC;

class LongSword : public BaseEquipment
{
public:
    // static ChippedSword *createChippedSword();
    virtual bool init();
    CREATE_FUNC(LongSword);
};

#endif // __LONG_SWORD_H__