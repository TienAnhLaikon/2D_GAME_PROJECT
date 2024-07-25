#ifndef __FIRE_SCORCH_H__
#define __FIRE_SCORCH_H__

#include "cocos2d.h"
#include "Fire.h"
USING_NS_CC;

class FireScorch : public Fire
{
public:
    //Init
    bool init();

    bool onTouchBegan(Touch* touch, Event* event);

    void onTouchMoved(Touch* touch, Event* event);

    void onTouchEnded(Touch* touch, Event* event);

    void performSkill(Vec2 target);

    void update(float dt);

    void updateEnemies(float dt);

    void updateEffect(float dt);


    CREATE_FUNC(FireScorch);
};

#endif 
