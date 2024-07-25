#include "BossIdle.h"
#include "Enemies/Boss/Boss.h"

void BossIdle::EnterState() {
	//CCLOG("Hello From Idle State");
	if (boss->getCurrentDirection() == 1) {
		boss->getCurrentSprite()->runAction(RepeatForever::create(boss->idle_E));
	}
	else if (boss->getCurrentDirection() == 2) {
		boss->getCurrentSprite()->runAction(RepeatForever::create(boss->idle_NE));
	}

	else if (boss->getCurrentDirection() == 3) {
		boss->getCurrentSprite()->runAction(RepeatForever::create(boss->idle_N));
	}

	else if (boss->getCurrentDirection() == 4) {
		boss->getCurrentSprite()->runAction(RepeatForever::create(boss->idle_NW));
	}

	else if (boss->getCurrentDirection() == 5) {
		boss->getCurrentSprite()->runAction(RepeatForever::create(boss->idle_W));
	}

	else if (boss->getCurrentDirection() == 6) {
		boss->getCurrentSprite()->runAction(RepeatForever::create(boss->idle_SW));
	}

	else if (boss->getCurrentDirection() == 7) {
		boss->getCurrentSprite()->runAction(RepeatForever::create(boss->idle_S));
	}

	else if (boss->getCurrentDirection() == 8) {
		boss->getCurrentSprite()->runAction(RepeatForever::create(boss->idle_SE));
	}

	lastDir = boss->getCurrentDirection();
}

void BossIdle::ExitState() {
	if (boss->getCurrentSprite()->getNumberOfRunningActions() > 0)boss->getCurrentSprite()->stopAllActions();
}


void BossIdle::UpdateState() {
	if (lastDir != boss->getCurrentDirection()) {
		boss->SwitchState(boss->idleState);
	}
	if (boss->getPhysicsBody()->getVelocity().x != 0 || boss->getPhysicsBody()->getVelocity().y != 0) {
		boss->SwitchState(boss->walkState);
	}
	if (boss->isTargetInAttackRange) {
		boss->SwitchState(boss->attackState);
	}
}

