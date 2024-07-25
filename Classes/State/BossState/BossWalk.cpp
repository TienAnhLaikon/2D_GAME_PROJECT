#include "BossWalk.h"
#include "Enemies/Boss/Boss.h"

void BossWalk::EnterState() {
	//CCLOG("Hello From Run State");
	if (boss->getCurrentDirection() == 1) {
		boss->getCurrentSprite()->runAction((boss->walk_E));
	}
	else if (boss->getCurrentDirection() == 2) {
		boss->getCurrentSprite()->runAction((boss->walk_NE));
	}
	else if (boss->getCurrentDirection() == 3) {
		boss->getCurrentSprite()->runAction((boss->walk_N));
	}
	else if (boss->getCurrentDirection() == 4) {
		boss->getCurrentSprite()->runAction((boss->walk_NW));
	}
	else if (boss->getCurrentDirection() == 5) {
		boss->getCurrentSprite()->runAction((boss->walk_W));
	}
	else if (boss->getCurrentDirection() == 6) {
		boss->getCurrentSprite()->runAction((boss->walk_SW));
	}
	else if (boss->getCurrentDirection() == 7) {
		boss->getCurrentSprite()->runAction((boss->walk_S));
	}
	else if (boss->getCurrentDirection() == 8) {
		boss->getCurrentSprite()->runAction((boss->walk_SE));
	}
	lastDir = boss->getCurrentDirection();
}

void BossWalk::ExitState() {
	if (boss->getCurrentSprite()->getNumberOfRunningActions() > 0)boss->getCurrentSprite()->stopAllActions();
}

void BossWalk::UpdateState() {
	if (boss->getCurrentSprite()->getNumberOfRunningActions() == 0) {
		boss->SwitchState(boss->walkState);
	}
	if (boss->getPhysicsBody()->getVelocity().x == 0 && boss->getPhysicsBody()->getVelocity().y == 0) {
		if (boss->isTargetInAttackRange) {
			boss->SwitchState(boss->attackState);
		}
		else boss->SwitchState(boss->idleState);
	}

}

