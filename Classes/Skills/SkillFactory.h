#ifndef __SKILL_FACTORY_H__
#define __SKILL_FACTORY_H__

#include "cocos2d.h"
#include "SkillBase.h"

#include "Skills/Thunder/ThunderBolt.h"
#include "Skills/Thunder/ThunderCastA.h"
#include "Skills/Thunder/ThunderCastB.h"
#include "Skills/Thunder/ThunderCrackle.h"
#include "Skills/Thunder/ThunderCurrent.h"
#include "Skills/Thunder/ThunderDynamo.h"
#include "Skills/Thunder/ThunderFulminationA.h"
#include "Skills/Thunder/ThunderFulminationB.h"
#include "Skills/Thunder/ThunderShock.h"
#include "Skills/Thunder/ThunderSpark.h"
#include "Skills/Thunder/ThunderVoltage.h"

#include "Skills/Fire/FireBall.h"
#include "Skills/Fire/FireCastA.h"
#include "Skills/Fire/FireCastB.h"
#include "Skills/Fire/FireCombust.h"
#include "Skills/Fire/FireConflagration.h"
#include "Skills/Fire/FireEruption.h"
#include "Skills/Fire/FireFlare.h"
#include "Skills/Fire/FireProminence.h"
#include "Skills/Fire/FireScorch.h"
#include "Skills/Fire/FireSear.h"

USING_NS_CC;

class SkillFactory
{
public:
    static SkillBase* createSkill(std::string skillType);
};

#endif 
