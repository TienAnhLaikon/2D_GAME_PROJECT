#include "NormalMonsterAttack.h"
#include "Enemies/NormalMonster/NormalMonster.h"

void NormalMonsterAttack::EnterState() {
	//CCLOG("Normal monster Attack State");
		Animate* animateResult;
		if (normalMonster->getCurrentDirection() == 1) {
			animateResult = ((normalMonster->defaultAttack_E));
		}
		else if (normalMonster->getCurrentDirection() == 2) {
			animateResult = ((normalMonster->defaultAttack_NE));
		}
		else if (normalMonster->getCurrentDirection() == 3) {
			animateResult = ((normalMonster->defaultAttack_N));
		}
		else if (normalMonster->getCurrentDirection() == 4) {
			animateResult = ((normalMonster->defaultAttack_NW));
		}
		else if (normalMonster->getCurrentDirection() == 5) {
			animateResult = ((normalMonster->defaultAttack_W));
		}

		else if (normalMonster->getCurrentDirection() == 6) {
			animateResult = ((normalMonster->defaultAttack_SW));
		}

		else if (normalMonster->getCurrentDirection() == 7) {
			animateResult = ((normalMonster->defaultAttack_S));
		}

		else if (normalMonster->getCurrentDirection() == 8) {
			animateResult = ((normalMonster->defaultAttack_SE));
		}

		lastDir = normalMonster->getCurrentDirection();

		normalMonster->getCurrentSprite()->runAction(cocos2d::Sequence::create(
			animateResult,
			cocos2d::CallFunc::create([this]() {
					//Running
					if (normalMonster->getPhysicsBody()->getVelocity().y != 0 || normalMonster->getPhysicsBody()->getVelocity().x != 0) {
						normalMonster->SwitchState(normalMonster->walkState);
					}
					//Idle
					if (normalMonster->getPhysicsBody()->getVelocity().y == 0 && normalMonster->getPhysicsBody()->getVelocity().x == 0) {
						if (normalMonster->isTargetInAttackRange) {
							UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/enemy_hit_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));

							normalMonster->SwitchState(normalMonster->attackState);
							normalMonster->getCurrentTarget()->takeDamage(normalMonster->getDamage());
						}
						else {
							normalMonster->SwitchState(normalMonster->idleState);
						}
					}
				}),
			nullptr
		));
}

void NormalMonsterAttack::ExitState() {
		if (normalMonster->getCurrentSprite()->getNumberOfRunningActions() > 0)normalMonster->getCurrentSprite()->stopAllActions();
}

void NormalMonsterAttack::UpdateState() {
	//if (normalMonster->getCurrentHP() <= 0) {
	//	normalMonster->die();
	//}
}

