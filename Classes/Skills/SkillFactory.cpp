#include "SkillFactory.h"

SkillBase* SkillFactory::createSkill(std::string skillType) {
	SkillBase* result;
	
	//Thunder
	if (skillType == "ThunderBolt") {
		result = ThunderBolt::create();
	}
	if (skillType == "ThunderCastA") {
		result = ThunderCastA::create();
	}
	if (skillType == "ThunderCastB") {
		result = ThunderCastB::create();
	}
	if (skillType == "ThunderCrackle") {
		result = ThunderCrackle::create();
	}
	if (skillType == "ThunderCurrent") {
		result = ThunderCurrent::create();
	}
	if (skillType == "ThunderDynamo") {
		result = ThunderDynamo::create();
	}
	if (skillType == "ThunderFulminationA") {
		result = ThunderFulminationA::create();
	}
	if (skillType == "ThunderFulminationB") {
		result = ThunderFulminationB::create();
	}
	if (skillType == "ThunderShock") {
		result = ThunderShock::create();
	}
	if (skillType == "ThunderSpark") {
		result = ThunderSpark::create();
	}
	if (skillType == "ThunderVoltage") {
		result = ThunderVoltage::create();
	}

	//Fire
	if (skillType == "FireBall") {
		result = FireBall::create();
	}
	if (skillType == "FireCastA") {
		result = FireCastA::create();
	}
	if (skillType == "FireCastB") {
		result = FireCastB::create();
	}
	if (skillType == "FireCombust") {
		result = FireCombust::create();
	}
	if (skillType == "FireConflagration") {
		result = FireConflagration::create();
	}
	if (skillType == "FireEruption") {
		result = FireEruption::create();
	}
	if (skillType == "FireFlare") {
		result = FireFlare::create();
	}
	if (skillType == "FireProminence") {
		result = FireProminence::create();
	}
	if (skillType == "FireScorch") {
		result = FireScorch::create();
	}
	if (skillType == "FireSear") {
		result = FireSear::create();
	}
	
	return result;
}