#ifndef __HELMET_OF_KNIGHT_H__
#define __HELMET_OF_KNIGHT_H__

#include "cocos2d.h"
#include "../BaseEquipment.h"
USING_NS_CC;

class HelmetOfKnight : public BaseEquipment
{
public:
    // static ChippedSword *createChippedSword();
    virtual bool init();
    CREATE_FUNC(HelmetOfKnight);
};

#endif // __HELMET_OF_KNIGHT_H__