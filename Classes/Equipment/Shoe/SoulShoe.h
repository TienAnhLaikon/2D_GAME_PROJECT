#ifndef __SOUL_SHOE_H__
#define __SOUL_SHOE_H__

#include "cocos2d.h"
#include "../BaseEquipment.h"
USING_NS_CC;

class SoulShoe : public BaseEquipment
{
public:
    //static ChippedShoe* createChippedShoe();
    virtual bool init();
    CREATE_FUNC(SoulShoe);
};

#endif // __SOUL_SHOE_H__