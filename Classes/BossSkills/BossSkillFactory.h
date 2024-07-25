#ifndef __BOSS_SKILL_FACTORY_H__
#define __BOSS_SKILL_FACTORY_H__

#include "cocos2d.h"
#include "BossSkillBase.h"
#include "BossSkills/HellBeastSkill/Fortify.h"
#include "BossSkills/HellBeastSkill/Unleash.h"
#include "BossSkills/HellBeastSkill/MultiFireBall.h"
#include "BossSkills/HellBeastSkill/BombRainbow.h"
#include "BossSkills/HellBeastSkill/Summon.h"

#include "BossSkills/OkyanusSkill/Burst.h"
#include "BossSkills/OkyanusSkill/Cure.h"

USING_NS_CC;

class BossSkillFactory {
public:
	static BossSkillBase* createBossSkill(std::string skillName);

};
#endif // !__BOSS_SKILL_BASE_H__
