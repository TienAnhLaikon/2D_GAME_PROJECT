#include "BossHealing.h"
#include "Enemies/Boss/Boss.h"

void BossHealing::EnterState() {
	// CCLOG("Hello From Healing State");
	 Animate* animateResult;
	 if (boss->getCurrentDirection() == 1) {
	 	animateResult = boss->healing_E;
	 }
	 if (boss->getCurrentDirection() == 2) {
	 	animateResult = boss->healing_NE;
	 }
	 if (boss->getCurrentDirection() == 3) {
	 	animateResult = boss->healing_N;
	 }
	 if (boss->getCurrentDirection() == 4) {
	 	animateResult = boss->healing_NW;
	 }
	 if (boss->getCurrentDirection() == 5) {
	 	animateResult = boss->healing_W;
	 }
	 if (boss->getCurrentDirection() == 6) {
	 	animateResult = boss->healing_SW;
	 }
	 if (boss->getCurrentDirection() == 7) {
	 	animateResult = boss->healing_S;
	 }
	 if (boss->getCurrentDirection() == 8) {
	 	animateResult = boss->healing_SE;
	 }
	 lastDir = boss->getCurrentDirection();

	 boss->getCurrentSprite()->runAction(cocos2d::Sequence::create(
	 	animateResult,
	 	cocos2d::CallFunc::create([this]() {
	 		if (boss->getPhysicsBody()->getVelocity().x == 0 && boss->getPhysicsBody()->getVelocity().y == 0)boss->SwitchState(boss->idleState);
	 		}),
	 	nullptr
	 ));
}

void BossHealing::ExitState() {
	if (boss->getCurrentSprite()->getNumberOfRunningActions() > 0)boss->getCurrentSprite()->stopAllActions();
}

void BossHealing::UpdateState() {

}

