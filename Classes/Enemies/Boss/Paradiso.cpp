#include "Paradiso.h"
#include "Engine/Engine.h"

bool Paradiso::init()
{
    // Load All Animate
    this->loadAnimate();
    // Init All States
    this->attackState = new BossAttack(this);
    this->idleState = new BossIdle(this);
    this->runState = new BossRun(this);
    this->healingState = new BossHealing(this);
    this->walkState = new BossWalk(this);
    this->getHitState = new BossGetHit(this);
    this->deadState = new BossDead(this);
    // Boss Skills
    // this->addSkill("Fortify");
    // this->addSkill("Unleash");

    direction = 1; // E

    // STATS
    // HP
    HP = 11400;
    // MP
    MP = 0;
    // HP regen
    HP_regen = 0;
    // MP regen
    MP_regen = 0;
    // Armor
    baseArmor = 120;
    // AD
    baseDamage = 350;
    // MS
    MS = 60;
    // AR
    attackRange = 50;
    // AS
    attackSpeed = 0.60;
    // Exp gain
    expGain = 770;

    // STATS BONUS
    HP_bonus = 1140;
    MP_bonus = 0;
    HP_regen_bonus = 0;
    MP_regen_bonus = 0;
    Armor_bonus = 12.8;
    AD_bonus = 23.5;
    MS_bonus = 0;
    AR_bonus = 0;
    AS_bonus = 0;
    expGain_bonus = 45.8;

    currentHP = HP;

    setAttackSpeed(attackSpeed);
    // SKILLS
    
    // Detect Player Range of Boss
    detectionRadius = 250;

    // Cool Down Period between skill uses
    cooldownPeriod = 5.0f;

    cooldownRoar = 5.0f;

    // CurrentCombo
    currentCombo = 1;

    isTargetInAttackRange = false;

    // Init Boss's Sprite
    characterCurrentSprite = Sprite::createWithSpriteFrameName("paradiso_attack1_E (1).png");

    Size size = Director::getInstance()->getVisibleSize();
    float scale = size.width / size.height;
    //// Khi to Background cho thanh mu
    backGroundBar = Sprite::create("res/boss_healthbar_background.jpg");
    backGroundBar->setScale(1 / (scale * 2.0f), 1 / (scale * 2.0f));
    /*backGroundBar->setContentSize(Size(characterCurrentSprite->getBoundingBox().size.width / scale, 30 / scale));*/
    backGroundBar->setPosition(Vec2(0, 60));
    //backGroundBar->setScale(0.2);
    this->addChild(backGroundBar);

    // Khi to thanh mu
    healthBar = ui::LoadingBar::create("res/boss_heathbar.jpg");
    healthBar->setDirection(ui::LoadingBar::Direction::LEFT); // t hng fill cho thanh mu
    healthBar->setPercent(100); // Gi tr ban u ca mu
    healthBar->setScale(1 / (scale * 2.0f), 1 / (scale * 2.0f));
    /*healthBar->setContentSize(Size(characterCurrentSprite->getBoundingBox().size.width / scale, 30 / scale));*/
    healthBar->setPosition(Vec2(0, 60));
    //healthBar->setScale(0.2);
    this->addChild(healthBar, 2);

    // Init physics
    auto physicsBody = PhysicsBody::createBox(Size(20, 20), PhysicsMaterial(1.0f, 0.0f, 0.0f));
    physicsBody->setGravityEnable(false);
    physicsBody->setRotationEnable(false);
    physicsBody->setDynamic(false);
    this->setPhysicsBody(physicsBody);
    this->addChild(characterCurrentSprite);

    registerObserver(QuestManager::getInstance());

    // Upate Detect Target Player
    schedule(CC_SCHEDULE_SELECTOR(Paradiso::updateBossSkills), 5.0f);
    return true;
}

