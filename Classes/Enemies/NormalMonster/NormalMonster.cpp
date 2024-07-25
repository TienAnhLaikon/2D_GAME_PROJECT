#include "NormalMonster.h"
#include "HelloWorldScene.h"
#include "Map/Map.h"
#include "Game/Game.h"
#include "ItemManager/ItemManager.h"
#include "NotificationManager/NotificationManager.h"
bool NormalMonster::init() {

	return true;
}

void NormalMonster::detectPlayer() {
    
}

void NormalMonster::chase() {
    if (UserDefault::getInstance()->getBoolForKey("is_pause", false) == true) {
            this->characterCurrentSprite->resume();
            if (!this->isScheduled(CC_SCHEDULE_SELECTOR(NormalMonster::updateEnemy)))
                this->schedule(CC_SCHEDULE_SELECTOR(NormalMonster::updateEnemy), 0.0f);
        }

    if (target && this->currentState!=deadState && this->currentState!=attackState) {
        Vec2 direction = (target->getPosition() - this->getPosition()).getNormalized();

        Vec2 velocity = direction * this->MS;

        // Thit lp vn tc cho Node
        this->getPhysicsBody()->setVelocity(velocity);

        float angleResult = calculateAngle(direction, Vec2(1, 0));

        if (angleResult >= -45 && angleResult < 0) { this->direction = 1; } //E
        if (angleResult >= -90 && angleResult < -45) { this->direction = 2; } //NE
        if (angleResult >= -135 && angleResult < -90) { this->direction = 3; } //N
        if (angleResult >= -180 && angleResult < -135) { this->direction = 4; } //NW
        if (angleResult >= 135 && angleResult < 180) { this->direction = 5; } //W
        if (angleResult >= 90 && angleResult < 135) { this->direction = 6; } //SW
        if (angleResult >= 45 && angleResult < 90) { this->direction = 7; } //S
        if (angleResult >= 0 && angleResult < 45) { this->direction = 8; } //SE
    }
}

void NormalMonster::stopSprite() {
    UserDefault::getInstance()->setBoolForKey("is_pause", true);
    this->characterCurrentSprite->pause();
    this->unschedule(CC_SCHEDULE_SELECTOR(NormalMonster::updateEnemy));
    this->getPhysicsBody()->setVelocity(Vec2::ZERO);
}

void NormalMonster::stopChase() {
    if (this->currentState != deadState) {
        this->SwitchState(idleState);
        this->getPhysicsBody()->setVelocity(Vec2::ZERO);
    }
}

float NormalMonster::calculateAngle(const Vec2& vectorA, const Vec2& vectorB) {
    float angle = vectorA.getAngle(vectorB);
    auto degree = CC_RADIANS_TO_DEGREES(angle);
    return degree;
}

void NormalMonster::attack() {
    if (this->currentState != deadState) {
        if (target && target->currentState != target->deadState) {
            if (this->getPosition().distance(target->getPosition()) <= attackRange) {
                this->getPhysicsBody()->setVelocity(Vec2::ZERO);
                isTargetInAttackRange = true;
            }
            else {
                isTargetInAttackRange = false;
                if (this->getCurrentSprite()->getNumberOfRunningActions() == 0)this->SwitchState(this->walkState);
            }
        }
    }
}

void NormalMonster::die() {

    //if (this->getCurrentHP() <= 0 && this->currentState != deadState) {
        /*auto game = dynamic_cast<Game*>(Director::getInstance()->getRunningScene()->getChildByName("GameInstance"));
        for (auto enemy : game->listOfMonster)
        {
            auto it = std::find(game->listOfMonster.begin(), game->listOfMonster.end(), this);
            if (it != game->listOfMonster.end())
            {
                game->listOfMonster.erase(it);
            }
        }*/
        this->healthBar->setVisible(false);
        this->backGroundBar->setVisible(false);
        this->monsterName->setVisible(false);
        this->levelSprite->setVisible(false);
        isDead = true;
        target->gainExp(this->getExpGain());

        ItemManager::dropItem(this, "crystal");
        ItemManager::dropEquipment(this, "");
        if (QuestManager::getInstance()->currentQuest && QuestManager::getInstance()->currentQuest->questStatus == QuestManager::getInstance()->currentQuest->RECEIVED)
            notifyObservers();
        this->unschedule(CC_SCHEDULE_SELECTOR(NormalMonster::updateEnemy));
        this->SwitchState(this->deadState);

    //}


}

bool NormalMonster::rayCast(const Vec2& start, const Vec2& end) {
    if (!game) {
        game = dynamic_cast<Game*>(Director::getInstance()->getRunningScene()->getChildByName("GameInstance"));
        if (!game) return false;
    }

    Vec2 direction = end - start;
    float length = direction.length();
    direction.normalize();

    float step = 39.0f; 
    int numSteps = ceil(length / step);
    for (int i = 0; i < numSteps; ++i) {
        Vec2 point = start + direction * step * i;
        for (Node* collisionNode : game->listOfCollision) {
            Rect collisionNodeRect = collisionNode->getBoundingBox();
            if (collisionNodeRect.containsPoint(point)) {
                return true;
            }
        }
        for (Node* saveZoneNode : game->listOfSafeZone) {
            Rect saveZoneNodeRect = saveZoneNode->getBoundingBox();
            if (saveZoneNodeRect.containsPoint(point)) {
                return true;
            }
        }
    }

    return false;
}

void NormalMonster::updateEnemy(float dt) {
    if (this != nullptr) {
        if (target) {
            Vec2 playerPosition = target->getPosition();
            Vec2 enemyPosition = getPosition();
            if (playerPosition.distance(enemyPosition) < this->detectionRadius && rayCast(enemyPosition, playerPosition)) {
                stopChase();
            }
            else {
                if (this->getPosition().distance(target->getPosition()) < 100)
                    chase();
                else stopChase();
            }
        }
        attack();
        currentState->UpdateState();
    }
}