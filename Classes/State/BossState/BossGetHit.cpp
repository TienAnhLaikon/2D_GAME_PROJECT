#include "BossGetHit.h"
#include "Enemies/Boss/Boss.h"

void BossGetHit::EnterState() {
	//CCLOG("Hello From GetHit State");
	if (boss->getCurrentDirection() == 1) {
		boss->getCurrentSprite()->runAction(RepeatForever::create(boss->getHit_E));
	}
	if (boss->getCurrentDirection() == 2) {
		boss->getCurrentSprite()->runAction(RepeatForever::create(boss->getHit_NE));
	}
	if (boss->getCurrentDirection() == 3) {
		boss->getCurrentSprite()->runAction(RepeatForever::create(boss->getHit_N));
	}
	if (boss->getCurrentDirection() == 4) {
		boss->getCurrentSprite()->runAction(RepeatForever::create(boss->getHit_NW));
	}
	if (boss->getCurrentDirection() == 5) {
		boss->getCurrentSprite()->runAction(RepeatForever::create(boss->getHit_W));
	}
	if (boss->getCurrentDirection() == 6) {
		boss->getCurrentSprite()->runAction(RepeatForever::create(boss->getHit_SW));
	}
	if (boss->getCurrentDirection() == 7) {
		boss->getCurrentSprite()->runAction(RepeatForever::create(boss->getHit_S));
	}
	if (boss->getCurrentDirection() == 8) {
		boss->getCurrentSprite()->runAction(RepeatForever::create(boss->getHit_SE));
	}

	lastDir = boss->getCurrentDirection();
}

void BossGetHit::ExitState() {

}

void BossGetHit::UpdateState() {

}

