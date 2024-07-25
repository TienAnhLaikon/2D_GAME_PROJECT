#include "Boss.h"
#include "Game/Game.h"
#include "Map/Map.h"
#include "HelloWorldScene.h"

bool Boss::init() {
    return true;
}

void Boss::detectPlayer() {

    // Vị trí của boss
    Vec2 bossPosition = this->getPosition();

    // Lấy scene chính từ Director
    Scene* currentScene = Director::getInstance()->getRunningScene();
    if (currentScene) {
        Game* game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
        if (game) {
            auto children = game->gameMap->getTiledMap()->getChildren();
            for (const auto& child : children) {
                // Kiểm tra xem đối tượng con có phải là Player không
                auto player = dynamic_cast<Player*>(child);
                if (player) {

                    // Tính khoảng cách giữa boss và Player
                    Vec2 playerPosition = player->getPosition();
                    float distance = bossPosition.distance(playerPosition);

                    // Nếu Player nằm trong phạm vi phát hiện của boss
                    if (distance <= detectionRadius) {
                        // Đặt mục tiêu cho boss
                        setTarget(player);
                        // Ngừng việc kiểm tra các Player khác nếu chỉ cần đặt một mục tiêu
                        return;
                    }
                }
            }
        }
    }
    //setTarget(nullptr);

}

float Boss::calculateAngle(const Vec2& vectorA, const Vec2& vectorB) {
    // Tính góc giữa hai vector sử dụng atan2
    float angle = vectorA.getAngle(vectorB);

    // Chuyển góc từ radian sang độ
    auto degree = CC_RADIANS_TO_DEGREES(angle);


    return degree;
}

void Boss::runToTarget() {

    if (target && this->currentState != deadState) {
        // Vector hướng từ vị trí hiện tại tới vị trí mục tiêu
        Vec2 direction = (target->getPosition() - this->getPosition()).getNormalized();

        // Tính toán vận tốc dựa trên thời gian di chuyển mong muốn

        Vec2 velocity = direction * this->MS;
        // Thiết lập vận tốc cho Node
        if (this == dynamic_cast <HellBeast*>(this) || this == dynamic_cast <Paradiso*>(this))
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

void Boss::walkToTarget() {

}

void Boss::stopRunAndWalk() {
    //If Boss has Target, Boss will chase till reach Attack Range: isTargetInAttackRange=true;
    if (target) {
        if (this->getPosition().distance(target->getPosition()) <= attackRange) {
            this->getPhysicsBody()->setVelocity(Vec2::ZERO);
            isTargetInAttackRange = true;
        }
        else {
            isTargetInAttackRange = false;
        }
    }
    // No Target, Boss will Stand Idle
    else {
        this->getPhysicsBody()->setVelocity(Vec2::ZERO);
        isTargetInAttackRange = false;
    }
}
void Boss::attack() {
    //if(target)
    //CCLOG("%s", target->getName());
    /*if (target && target->getPosition().distance(this->getPosition()) <= this->attackRange
        && this->getAAcanDealDamage() == true)target->takeDamage(this->getDamage());*/
}

void Boss::update(float dt) {
    Boss::detectPlayer();
    if (this->currentState != this->roarState)Boss:runToTarget();
    //If distance closed, stop Boss
    Boss::stopRunAndWalk();
    attack();
    this->currentState->UpdateState();
}
void Boss::die() {
    if (this->getCurrentHP() <= 0 && this->currentState != deadState) {
        if (this->getCurrentSprite()->getNumberOfRunningActions() > 0)this->getCurrentSprite()->stopAllActions();
        isDead = true;
        target->gainExp(this->getExpGain());

        this->healthBar->setVisible(false);
        this->backGroundBar->setVisible(false);
        this->monsterName->setVisible(false);

        if (QuestManager::getInstance()->currentQuest && QuestManager::getInstance()->currentQuest->questStatus == QuestManager::getInstance()->currentQuest->RECEIVED)
            notifyObservers();
        this->SwitchState(this->deadState);
    }
}

void Boss::updateBossSkills(float dt) {
    if (target && this->currentState != attackState && this->currentState != deadState
        && target->currentState != target->deadState) {
        useSkillWithDelay(getRandomSkill(), target->getPosition(), 1.0f);
    }
}

BossSkillBase* Boss::getRandomSkill() {
    // Kiểm tra xem danh sách kỹ năng có rỗng không
    if (skills.empty()) {
        return nullptr; // Trả về nullptr nếu không có kỹ năng nào
    }

    // Tạo một đối tượng phát sinh số ngẫu nhiên
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, skills.size() - 1);

    // Chọn một chỉ số ngẫu nhiên từ 0 đến kích thước của danh sách kỹ năng - 1
    int randomIndex = dis(gen);

    if (summonTime < 2) {
        if(randomIndex == 0)
        summonTime++;
    }
    else {
        if(randomIndex == 0)
        randomIndex = 2;
    }
        if (randomIndex == 0 || randomIndex == 3) {
            if (this->attackState) {

                this->SwitchState(roarState);
            }
        }
        else {
            if (this->attackState) {
                this->currentCombo = 3;
                this->SwitchState(attackState);
            }
        }

    // Trả về kỹ năng tại chỉ số ngẫu nhiên
    return skills[randomIndex];
}

void Boss::useSkillWithDelay(BossSkillBase* skill, Vec2 target, float delayTime) {
    if (skills.empty()) return;
    // Bước 1: Hiển thị Range Indicator
    // Target is in Boss Corrdinate, so i convert to scene game coordinate
    //Vec2 targetInSceneCoor = this->getScene()->convertToNodeSpace(target);
    skill->castRangeIndicator(target);

    // Bước 2: Tạo hành động delay trước khi thực hiện skill
    auto delayAction = DelayTime::create(delayTime);

    // Bước 3: Thực hiện skill sau khi delay
    auto performAction = CallFunc::create([=]() {
        skill->performSkill(target);
        });

    // Tạo chuỗi hành động gồm delayAction và performAction
    auto sequence = Sequence::create(delayAction, performAction, nullptr);

    // Thực hiện chuỗi hành động trên Boss
    this->runAction(sequence);

}

