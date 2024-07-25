#ifndef __SWORD_OF_KNIGHT_H__
#define __SWORD_OF_KNIGHT_H__

#include "cocos2d.h"
#include "../BaseEquipment.h"
USING_NS_CC;

class SwordOfKnight : public BaseEquipment
{
public:
    // static ChippedSword *createChippedSword();
    virtual bool init();
    CREATE_FUNC(SwordOfKnight);
};

#endif // __SWORD_OF_KNIGHT_H__