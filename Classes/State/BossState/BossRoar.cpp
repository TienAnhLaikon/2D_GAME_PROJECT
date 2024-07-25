#include "BossRoar.h"
#include "Enemies/Boss/Boss.h"

void BossRoar::EnterState() {
	//CCLOG("Hello From Roar State");
	Animate* animateResult;
	boss->getPhysicsBody()->setVelocity(Vec2::ZERO);

	if (boss->getCurrentDirection() == 1) {
		animateResult = boss->roar_E;
	}

	else if (boss->getCurrentDirection() == 2) {
		animateResult = boss->roar_NE;
	}

	else if (boss->getCurrentDirection() == 3) {
		animateResult = boss->roar_N;
	}

	else if (boss->getCurrentDirection() == 4) {
		animateResult = boss->roar_NW;
	}

	else if (boss->getCurrentDirection() == 5) {
		animateResult = boss->roar_W;
	}

	else if (boss->getCurrentDirection() == 6) {
		animateResult = boss->roar_SW;
	}

	else if (boss->getCurrentDirection() == 7) {
		animateResult = boss->roar_S;
	}

	else if (boss->getCurrentDirection() == 8) {
		animateResult = boss->roar_SE;
	}

	lastDir = boss->getCurrentDirection();

	boss->getCurrentSprite()->runAction(cocos2d::Sequence::create(
		animateResult,
		cocos2d::CallFunc::create([this]() {
			if (boss->target) {
				Vec2 direction = boss->target->getPosition() - boss->getPosition();
				direction.normalize();
				boss->getPhysicsBody()->setVelocity(lastDir * direction * boss->getMS());
			}
			if (boss->getPhysicsBody()->getVelocity().x != 0 || boss->getPhysicsBody()->getVelocity().y != 0)boss->SwitchState(boss->walkState);
			if (boss->getPhysicsBody()->getVelocity().x == 0 && boss->getPhysicsBody()->getVelocity().y == 0)boss->SwitchState(boss->idleState);
			}),
		nullptr
	));
}

void BossRoar::ExitState() {
	if (boss->getCurrentSprite()->getNumberOfRunningActions() > 0)boss->getCurrentSprite()->stopAllActions();
}

void BossRoar::UpdateState() {

}

