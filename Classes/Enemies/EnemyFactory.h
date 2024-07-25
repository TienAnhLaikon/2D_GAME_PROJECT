#ifndef __ENEMY_FACTORY_H__
#define __ENEMY_FACTORY_H__

#include "cocos2d.h"
#include "Enemies/Enemy.h"

#include "Enemies/NormalMonster/Skeleton.h"
#include "Enemies/NormalMonster/Slime.h"
#include "Enemies/NormalMonster/BabySpider.h"
#include "Enemies/NormalMonster/Angelos.h"
#include "Enemies/NormalMonster/ShadowSentinels.h"

#include "Enemies/Boss/HellBeast.h"
#include "Enemies/Boss/Okyanus.h"
#include "Enemies/Boss/Paradiso.h"

USING_NS_CC;

class EnemyFactory
{
public:
    static Enemy *createEnemy(std::string enemyType);
};

#endif
