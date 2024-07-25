#ifndef __FIRE_ERUPTION_H__
#define __FIRE_ERUPTION_H__

#include "cocos2d.h"
#include "Fire.h"
USING_NS_CC;

class FireEruption : public Fire
{
public:
    //Init
    bool init();

    bool onTouchBegan(Touch* touch, Event* event);

    void onTouchMoved(Touch* touch, Event* event);

    void onTouchEnded(Touch* touch, Event* event);

    void performSkill(Vec2 target);

    void update(float dt);

    CREATE_FUNC(FireEruption);
private:
    Animate* _flyingAnimate;
    Animate* _explodeAnimate;
};

#endif 
