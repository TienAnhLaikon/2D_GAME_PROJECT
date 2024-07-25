#include "NormalMonsterFactory.h"

NormalMonster* NormalMonsterFactory::createNormalMonster(std::string NormalMonsterType) {
	NormalMonster* result = nullptr;

	if (NormalMonsterType == "Skeleton") {
		result = Skeleton::create();
	}
	else if (NormalMonsterType == "Slime") {
		result = Slime::create();
	}
	else if (NormalMonsterType == "ShadowLord") {
		result = ShadowSentinels::create();
	}
	else if (NormalMonsterType == "BabySpider") {
		result = BabySpider::create();
	}
	else if (NormalMonsterType == "Angelos") {
		result = Angelos::create();
	}
	else if (NormalMonsterType == "ShadowSentinel") {
		result = ShadowSentinels::create();
	}
	return result;
}

void NormalMonsterFactory::spawn(float dt) {

}