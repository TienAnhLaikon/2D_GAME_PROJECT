#ifndef __FIRE_CAST_A_H__
#define __FIRE_CAST_A_H__

#include "cocos2d.h"
#include "Fire.h"
USING_NS_CC;

class FireCastA : public Fire
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

    CREATE_FUNC(FireCastA);
private:
    bool isActive = false;
    float activeTime = 0;
    Sprite* skillEffectiveIndicate;
    Label* timeEffectLabel;

};

#endif 
