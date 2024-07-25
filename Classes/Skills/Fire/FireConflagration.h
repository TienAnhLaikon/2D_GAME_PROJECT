#ifndef __FIRE_CONFLAGRATION_H__
#define __FIRE_CONFLAGRATION_H__

#include "cocos2d.h"
#include "Fire.h"
USING_NS_CC;

class FireConflagration : public Fire
{
public:
    //Init
    bool init();

    bool onTouchBegan(Touch* touch, Event* event);

    void onTouchMoved(Touch* touch, Event* event);

    void onTouchEnded(Touch* touch, Event* event);

    void performSkill(Vec2 target); 

    void updateCooldown(float dt);

    void updateEffect(float dt);

    CREATE_FUNC(FireConflagration);

};

#endif 
