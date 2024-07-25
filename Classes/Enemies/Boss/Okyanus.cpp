#include "Okyanus.h"
#include "Engine/Engine.h"

bool Okyanus::init()
{
    AAcanDealDamage;
    // Load All Animate
    this->loadAnimate();
    // Init All States
    this->attackState = new BossAttack(this);
    this->idleState = new BossIdle(this);
    this->getHitState = new BossGetHit(this);
    this->deadState = new BossDead(this);

    this->addSkill("Burst");
    this->addSkill("Cure");

    direction = 1; // E

    //STATS
        //HP
    HP = 13400;
    //MP
    MP = 0;
    //HP regen
    HP_regen = 0;
    //MP regen
    MP_regen = 0;
    //Armor
    baseArmor = 112;
    //AD
    baseDamage = 250;
    //MS
    MS = 0;
    //AR
    attackRange = 50;
    //AS
    attackSpeed = 0.75;
    //Exp gain
    expGain = 800;

    //STATS BONUS
    HP_bonus = 1340;
    MP_bonus = 0;
    HP_regen_bonus = 0;
    MP_regen_bonus = 0;
    Armor_bonus = 11.2;
    AD_bonus = 23.5;
    MS_bonus = 0;
    AR_bonus = 0;
    AS_bonus = 0;
    expGain_bonus = 45.7;

    currentHP = HP;

    setAttackSpeed(attackSpeed);

    // Detect Player Range of Boss
    detectionRadius = 250;

    // CurrentCombo
    currentCombo = 1;

    isTargetInAttackRange = false;

    // Init Boss's Sprite
    characterCurrentSprite = Sprite::createWithSpriteFrameName("okyanus_idle_E (1).png");

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

    //// Upate Detect Target Player
    schedule(CC_SCHEDULE_SELECTOR(Okyanus::updateBossSkills), 5.0f);
    return true;
}