void Paradiso::setAttackSpeed(float attackSpeed)
{
    int numSprite = 3;
    float delay = 0.17;
    // Attack1
    attack1_E = Animate::create(Engine::getInstance()->createAnimation2("paradiso_attack1_E", numSprite, delay));

    attack1_NE = Animate::create(Engine::getInstance()->createAnimation2("paradiso_attack1_NE", numSprite, delay));

    attack1_N = Animate::create(Engine::getInstance()->createAnimation2("paradiso_attack1_N", numSprite, delay));

    attack1_NW = Animate::create(Engine::getInstance()->createAnimation2("paradiso_attack1_NW", numSprite, delay));

    attack1_W = Animate::create(Engine::getInstance()->createAnimation2("paradiso_attack1_W", numSprite, delay));

    attack1_SW = Animate::create(Engine::getInstance()->createAnimation2("paradiso_attack1_SW", numSprite, delay));

    attack1_S = Animate::create(Engine::getInstance()->createAnimation2("paradiso_attack1_S", numSprite, delay));

    attack1_SE = Animate::create(Engine::getInstance()->createAnimation2("paradiso_attack1_SE", numSprite, delay));

    // Attack2
    attack2_E = Animate::create(Engine::getInstance()->createAnimation2("paradiso_attack2_E", numSprite, delay));

    attack2_NE = Animate::create(Engine::getInstance()->createAnimation2("paradiso_attack2_NE", numSprite, delay));

    attack2_N = Animate::create(Engine::getInstance()->createAnimation2("paradiso_attack2_N", numSprite, delay));

    attack2_NW = Animate::create(Engine::getInstance()->createAnimation2("paradiso_attack2_NW", numSprite, delay));

    attack2_W = Animate::create(Engine::getInstance()->createAnimation2("paradiso_attack2_W", numSprite, delay));

    attack2_SW = Animate::create(Engine::getInstance()->createAnimation2("paradiso_attack2_SW", numSprite, delay));

    attack2_S = Animate::create(Engine::getInstance()->createAnimation2("paradiso_attack2_S", numSprite, delay));

    attack2_SE = Animate::create(Engine::getInstance()->createAnimation2("paradiso_attack2_SE", numSprite, delay));


    // Attack1
    attack1_E->retain(); // 1

    attack1_NE->retain(); // 3

    attack1_N->retain(); // 5

    attack1_NW->retain(); // 7

    attack1_W->retain(); // 9

    attack1_SW->retain(); // 11

    attack1_S->retain(); // 13

    attack1_SE->retain(); // 15

    // Attack2
    attack2_E->retain(); // 2

    attack2_NE->retain(); // 3

    attack2_N->retain(); // 5

    attack2_NW->retain(); // 7

    attack2_W->retain(); // 9

    attack2_SW->retain(); // 22

    attack2_S->retain(); // 23

    attack2_SE->retain(); // 25
}

