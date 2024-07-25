#ifndef __ARMOR_OF_KNIGHT_H__
#define __ARMOR_OF_KNIGHT_H__

#include "cocos2d.h"
#include "../BaseEquipment.h"
USING_NS_CC;

class ArmorOfKnight : public BaseEquipment
{
public:
    // static ChippedSword *createChippedSword();
    virtual bool init();
    CREATE_FUNC(ArmorOfKnight);
};

#endif // __ARMOR_OF_KNIGHT_H__