void Okyanus::setAttackSpeed(float attackSpeed) {
    int numSprite = 20;
    float delay = (1 / attackSpeed) / numSprite;
    // Attack1
    attack1_E = Animate::create(Engine::getInstance()->createAnimation2("okyanus_attack1_E", numSprite, delay));

    attack1_NE = Animate::create(Engine::getInstance()->createAnimation2("okyanus_attack1_NE", numSprite, delay));

    attack1_N = Animate::create(Engine::getInstance()->createAnimation2("okyanus_attack1_N", numSprite, delay));

    attack1_NW = Animate::create(Engine::getInstance()->createAnimation2("okyanus_attack1_NW", numSprite, delay));

    attack1_W = Animate::create(Engine::getInstance()->createAnimation2("okyanus_attack1_W", numSprite, delay));

    attack1_SW = Animate::create(Engine::getInstance()->createAnimation2("okyanus_attack1_SW", numSprite, delay));

    attack1_S = Animate::create(Engine::getInstance()->createAnimation2("okyanus_attack1_S", numSprite, delay));

    attack1_SE = Animate::create(Engine::getInstance()->createAnimation2("okyanus_attack1_SE", numSprite, delay));


    // Attack2
    attack2_E = Animate::create(Engine::getInstance()->createAnimation2("okyanus_attack2_E", numSprite, delay));

    attack2_NE = Animate::create(Engine::getInstance()->createAnimation2("okyanus_attack2_NE", numSprite, delay));

    attack2_N = Animate::create(Engine::getInstance()->createAnimation2("okyanus_attack2_N", numSprite, delay));

    attack2_NW = Animate::create(Engine::getInstance()->createAnimation2("okyanus_attack2_NW", numSprite, delay));

    attack2_W = Animate::create(Engine::getInstance()->createAnimation2("okyanus_attack2_W", numSprite, delay));

    attack2_SW = Animate::create(Engine::getInstance()->createAnimation2("okyanus_attack2_SW", numSprite, delay));

    attack2_S = Animate::create(Engine::getInstance()->createAnimation2("okyanus_attack2_S", numSprite, delay));

    attack2_SE = Animate::create(Engine::getInstance()->createAnimation2("okyanus_attack2_SE", numSprite, delay));


    // Attack3
    attack3_E = Animate::create(Engine::getInstance()->createAnimation2("okyanus_attack3_E", numSprite, delay));

    attack3_NE = Animate::create(Engine::getInstance()->createAnimation2("okyanus_attack3_NE", numSprite, delay));

    attack3_N = Animate::create(Engine::getInstance()->createAnimation2("okyanus_attack3_N", numSprite, delay));

    attack3_NW = Animate::create(Engine::getInstance()->createAnimation2("okyanus_attack3_NW", numSprite, delay));

    attack3_W = Animate::create(Engine::getInstance()->createAnimation2("okyanus_attack3_W", numSprite, delay));

    attack3_SW = Animate::create(Engine::getInstance()->createAnimation2("okyanus_attack3_SW", numSprite, delay));

    attack3_S = Animate::create(Engine::getInstance()->createAnimation2("okyanus_attack3_S", numSprite, delay));

    attack3_SE = Animate::create(Engine::getInstance()->createAnimation2("okyanus_attack3_SE", numSprite, delay));

    // Attack1
    attack1_E->retain();   // 1

    attack1_NE->retain();  // 3

    attack1_N->retain();   // 5

    attack1_NW->retain();  // 7

    attack1_W->retain();   // 9

    attack1_SW->retain();  // 11

    attack1_S->retain();   // 13

    attack1_SE->retain();  // 15


    // Attack2
    attack2_E->retain();   // 2

    attack2_NE->retain();  // 3

    attack2_N->retain();   // 5

    attack2_NW->retain();  // 7

    attack2_W->retain();   // 9

    attack2_SW->retain();  // 22

    attack2_S->retain();   // 23

    attack2_SE->retain();  // 25


    // Attack3
    attack3_E->retain();   // 3

    attack3_NE->retain();  // 3

    attack3_N->retain();   // 5

    attack3_NW->retain();  // 7

    attack3_W->retain();   // 9

    attack3_SW->retain();  // 33

    attack3_S->retain();   // 33

    attack3_SE->retain();  // 35
}

