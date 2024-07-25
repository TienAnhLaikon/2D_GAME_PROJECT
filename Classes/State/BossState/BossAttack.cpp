#include "BossAttack.h"
#include "Enemies/Boss/Boss.h"

const int ATTACK1 = 1;
const int ATTACK2 = 2;
const int ATTACK3 = 3;
void BossAttack::EnterState() {
	//CCLOG("Hello From Attack State");
    Animate* animateResult;
    if(boss->getPositionX() < boss->target->getPositionX())
    animateResult = boss->attack1_E;
    if (boss->getPositionX() >= boss->target->getPositionX())
        animateResult = boss->attack1_W;
    if (boss->getCurrentDirection() == 1) {
        if(boss->currentCombo == ATTACK1)animateResult = boss->attack1_E;
        else if (boss->currentCombo == ATTACK2)animateResult = boss->attack2_E;
        else if (boss->currentCombo == ATTACK3)animateResult = boss->attack3_E;
    }
    else if (boss->getCurrentDirection() == 2) {
        if (boss->currentCombo == ATTACK1)animateResult = boss->attack1_NE;
        else if (boss->currentCombo == ATTACK2)animateResult = boss->attack2_NE;
        else if (boss->currentCombo == ATTACK3)animateResult = boss->attack3_NE;
    }

    else if (boss->getCurrentDirection() == 3) {
        if (boss->currentCombo == ATTACK1)animateResult = boss->attack1_N;
        else if (boss->currentCombo == ATTACK2)animateResult = boss->attack2_N;
        else if (boss->currentCombo == ATTACK3)animateResult = boss->attack3_N;
    }

    else if (boss->getCurrentDirection() == 4) {
        if (boss->currentCombo == ATTACK1)animateResult = boss->attack1_NW;
        else if (boss->currentCombo == ATTACK2)animateResult = boss->attack2_NW;
        else if (boss->currentCombo == ATTACK3)animateResult = boss->attack3_NW;
    }

    else if (boss->getCurrentDirection() == 5) {
        if (boss->currentCombo == ATTACK1)animateResult = boss->attack1_W;
        else if (boss->currentCombo == ATTACK2)animateResult = boss->attack2_W;
        else if (boss->currentCombo == ATTACK3)animateResult = boss->attack3_W;
    }

    else if (boss->getCurrentDirection() == 6) {
        if (boss->currentCombo == ATTACK1)animateResult = boss->attack1_SW;
        else if (boss->currentCombo == ATTACK2)animateResult = boss->attack2_SW;
        else if (boss->currentCombo == ATTACK3)animateResult = boss->attack3_SW;
    }

    else if (boss->getCurrentDirection() == 7) {
        if (boss->currentCombo == ATTACK1)animateResult = boss->attack1_S;
        else if (boss->currentCombo == ATTACK2)animateResult = boss->attack2_S;
        else if (boss->currentCombo == ATTACK3)animateResult = boss->attack3_S;
    }

    else if (boss->getCurrentDirection() == 8) {
        if (boss->currentCombo == ATTACK1)animateResult = boss->attack1_SE;
        else if (boss->currentCombo == ATTACK2)animateResult = boss->attack2_SE;
        else if (boss->currentCombo == ATTACK3)animateResult = boss->attack3_SE;
    }

    lastDir = boss->getCurrentDirection();
    boss->getCurrentSprite()->runAction(cocos2d::Sequence::create(
        cocos2d::CallFunc::create([this]() {
            float amountDamage = boss->getDamage() + 100.0f * (boss->currentCombo - 1);
            if(boss->getPosition().distance(boss->target->getPosition()) <= 50)
            boss->getCurrentTarget()->takeDamage(amountDamage);
            }),
        animateResult,
        cocos2d::CallFunc::create([this]() {
            
            // Khi animation kt thc, chuyn trng thi v trng thi trc 
            boss->currentCombo++;
            auto bossName = boss->getName();
           // CCLOG("BOSS NAME: %s", bossName);
            if (bossName == "Okyanus" || bossName == "HellBeast") {
                if (boss->currentCombo == 4) {
                    boss->currentCombo = 1;
                }
            }
            else if (bossName == "Paradiso") {
                if (boss->currentCombo == 3) {
                    boss->currentCombo = 1;
                }
            }
            //Running
            if (boss->getPhysicsBody()->getVelocity().y != 0 || boss->getPhysicsBody()->getVelocity().x != 0) {
                boss->currentCombo = 1;
                boss->SwitchState(boss->walkState);
            }
            //Idle
            if (boss->getPhysicsBody()->getVelocity().y == 0 && boss->getPhysicsBody()->getVelocity().x == 0) {
                if (boss->isTargetInAttackRange && boss->target->currentState != boss->target->deadState) { 
                    boss->SwitchState(boss->attackState); 
                }
                else {
                    boss->currentCombo = 1;
                    boss->SwitchState(boss->idleState);
                }
            }
            }),
        nullptr
    ));
}

void BossAttack::ExitState() {

}

void BossAttack::UpdateState() {
   // CCLOG("Updating for Boss");
    
}

