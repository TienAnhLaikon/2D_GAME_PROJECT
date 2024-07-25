#ifndef __CHIPPED_SWORD_H__
#define __CHIPPED_SWORD_H__

#include "cocos2d.h"
#include "../BaseEquipment.h"
USING_NS_CC;

class ChippedSword : public BaseEquipment
{
public:
    // static ChippedSword *createChippedSword();
    virtual bool init();
    void showLootIndicator();
    CREATE_FUNC(ChippedSword);
};

#endif // __CHIPPED_SWORD_H__