#ifndef __CHIPPED_HELMET_H__
#define __CHIPPED_HELMET_H__

#include "cocos2d.h"
#include "../BaseEquipment.h"
USING_NS_CC;

class ChippedHelmet : public BaseEquipment
{
public:
    // static ChippedSword *createChippedSword();
    virtual bool init();
    CREATE_FUNC(ChippedHelmet);
};

#endif // __CHIPPED_GLOVES_H__