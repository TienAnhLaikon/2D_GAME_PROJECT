#ifndef __SNEAKER_H__
#define __SNEAKER_H__

#include "cocos2d.h"
#include "../BaseEquipment.h"
USING_NS_CC;

class Sneaker : public BaseEquipment
{
public:
    // static ChippedShoe* createChippedShoe();
    virtual bool init();
    CREATE_FUNC(Sneaker);
};

#endif // __CHIPPED_SHOE_H__