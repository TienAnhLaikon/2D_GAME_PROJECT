#include "HellBeast.h"
#include "Engine/Engine.h"

bool HellBeast::init() {
    AAcanDealDamage = false;
    //Load All Animate
    this->loadAnimate();
    //Init All States
    this->attackState = new BossAttack(this);
    this->idleState = new BossIdle(this);
    this->roarState = new BossRoar(this);
    this->runState = new BossRun(this);
    this->walkState = new BossWalk(this);
    this->getHitState = new BossGetHit(this);
    this->deadState = new BossDead(this);
    //Boss Skills
    //this->addSkill("Summon");
    this->addSkill("Unleash");
    ////this->addSkill("BombRainbow");
    this->addSkill("MultiFireBall");
    this->addSkill("Fortify");

    direction = 1;//E

    //STATS
        //HP
    HP = 5600;
    //MP
    MP = 0;
    //HP regen
    HP_regen = 0;
    //MP regen
    MP_regen = 0;
    //Armor
    baseArmor = 120;
    //AD
    baseDamage = 100;
    //MS
    MS = 80;
    //AR
    attackRange = 50;
    //AS
    attackSpeed = 1.2;
    //Exp gain
    expGain = 770;

    //STATS BONUS
    HP_bonus = 1.1;
    MP_bonus = 0;
    HP_regen_bonus = 0;
    MP_regen_bonus = 0;
    Armor_bonus = 1.1;
    AD_bonus = 1.1;
    MS_bonus = 0;
    AR_bonus = 0;
    AS_bonus = 0;
    expGain_bonus = 1.35;

    currentHP = HP;

    direction = 7;
    setAttackSpeed(attackSpeed);
    //SKILLS
    //
    //Detect Player Range of Boss
    detectionRadius = 250;

    //Cool Down Period between skill uses
    cooldownPeriod = 5.0f;

    cooldownRoar = 5.0f;

    //CurrentCombo
    currentCombo = 1;

    isTargetInAttackRange = false;

    //Init Boss's Sprite
    characterCurrentSprite = Sprite::createWithSpriteFrameName("hellbeast_attack1_E (1).png");

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

    //Lable Name
    monsterName = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 10);
    monsterName->setColor(Color3B::RED);
    monsterName->setPosition(Vec2(0, 70));
    this->addChild(monsterName);
    monsterName->setString(StringUtils::format("%s", "Terra"));

    registerObserver(QuestManager::getInstance());
    //Init physics
    auto physicsBody = PhysicsBody::createBox(Size(20, 20), PhysicsMaterial(1.0f, 0.0f, 0.0f));
    physicsBody->setGravityEnable(false);
    physicsBody->setRotationEnable(false);
    physicsBody->setDynamic(false);
    this->setPhysicsBody(physicsBody);
    this->addChild(characterCurrentSprite);

    schedule(CC_SCHEDULE_SELECTOR(HellBeast::updateBossSkills), 5.0f);
    return true;
}

void HellBeast::setAttackSpeed(float attackSpeed) {
    int numSprite = 20;
    float delay = (1 / attackSpeed) / numSprite;
    //Attack1
    attack1_E = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_attack1_E", numSprite, delay));

    attack1_NE = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_attack1_NE", numSprite, delay));

    attack1_N = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_attack1_N", numSprite, delay));

    attack1_NW = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_attack1_NW", numSprite, delay));

    attack1_W = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_attack1_W", numSprite, delay));

    attack1_SW = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_attack1_SW", numSprite, delay));

    attack1_S = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_attack1_S", numSprite, delay));

    attack1_SE = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_attack1_SE", numSprite, delay));


    //Attack2
    attack2_E = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_attack2_E", numSprite, delay));

    attack2_NE = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_attack2_NE", numSprite, delay));

    attack2_N = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_attack2_N", numSprite, delay));

    attack2_NW = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_attack2_NW", numSprite, delay));

    attack2_W = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_attack2_W", numSprite, delay));

    attack2_SW = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_attack2_SW", numSprite, delay));

    attack2_S = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_attack2_S", numSprite, delay));

    attack2_SE = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_attack2_SE", numSprite, delay));


    //Attack3
    attack3_E = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_attack3_E", numSprite, delay));

    attack3_NE = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_attack3_NE", numSprite, delay));

    attack3_N = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_attack3_N", numSprite, delay));

    attack3_NW = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_attack3_NW", numSprite, delay));

    attack3_W = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_attack3_W", numSprite, delay));

    attack3_SW = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_attack3_SW", numSprite, delay));

    attack3_S = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_attack3_S", numSprite, delay));

    attack3_SE = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_attack3_SE", numSprite, delay));

    //Attack1
    attack1_E->retain(); //1

    attack1_NE->retain(); //3

    attack1_N->retain(); //5

    attack1_NW->retain(); //7

    attack1_W->retain(); //9

    attack1_SW->retain(); //11

    attack1_S->retain(); //13

    attack1_SE->retain(); //15


    //Attack2
    attack2_E->retain(); //2

    attack2_NE->retain(); //3

    attack2_N->retain(); //5

    attack2_NW->retain(); //7

    attack2_W->retain(); //9

    attack2_SW->retain(); //22

    attack2_S->retain(); //23

    attack2_SE->retain(); //25


    //Attack3
    attack3_E->retain(); //3

    attack3_NE->retain(); //3

    attack3_N->retain(); //5

    attack3_NW->retain(); //7

    attack3_W->retain(); //9

    attack3_SW->retain(); //33

    attack3_S->retain(); //33

    attack3_SE->retain(); //35
}

