#ifndef __THUNDER_BOLT_H__
#define __THUNDER_BOLT_H__

#include "cocos2d.h"
#include "Thunder.h"
USING_NS_CC;

class ThunderBolt : public Thunder
{
public:
    //Init
    bool init();

    bool onTouchBegan(Touch* touch, Event* event);

    void onTouchMoved(Touch* touch, Event* event);

    void onTouchEnded(Touch* touch, Event* event);

    void performSkill(Vec2 target);

    void update(float dt);

    void updateEffect(float dt);

    void updateEnemies(float dt);

    CREATE_FUNC(ThunderBolt);
};

#endif 
