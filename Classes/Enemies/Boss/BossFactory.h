#ifndef __BOSS_FACTORY_H__
#define __BOSS_FACTORY_H__

#include "cocos2d.h"
#include "Enemies/Enemy.h"

#include "Enemies/Boss/HellBeast.h"
#include "Enemies/Boss/Okyanus.h"
#include "Enemies/Boss/Paradiso.h"

USING_NS_CC;

class BossFactory
{
public:
    static Boss* createBoss(std::string BossType);
};

#endif 
