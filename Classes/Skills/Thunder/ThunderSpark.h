#ifndef __THUNDER_SPARK_H__
#define __THUNDER_SPARK_H__

#include "cocos2d.h"
#include "Thunder.h"
USING_NS_CC;

class ThunderSpark : public Thunder
{
public:
    //Init
    bool init();

    bool onTouchBegan(Touch* touch, Event* event);

    void onTouchMoved(Touch* touch, Event* event);

    void onTouchEnded(Touch* touch, Event* event);

    void performSkill(Vec2 target);

    void update(float dt);

    CREATE_FUNC(ThunderSpark);
};

#endif 
