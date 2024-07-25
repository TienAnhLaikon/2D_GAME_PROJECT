#ifndef __FIRE_SEAR_H__
#define __FIRE_SEAR_H__

#include "cocos2d.h"
#include "Fire.h"
USING_NS_CC;

class NormalMonster;
class FireSear : public Fire
{
public:
    //Init
    bool init();

    bool onTouchBegan(Touch* touch, Event* event);

    void onTouchMoved(Touch* touch, Event* event);

    void onTouchEnded(Touch* touch, Event* event);

    void performSkill(Vec2 target);

    CREATE_FUNC(FireSear);

    std::vector<NormalMonster*> monstersRemove;

    void deleteEnemies(float);

    bool isDeleteEnemiesScheduled = false;
};

#endif 
