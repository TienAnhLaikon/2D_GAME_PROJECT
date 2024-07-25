#include "BossSkillFactory.h"

BossSkillBase* BossSkillFactory::createBossSkill(std::string skillName) {
	BossSkillBase* result;
	if (skillName == "Fortify") {
		result = Fortify::create();
	}
	if (skillName == "Unleash") {
		result = Unleash::create();
	}
	if (skillName == "Burst") {
		result = Burst::create();
	}
	if (skillName == "Cure") {
		result = Cure::create();
	}
	if (skillName == "MultiFireBall") {
		result = MultiFireBall::create();
	}
	if (skillName == "BombRainbow") {
		result = BombRainbow::create();
	}
	if (skillName == "Summon") {
		result = Summon::create();
	}
	return result;
}