#ifndef __BOSS_H__
#define __BOSS_H__

#include "cocos2d.h"

#include "State/BossState/BossStateBase.h"
#include "State/BossState/BossAttack.h"
#include "State/BossState/BossDead.h"
#include "State/BossState/BossGetHit.h"
#include "State/BossState/BossIdle.h"
#include "State/BossState/BossRoar.h"
#include "State/BossState/BossRun.h"
#include "State/BossState/BossWalk.h"
#include "State/BossState/BossHealing.h"
#include "BossSkills/BossSkillBase.h"
#include "BossSkills/BossSkillFactory.h"
#include "Item/BaseItem.h"
#include "Player/Player.h"
#include "Enemies/Enemy.h"
USING_NS_CC;

class Boss : public Enemy
{
public:
    BossStateBase* currentState;
    BossStateBase* attackState;
    BossStateBase* deadState;
    BossStateBase* idleState;
    BossStateBase* getHitState;
    BossStateBase* runState;
    BossStateBase* walkState;  

    // thuc tnh ring ca Terra
    BossStateBase* roarState;

    // Thuc tnh ring ca Okyanus 
    BossStateBase* angryState;
    
    // thuc tnh ring ca paradiso
    BossStateBase* healingState;

    //Attack1
    Animate* attack1_E; //1
    Animate* attack1_NE; //3
    Animate* attack1_N; //5
    Animate* attack1_NW; //7
    Animate* attack1_W; //9
    Animate* attack1_SW; //11
    Animate* attack1_S; //13
    Animate* attack1_SE; //15

    //Attack2
    Animate* attack2_E; //2
    Animate* attack2_NE; //3
    Animate* attack2_N; //5
    Animate* attack2_NW; //7
    Animate* attack2_W; //9
    Animate* attack2_SW; //22
    Animate* attack2_S; //23
    Animate* attack2_SE; //25

    //Attack3
    Animate* attack3_E; //3
    Animate* attack3_NE; //3
    Animate* attack3_N; //5
    Animate* attack3_NW; //7
    Animate* attack3_W; //9
    Animate* attack3_SW; //33
    Animate* attack3_S; //33
    Animate* attack3_SE; //35

    //Death
    Animate* death_E; //3
    Animate* death_NE; //3
    Animate* death_N; //5
    Animate* death_NW; //7
    Animate* death_W; //9
    Animate* death_SW; //33
    Animate* death_S; //33
    Animate* death_SE; //35

    //Get Hit
    Animate* getHit_E; //3
    Animate* getHit_NE; //3
    Animate* getHit_N; //5
    Animate* getHit_NW; //7
    Animate* getHit_W; //9
    Animate* getHit_SW; //33
    Animate* getHit_S; //33
    Animate* getHit_SE; //35

    //Run
    Animate* run_E; //3
    Animate* run_NE; //3
    Animate* run_N; //5
    Animate* run_NW; //7
    Animate* run_W; //9
    Animate* run_SW; //33
    Animate* run_S; //33
    Animate* run_SE; //35

    //Walk
    Animate* walk_E; //3
    Animate* walk_NE; //3
    Animate* walk_N; //5
    Animate* walk_NW; //7
    Animate* walk_W; //9
    Animate* walk_SW; //33
    Animate* walk_S; //33
    Animate* walk_SE; //35

    //Roar
    Animate* roar_E; //3
    Animate* roar_NE; //3
    Animate* roar_N; //5
    Animate* roar_NW; //7
    Animate* roar_W; //9
    Animate* roar_SW; //33
    Animate* roar_S; //33
    Animate* roar_SE; //35

    //Idle
    Animate* idle_E; //3
    Animate* idle_NE; //3
    Animate* idle_N; //5
    Animate* idle_NW; //7
    Animate* idle_W; //9
    Animate* idle_SW; //33
    Animate* idle_S; //33
    Animate* idle_SE; //35

    //angry
    Animate* angry_E; //3
    Animate* angry_NE; //3
    Animate* angry_N; //5
    Animate* angry_NW; //7
    Animate* angry_W; //9
    Animate* angry_SW; //33
    Animate* angry_S; //33
    Animate* angry_SE; //35

    // healing
    Animate* healing_E;
    Animate* healing_NE;
    Animate* healing_N;
    Animate* healing_NW;
    Animate* healing_W;
    Animate* healing_SW;
    Animate* healing_S;
    Animate* healing_SE;

    ///
    void Initalize(BossStateBase* startingState) {
        currentState = startingState;
        currentState->EnterState();
    }

    void SwitchState(BossStateBase* newState) {
        /*if (currentState == newState) {
            CCLOG("Not change to new State!");
            return;
        }*/
        currentState->ExitState();
        currentState = newState;
        currentState->EnterState();
    }
    void takeDamage(float amountDamage) {
        float damageActuallyTake = amountDamage * (100 / (100 + this->baseArmor));
        if (this->currentHP <= damageActuallyTake) {
            currentHP = 0;
        }
        else {
            currentHP -= damageActuallyTake;
        }
        healthBar->setPercent((currentHP / HP) * 100);
        //Lable
        auto dmgTakeLable = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 10);
        dmgTakeLable->setColor(Color3B::RED);
        this->addChild(dmgTakeLable);
        int dmgToInt = std::floor(damageActuallyTake);
        dmgTakeLable->setString(StringUtils::format("-%d", dmgToInt));
        Vec2 pos = Vec2(0, characterCurrentSprite->getBoundingBox().size.height / 2);
        auto moveTo = MoveTo::create(1.0f, pos);
        auto fadeOut = FadeOut::create(1.0f);
        auto spawn = Spawn::create(fadeOut, moveTo, nullptr);
        auto sqe = Sequence::create(spawn, RemoveSelf::create(), nullptr);
        dmgTakeLable->runAction(sqe);
    }
    float cooldownPeriod;

    float cooldownRoar;
    //Props

    bool isTargetInAttackRange;

    int summonTime = 0;

    int currentCombo;

    //Behaviors

    virtual bool init();
    
    void update(float dt);

    void detectPlayer();

    void runToTarget();

    void attack();

    void die();

    void walkToTarget();

    void stopRunAndWalk();

    float calculateAngle(const Vec2& vectorA, const Vec2& vectorB);

    void updateBossSkills(float dt);
    
    BossSkillBase* getRandomSkill();

    void useSkillWithDelay(BossSkillBase* skill, Vec2 target, float delayTime);

    void addSkill(std::string skillName) {
        auto skill = BossSkillFactory::createBossSkill(skillName);
        skills.push_back(skill);
        skill->retain();
        this->addChild(skill);
    }

    void setTarget(Player* newTarget) {
        target = newTarget;
    }

    Player* getCurrentTarget() {
        return this->target;
    }
    CREATE_FUNC(Boss);

    std::vector<BaseItem*> drops;

    std::vector<BossSkillBase*> skills;
    float detectionRadius;

    Player* target;
};

#endif 
