﻿#ifndef __THUNDER_FULMINATION_A_H__
#define __THUNDER_FULMINATION_A_H__

#include "cocos2d.h"
#include "Thunder.h"
USING_NS_CC;

class ThunderFulminationA : public Thunder
{
public:
    //Init
    bool init();

    bool onTouchBegan(Touch* touch, Event* event);

    void onTouchMoved(Touch* touch, Event* event);

    void onTouchEnded(Touch* touch, Event* event);

    void performSkill(Vec2 target);

    void update(float dt);

    CREATE_FUNC(ThunderFulminationA);
};

#endif 
