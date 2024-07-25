#ifndef __HUNTER_SWORD_H__
#define __HUNTER_SWORD_H__

#include "cocos2d.h"
#include "../BaseEquipment.h"
USING_NS_CC;

class HunterSword : public BaseEquipment
{
public:
    // static ChippedSword *createChippedSword();
    virtual bool init();
    CREATE_FUNC(HunterSword);
};

#endif // __HUNTER_SWORD_H__