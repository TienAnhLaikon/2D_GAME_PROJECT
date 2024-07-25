#ifndef __THUNDER_CURRENT_H__
#define __THUNDER_CURRENT_H__

#include "cocos2d.h"
#include "Thunder.h"
USING_NS_CC;

class ThunderCurrent : public Thunder
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

    CREATE_FUNC(ThunderCurrent);

private:
    bool isActive = false;
    float activeTime = 0;
};

#endif 
