#include "BossFactory.h"

Boss* BossFactory::createBoss(std::string BossType) {
	Boss* result = nullptr;

	if (BossType == "HellBeast") {
		result = HellBeast::create();
	}
	else if (BossType == "Okyanus") {
		result = Okyanus::create();
	}
	else if (BossType == "Paradiso") {
		result = Paradiso::create();
	}
	return result;
}