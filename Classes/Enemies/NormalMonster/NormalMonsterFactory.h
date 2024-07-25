#ifndef __NORMAL_MONSTER_FACTORY_H__
#define __NORMAL_MONSTER_FACTORY_H__

#include "cocos2d.h"
#include "Enemies/NormalMonster/NormalMonster.h"

#include "Enemies/NormalMonster/ShadowSentinels.h"
#include "Enemies/NormalMonster/Skeleton.h"
#include "Enemies/NormalMonster/Slime.h"
#include "Enemies/NormalMonster/BabySpider.h"
#include "Enemies/NormalMonster/Angelos.h"


USING_NS_CC;

class NormalMonsterFactory
{
public:
    static NormalMonster* createNormalMonster(std::string NormalMonsterType);
    static std::vector<NormalMonster*> normalMonsters;
    static void spawn(float dt);
    
};

#endif 
