#ifndef __THUNDER_CAST_B_H__
#define __THUNDER_CAST_B_H__

#include "cocos2d.h"
#include "Thunder.h"
USING_NS_CC;

class ThunderCastB : public Thunder
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

    CREATE_FUNC(ThunderCastB);
private:
    bool isActive = false;
    float activeTime = 0;
    Sprite* skillEffectiveIndicate;
    Label* timeEffectLabel;
};

#endif 