void HellBeast::loadAnimate() {
    
    

    //Death
    death_E = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_death_E", 30, 0.05));
    
    death_NE = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_death_NE", 30, 0.05));
    
    death_N = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_death_N", 30, 0.05));
    
    death_NW = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_death_NW", 30, 0.05));
    
    death_W = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_death_W", 30, 0.05));
    
    death_SW = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_death_SW", 30, 0.05));
    
    death_S = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_death_S", 30, 0.05));
    
    death_SE = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_death_SE", 30, 0.05));
    

    //Get Hit
    getHit_E = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_hit_E", 16, 0.05));
   
    getHit_NE = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_hit_NE", 16, 0.05));
   
    getHit_N = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_hit_N", 16, 0.05));
   
    getHit_NW = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_hit_NW", 16, 0.05));
   
    getHit_W = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_hit_W", 16, 0.05));
   
    getHit_SW = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_hit_SW", 16, 0.05));
   
    getHit_S = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_hit_S", 16, 0.05));
   
    getHit_SE = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_hit_SE", 16, 0.05));
   

    //Run
    run_E = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_run_E", 20, 0.035));
   
    run_NE = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_run_NE", 20, 0.035));
   
    run_N = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_run_N", 20, 0.035));
   
    run_NW = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_run_NW", 20, 0.035));
   
    run_W = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_run_W", 20, 0.035));
   
    run_SW = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_run_SW", 20, 0.035));
   
    run_S = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_run_S", 20, 0.035));
   
    run_SE = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_run_SE", 20, 0.035));
   

    //Walk
    walk_E = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_walk_E", 20, 0.05));
    
    walk_NE = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_walk_NE", 20, 0.05));
    
    walk_N = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_walk_N", 20, 0.05));
    
    walk_NW = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_walk_NW", 20, 0.05));
    
    walk_W = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_walk_W", 20, 0.05));
    
    walk_SW = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_walk_SW", 20, 0.05));
    
    walk_S = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_walk_S", 20, 0.05));
    
    walk_SE = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_walk_SE", 20, 0.05));
    

    //Roar
    roar_E = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_roar_E", 40, 0.05));
   
    roar_NE = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_roar_NE", 40, 0.05));
   
    roar_N = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_roar_N", 40, 0.05));
   
    roar_NW = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_roar_NW", 40, 0.05));
   
    roar_W = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_roar_W", 40, 0.05));
   
    roar_SW = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_roar_SW", 40, 0.05));
   
    roar_S = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_roar_S", 40, 0.05));
   
    roar_SE = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_roar_SE", 40, 0.05));
   

    //Idle
    idle_E = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_idle_E", 24, 0.05));
   
    idle_NE = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_idle_NE", 24, 0.05));
   
    idle_N = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_idle_N", 24, 0.05));
   
    idle_NW = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_idle_NW", 24, 0.05));
   
    idle_W = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_idle_W", 24, 0.05));
   
    idle_SW = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_idle_SW", 24, 0.05));
   
    idle_S = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_idle_S", 24, 0.05));
   
    idle_SE = Animate::create(Engine::getInstance()->createAnimation2("hellbeast_idle_SE", 24, 0.05));
   
    

    //Death
    death_E->retain(); //1
 
    death_NE->retain(); //3
 
    death_N->retain(); //5
 
    death_NW->retain(); //7
 
    death_W->retain(); //9
  
    death_SW->retain(); //11
  
    death_S->retain(); //13
  
    death_SE->retain(); //15
  

    //Get Hit
    getHit_E->retain(); //1
    
    getHit_NE->retain();
   
    getHit_N->retain(); //5
   
    getHit_NW->retain(); //7
   
    getHit_W->retain(); //9
    
    getHit_SW->retain(); //11
    
    getHit_S->retain(); //13
    
    getHit_SE->retain(); //15
    

    //Run
    run_E->retain(); //1

    run_NE->retain(); //3

    run_N->retain(); //5

    run_NW->retain(); //7

    run_W->retain(); //9
 
    run_SW->retain(); //11
 
    run_S->retain(); //13
 
    run_SE->retain(); //15
 

    //Walk
    walk_E->retain(); //1
 
    walk_NE->retain(); //3
 
    walk_N->retain(); //5
 
    walk_NW->retain(); //7
 
    walk_W->retain(); //9
  
    walk_SW->retain(); //11
  
    walk_S->retain(); //13
  
    walk_SE->retain(); //15
  

    //Roar
    roar_E->retain(); //1
 
    roar_NE->retain(); //3
 
    roar_N->retain(); //5
 
    roar_NW->retain(); //7
 
    roar_W->retain(); //9
  
    roar_SW->retain(); //11
  
    roar_S->retain(); //13
  
    roar_SE->retain(); //15
  

    //Idle
    idle_E->retain(); //1
 
    idle_NE->retain(); //3
 
    idle_N->retain(); //5
 
    idle_NW->retain(); //7
 
    idle_W->retain(); //9
  
    idle_SW->retain(); //11
  
    idle_S->retain(); //13
  
    idle_SE->retain(); //15
  
}