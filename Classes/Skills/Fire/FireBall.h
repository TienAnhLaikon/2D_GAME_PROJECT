#ifndef __FIRE_BALL_H__
#define __FIRE_BALL_H__

#include "cocos2d.h"
#include "Fire.h"
USING_NS_CC;

class NormalMonster;
class Boss;

class FireBall : public Fire
{
public:

    //Init
    bool init();

    bool onTouchBegan(Touch* touch, Event* event);

    void onTouchMoved(Touch* touch, Event* event);

    void onTouchEnded(Touch* touch, Event* event);

    void performSkill(Vec2 target);

    void update(float dt);

    void updateCooldown(float dt);

    void updateEffect(float dt);

    //bool onContactBegin(PhysicsContact& contact);

    CREATE_FUNC(FireBall);
private:
    Animate* _flyingAnimate;
    Animate* _explodeAnimate;
};

#endif 
