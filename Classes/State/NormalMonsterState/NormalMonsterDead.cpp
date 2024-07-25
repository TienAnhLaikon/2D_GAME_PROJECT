#include "NormalMonsterDead.h"
#include "Enemies/NormalMonster/NormalMonster.h"

void NormalMonsterDead::EnterState() {
		//CCLOG("Hello From Dead State");
		normalMonster->getPhysicsBody()->setVelocity(Vec2(0, 0));
		Animate* animateResult;
		if (normalMonster->getCurrentDirection() == 1) {
			animateResult = ((normalMonster->specialDeath_E));
		}

		else if (normalMonster->getCurrentDirection() == 2) {
			animateResult = ((normalMonster->specialDeath_NE));
		}

		else if (normalMonster->getCurrentDirection() == 3) {
			animateResult = ((normalMonster->specialDeath_N));
		}

		else if (normalMonster->getCurrentDirection() == 4) {
			animateResult = ((normalMonster->specialDeath_NW));
		}

		else if (normalMonster->getCurrentDirection() == 5) {
			animateResult = ((normalMonster->specialDeath_W));
		}

		else if (normalMonster->getCurrentDirection() == 6) {
			animateResult = ((normalMonster->specialDeath_SW));
		}

		else if (normalMonster->getCurrentDirection() == 7) {
			animateResult = ((normalMonster->specialDeath_S));
		}

		else if (normalMonster->getCurrentDirection() == 8) {
			animateResult = ((normalMonster->specialDeath_SE));
		}

		normalMonster->getCurrentSprite()->runAction(Sequence::create(animateResult, FadeOut::create(1.0),
			cocos2d::CallFunc::create([this]() {
				normalMonster->setVisible(false);
				}),nullptr));


		//normalMonster->getCurrentSprite()->runAction(
		//	);
}

void NormalMonsterDead::ExitState() {
	if (normalMonster->getCurrentSprite()->getNumberOfRunningActions() > 0)normalMonster->getCurrentSprite()->stopAllActions();
}

void NormalMonsterDead::UpdateState() {

}

