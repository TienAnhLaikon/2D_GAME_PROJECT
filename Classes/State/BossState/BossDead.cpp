#include "BossDead.h"
#include "Enemies/Boss/Boss.h"

void BossDead::EnterState() {
		boss->getPhysicsBody()->setVelocity(Vec2(0, 0));
	Animate* animateResult;
	if (boss->getCurrentDirection() == 1) {
		animateResult = ((boss->death_E));
	}

	else if (boss->getCurrentDirection() == 2) {
		animateResult = ((boss->death_NE));
	}

	else if (boss->getCurrentDirection() == 3) {
		animateResult = ((boss->death_N));
	}

	else if (boss->getCurrentDirection() == 4) {
		animateResult = ((boss->death_NW));
	}

	else if (boss->getCurrentDirection() == 5) {
		animateResult = ((boss->death_W));
	}

	else if (boss->getCurrentDirection() == 6) {
		animateResult = ((boss->death_SW));
	}

	else if (boss->getCurrentDirection() == 7) {
		animateResult = ((boss->death_S));
	}

	else if (boss->getCurrentDirection() == 8) {
		animateResult = ((boss->death_SE));
	}

	boss->getCurrentSprite()->runAction(Sequence::create(animateResult, FadeOut::create(1.0), nullptr));
}

void BossDead::ExitState() {

}

void BossDead::UpdateState() {

}

