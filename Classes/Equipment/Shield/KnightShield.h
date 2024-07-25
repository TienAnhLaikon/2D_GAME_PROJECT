#ifndef __KNIGHT_SHIELD_H__
#define __KNIGHT_SHIELD_H__

#include "cocos2d.h"
#include "../BaseEquipment.h"
USING_NS_CC;

class KnightShield : public BaseEquipment
{
public:
    // static ChippedSword *createChippedSword();
    virtual bool init();
    CREATE_FUNC(KnightShield);
};

#endif // __KNIGHT_SHIELD_H__