void Paradiso::loadAnimate()
{

    // Death
    death_E = Animate::create(Engine::getInstance()->createAnimation2("paradiso_death_E", 0, 0.05));

    death_NE = Animate::create(Engine::getInstance()->createAnimation2("paradiso_death_NE", 0, 0.05));

    death_N = Animate::create(Engine::getInstance()->createAnimation2("paradiso_death_N", 0, 0.05));

    death_NW = Animate::create(Engine::getInstance()->createAnimation2("paradiso_death_NW", 0, 0.05));

    death_W = Animate::create(Engine::getInstance()->createAnimation2("paradiso_death_W", 0, 0.05));

    death_SW = Animate::create(Engine::getInstance()->createAnimation2("paradiso_death_SW", 0, 0.05));

    death_S = Animate::create(Engine::getInstance()->createAnimation2("paradiso_death_S", 0, 0.05));

    death_SE = Animate::create(Engine::getInstance()->createAnimation2("paradiso_death_SE", 0, 0.05));

    // Get Hit
    getHit_E = Animate::create(Engine::getInstance()->createAnimation2("paradiso_hit_E", 3, 0.05));

    getHit_NE = Animate::create(Engine::getInstance()->createAnimation2("paradiso_hit_NE", 3, 0.05));

    getHit_N = Animate::create(Engine::getInstance()->createAnimation2("paradiso_hit_N", 3, 0.05));

    getHit_NW = Animate::create(Engine::getInstance()->createAnimation2("paradiso_hit_NW", 3, 0.05));

    getHit_W = Animate::create(Engine::getInstance()->createAnimation2("paradiso_hit_W", 3, 0.05));

    getHit_SW = Animate::create(Engine::getInstance()->createAnimation2("paradiso_hit_SW", 3, 0.05));

    getHit_S = Animate::create(Engine::getInstance()->createAnimation2("paradiso_hit_S", 3, 0.05));

    getHit_SE = Animate::create(Engine::getInstance()->createAnimation2("paradiso_hit_SE", 3, 0.05));

    // Run
    run_E = Animate::create(Engine::getInstance()->createAnimation2("paradiso_run_E", 8, 0.10));

    run_NE = Animate::create(Engine::getInstance()->createAnimation2("paradiso_run_NE", 8, 0.10));

    run_N = Animate::create(Engine::getInstance()->createAnimation2("paradiso_run_N", 8, 0.10));

    run_NW = Animate::create(Engine::getInstance()->createAnimation2("paradiso_run_NW", 8, 0.10));

    run_W = Animate::create(Engine::getInstance()->createAnimation2("paradiso_run_W", 8, 0.10));

    run_SW = Animate::create(Engine::getInstance()->createAnimation2("paradiso_run_SW", 8, 0.10));

    run_S = Animate::create(Engine::getInstance()->createAnimation2("paradiso_run_S", 8, 0.10));

    run_SE = Animate::create(Engine::getInstance()->createAnimation2("paradiso_run_SE", 8, 0.10));

    // Walk
    walk_E = Animate::create(Engine::getInstance()->createAnimation2("paradiso_walk_E", 8, 0.18));

    walk_NE = Animate::create(Engine::getInstance()->createAnimation2("paradiso_walk_NE", 8, 0.18));

    walk_N = Animate::create(Engine::getInstance()->createAnimation2("paradiso_walk_N", 8, 0.18));

    walk_NW = Animate::create(Engine::getInstance()->createAnimation2("paradiso_walk_NW", 8, 0.18));

    walk_W = Animate::create(Engine::getInstance()->createAnimation2("paradiso_walk_W", 8, 0.18));

    walk_SW = Animate::create(Engine::getInstance()->createAnimation2("paradiso_walk_SW", 8, 0.18));

    walk_S = Animate::create(Engine::getInstance()->createAnimation2("paradiso_walk_S", 8, 0.18));

    walk_SE = Animate::create(Engine::getInstance()->createAnimation2("paradiso_walk_SE", 8, 0.18));

    // Idle
    idle_E = Animate::create(Engine::getInstance()->createAnimation2("paradiso_idle_E", 3, 0.18));

    idle_NE = Animate::create(Engine::getInstance()->createAnimation2("paradiso_idle_NE", 3, 0.18));

    idle_N = Animate::create(Engine::getInstance()->createAnimation2("paradiso_idle_N", 3, 0.18));

    idle_NW = Animate::create(Engine::getInstance()->createAnimation2("paradiso_idle_NW", 3, 0.18));

    idle_W = Animate::create(Engine::getInstance()->createAnimation2("paradiso_idle_W", 3, 0.18));

    idle_SW = Animate::create(Engine::getInstance()->createAnimation2("paradiso_idle_SW", 3, 0.18));

    idle_S = Animate::create(Engine::getInstance()->createAnimation2("paradiso_idle_S", 3, 0.18));

    idle_SE = Animate::create(Engine::getInstance()->createAnimation2("paradiso_idle_SE", 3, 0.18));

    // Death
    death_E->retain(); // 1

    death_NE->retain(); // 3

    death_N->retain(); // 5

    death_NW->retain(); // 7

    death_W->retain(); // 9

    death_SW->retain(); // 11

    death_S->retain(); // 13

    death_SE->retain(); // 15

    // Get Hit
    getHit_E->retain(); // 1

    getHit_NE->retain();

    getHit_N->retain(); // 5

    getHit_NW->retain(); // 7

    getHit_W->retain(); // 9

    getHit_SW->retain(); // 11

    getHit_S->retain(); // 13

    getHit_SE->retain(); // 15

    // Run
    run_E->retain(); // 1

    run_NE->retain(); // 3

    run_N->retain(); // 5

    run_NW->retain(); // 7

    run_W->retain(); // 9

    run_SW->retain(); // 11

    run_S->retain(); // 13

    run_SE->retain(); // 15

    // Walk
    walk_E->retain(); // 1

    walk_NE->retain(); // 3

    walk_N->retain(); // 5

    walk_NW->retain(); // 7

    walk_W->retain(); // 9

    walk_SW->retain(); // 11

    walk_S->retain(); // 13

    walk_SE->retain(); // 15

    // Idle
    idle_E->retain(); // 1

    idle_NE->retain(); // 3

    idle_N->retain(); // 5

    idle_NW->retain(); // 7

    idle_W->retain(); // 9

    idle_SW->retain(); // 11

    idle_S->retain(); // 13

    idle_SE->retain(); // 15
}