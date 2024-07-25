#ifndef __CHIPPED_SHOE_H__
#define __CHIPPED_SHOE_H__

#include "cocos2d.h"
#include "../BaseEquipment.h"
USING_NS_CC;

class ChippedShoe : public BaseEquipment
{
public:
    //static ChippedShoe* createChippedShoe();
    virtual bool init();
    CREATE_FUNC(ChippedShoe);
};

#endif // __CHIPPED_SHOE_H__