#include "NormalMonsterWalk.h"
#include "Enemies/NormalMonster/NormalMonster.h"

void NormalMonsterWalk::EnterState() {
	//CCLOG("Hello From Walk State");
		if (normalMonster->getCurrentDirection() == 1) {
			normalMonster->getCurrentSprite()->runAction((normalMonster->defaultWalk_E));
		}
		else if (normalMonster->getCurrentDirection() == 2) {
			normalMonster->getCurrentSprite()->runAction((normalMonster->defaultWalk_NE));
		}

		else if (normalMonster->getCurrentDirection() == 3) {
			normalMonster->getCurrentSprite()->runAction((normalMonster->defaultWalk_N));
		}

		else if (normalMonster->getCurrentDirection() == 4) {
			normalMonster->getCurrentSprite()->runAction((normalMonster->defaultWalk_NW));
		}

		else if (normalMonster->getCurrentDirection() == 5) {
			normalMonster->getCurrentSprite()->runAction((normalMonster->defaultWalk_W));
		}

		else if (normalMonster->getCurrentDirection() == 6) {
			normalMonster->getCurrentSprite()->runAction((normalMonster->defaultWalk_SW));
		}

		else if (normalMonster->getCurrentDirection() == 7) {
			normalMonster->getCurrentSprite()->runAction((normalMonster->defaultWalk_S));
		}

		else if (normalMonster->getCurrentDirection() == 8) {
			normalMonster->getCurrentSprite()->runAction((normalMonster->defaultWalk_SE));
		}

		lastDir = normalMonster->getCurrentDirection();
}

void NormalMonsterWalk::ExitState() {
		if (normalMonster->getCurrentSprite()->getNumberOfRunningActions() > 0)normalMonster->getCurrentSprite()->stopAllActions();
}

void NormalMonsterWalk::UpdateState() {
		if (normalMonster->getCurrentSprite()->getNumberOfRunningActions() == 0) {
			normalMonster->SwitchState(normalMonster->walkState);
		}
		if (normalMonster->getPhysicsBody()->getVelocity().x == 0 && normalMonster->getPhysicsBody()->getVelocity().y == 0) {
			if (normalMonster->isTargetInAttackRange && normalMonster->attackState) {
				normalMonster->SwitchState(normalMonster->attackState);
			}
			else normalMonster->SwitchState(normalMonster->idleState);
		}
		//if (normalMonster->getCurrentHP() == 0) {
		//	normalMonster->SwitchState(normalMonster->deadState);
		//}
}