void Okyanus::loadAnimate()
{   

    // Death
    death_E = Animate::create(Engine::getInstance()->createAnimation2("okyanus_death_E", 24, 0.05));

    death_NE = Animate::create(Engine::getInstance()->createAnimation2("okyanus_death_NE", 24, 0.05));

    death_N = Animate::create(Engine::getInstance()->createAnimation2("okyanus_death_N", 24, 0.05));

    death_NW = Animate::create(Engine::getInstance()->createAnimation2("okyanus_death_NW", 24, 0.05));

    death_W = Animate::create(Engine::getInstance()->createAnimation2("okyanus_death_W", 24, 0.05));

    death_SW = Animate::create(Engine::getInstance()->createAnimation2("okyanus_death_SW", 24, 0.05));

    death_S = Animate::create(Engine::getInstance()->createAnimation2("okyanus_death_S", 24, 0.05));

    death_SE = Animate::create(Engine::getInstance()->createAnimation2("okyanus_death_SE", 24, 0.05));


    // Get Hit
    getHit_E = Animate::create(Engine::getInstance()->createAnimation2("okyanus_hit_E", 16, 0.05));
    getHit_NE = Animate::create(Engine::getInstance()->createAnimation2("okyanus_hit_NE", 16, 0.05));
    getHit_N = Animate::create(Engine::getInstance()->createAnimation2("okyanus_hit_N", 16, 0.05));
    getHit_NW = Animate::create(Engine::getInstance()->createAnimation2("okyanus_hit_NW", 16, 0.05));
    getHit_W = Animate::create(Engine::getInstance()->createAnimation2("okyanus_hit_W", 16, 0.05));
    getHit_SW = Animate::create(Engine::getInstance()->createAnimation2("okyanus_hit_SW", 16, 0.05));
    getHit_S = Animate::create(Engine::getInstance()->createAnimation2("okyanus_hit_S", 16, 0.05));
    getHit_SE = Animate::create(Engine::getInstance()->createAnimation2("okyanus_hit_SE", 16, 0.05));

    // Angry
    angry_E = Animate::create(Engine::getInstance()->createAnimation2("okyanus_angry_E", 24, 0.05));
    
    angry_NE = Animate::create(Engine::getInstance()->createAnimation2("okyanus_angry_NE", 24, 0.05));
    
    angry_N = Animate::create(Engine::getInstance()->createAnimation2("okyanus_angry_N", 24, 0.05));
    
    angry_NW = Animate::create(Engine::getInstance()->createAnimation2("okyanus_angry_NW", 24, 0.05));
    
    angry_W = Animate::create(Engine::getInstance()->createAnimation2("okyanus_angry_W", 24, 0.05));
    
    angry_SW = Animate::create(Engine::getInstance()->createAnimation2("okyanus_angry_SW", 24, 0.05));
    
    angry_S = Animate::create(Engine::getInstance()->createAnimation2("okyanus_angry_S", 24, 0.05));
    
    angry_SE = Animate::create(Engine::getInstance()->createAnimation2("okyanus_angry_SE", 24, 0.05));
    

    // Idle
    idle_E = Animate::create(Engine::getInstance()->createAnimation2("okyanus_idle_E", 16, 0.05));
    
    idle_NE = Animate::create(Engine::getInstance()->createAnimation2("okyanus_idle_NE", 16, 0.05));
    
    idle_N = Animate::create(Engine::getInstance()->createAnimation2("okyanus_idle_N", 16, 0.05));
    
    idle_NW = Animate::create(Engine::getInstance()->createAnimation2("okyanus_idle_NW", 16, 0.05));
    
    idle_W = Animate::create(Engine::getInstance()->createAnimation2("okyanus_idle_W", 16, 0.05));
    
    idle_SW = Animate::create(Engine::getInstance()->createAnimation2("okyanus_idle_SW", 16, 0.05));
    
    idle_S = Animate::create(Engine::getInstance()->createAnimation2("okyanus_idle_S", 16, 0.05));
    
    idle_SE = Animate::create(Engine::getInstance()->createAnimation2("okyanus_idle_SE", 16, 0.05));
   
    

    // Death
    death_E->retain();   // 1
 
    death_NE->retain();  // 3
 
    death_N->retain();   // 5
 
    death_NW->retain();  // 7
 
    death_W->retain();   // 9
  
    death_SW->retain();  // 11
  
    death_S->retain();   // 13
  
    death_SE->retain();  // 15
  

    // Get Hit
    getHit_E->retain();   // 1
  
    getHit_NE->retain();  // 3
  
    getHit_N->retain();   // 5
  
    getHit_NW->retain();  // 7
  
    getHit_W->retain();   // 9
   
    getHit_SW->retain();  // 11
   
    getHit_S->retain();   // 13
   
    getHit_SE->retain();  // 15
   

    // angry
    angry_E->retain();   // 1
 
    angry_NE->retain();  // 3
 
    angry_N->retain();   // 5
 
    angry_NW->retain();  // 7
 
    angry_W->retain();   // 9
  
    angry_SW->retain();  // 11
  
    angry_S->retain();   // 13
  
    angry_SE->retain();  // 15
  

    // Idle
    idle_E->retain();   // 1

    idle_NE->retain();  // 3

    idle_N->retain();   // 5

    idle_NW->retain();  // 7

    idle_W->retain();   // 9
 
    idle_SW->retain();  // 11
 
    idle_S->retain();   // 13
 
    idle_SE->retain();  // 15
 
}