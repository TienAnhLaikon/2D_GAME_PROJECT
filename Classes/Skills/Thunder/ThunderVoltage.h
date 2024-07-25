#ifndef __THUNDER_VOLTAGE_H__
#define __THUNDER_VOLTAGE_H__

#include "cocos2d.h"
#include "Thunder.h"
USING_NS_CC;

class ThunderVoltage : public Thunder
{
public:
    //Init
    bool init();

    bool onTouchBegan(Touch* touch, Event* event);

    void onTouchMoved(Touch* touch, Event* event);

    void onTouchEnded(Touch* touch, Event* event);

    void performSkill(Vec2 target);

    bool onContactBegin(PhysicsContact& contact);

    void update(float dt);

    void updateEffect(float dt);

    CREATE_FUNC(ThunderVoltage);
private:
    Animate* _flyingAnimate;
    Animate* _explodeAnimate;
    bool isExplode;
    bool isFlying;
};

#endif 
