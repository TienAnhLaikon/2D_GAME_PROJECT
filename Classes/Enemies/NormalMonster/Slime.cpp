#include "Slime.h"
#include "Engine/Engine.h"

bool Slime::init() {
    AAcanDealDamage = false;
    //Direction
    direction = 1;

    //STATS
   //HP
    HP = 725;
    currentHP = HP;
    //MP
    MP = 0;
    //HP regen
    HP_regen = 0;
    //MP regen
    MP_regen = 0;
    //Armor
    baseArmor = 30;
    //AD
    baseDamage = 0;
    //MS
    MS = 60;
    //AR
    attackRange = 0;
    //AS
    attackSpeed = 0;
    //Exp gain
    expGain = 120.5;

    //STATS BONUS
    HP_bonus = 0;
    MP_bonus = 0;
    HP_regen_bonus = 0;
    MP_regen_bonus = 0;
    Armor_bonus = 0;
    AD_bonus = 0;
    MS_bonus = 0;
    AR_bonus = 0;
    AS_bonus = 0;

    this->loadAnimate();
    //Detect Player Range of Boss
    detectionRadius = 100;

    isTargetInAttackRange = false;
    characterCurrentSprite = Sprite::createWithSpriteFrameName("slime_default_idle_E_0.0_0.png");

    Size size = Director::getInstance()->getVisibleSize();
    float scale = size.width / size.height;
    //// Khi to Background cho thanh mu
    backGroundBar = Sprite::create("res/background_bar.jpg");
    backGroundBar->setScale(1 / (scale * 8.0f), 1 / (scale * 2.0f));
    /*backGroundBar->setContentSize(Size(characterCurrentSprite->getBoundingBox().size.width / scale, 30 / scale));*/
    backGroundBar->setPosition(Vec2(0, 40));
    //backGroundBar->setScale(0.2);
    this->addChild(backGroundBar);

    // Khi to thanh mu
    healthBar = ui::LoadingBar::create("res/health_bar.jpg");
    healthBar->setDirection(ui::LoadingBar::Direction::LEFT); // t hng fill cho thanh mu
    healthBar->setPercent(100); // Gi tr ban u ca mu
    healthBar->setScale(1 / (scale * 8.0f), 1 / (scale * 2.0f));
    /*healthBar->setContentSize(Size(characterCurrentSprite->getBoundingBox().size.width / scale, 30 / scale));*/
    healthBar->setPosition(Vec2(0, 40));
    //healthBar->setScale(0.2);
    this->addChild(healthBar, 2);

    //Lable Name
    monsterName = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 7);
    monsterName->setColor(Color3B::GREEN);
    monsterName->setPosition(Vec2(0, 50));
    this->addChild(monsterName);
    monsterName->setString(StringUtils::format("%s", "Slime"));

    registerObserver(QuestManager::getInstance());

    //Init physics
    auto physicsBody = PhysicsBody::createBox(Size(20, 20), PhysicsMaterial(1.0f, 0.0f, 0.0f));
    physicsBody->setGravityEnable(false);
    physicsBody->setRotationEnable(false);
    physicsBody->setContactTestBitmask(0x01);
    this->setPhysicsBody(physicsBody);
    this->addChild(characterCurrentSprite);


    this->idleState = new NormalMonsterIdle(this);
    this->walkState = new NormalMonsterWalk(this);
    this->deadState = new NormalMonsterDead(this);

    return true;
}

void Slime::loadAnimate() {
    //Default Walk
    defaultWalk_E = Animate::create(Engine::getInstance()->createAnimation1("slime_default_walk_E_0.0_", 8, 0.1)); //1
  
    defaultWalk_NE = Animate::create(Engine::getInstance()->createAnimation1("slime_default_walk_NE_45.0_", 8, 0.1)); //3
  
    defaultWalk_N = Animate::create(Engine::getInstance()->createAnimation1("slime_default_walk_N_90.0_", 8, 0.1)); //5
   
    defaultWalk_NW = Animate::create(Engine::getInstance()->createAnimation1("slime_default_walk_NW_135.0_", 8, 0.1)); //7
   
    defaultWalk_W = Animate::create(Engine::getInstance()->createAnimation1("slime_default_walk_W_180.0_", 8, 0.1)); //9
    
    defaultWalk_SW = Animate::create(Engine::getInstance()->createAnimation1("slime_default_walk_SW_225.0_", 8, 0.1)); //11
    
    defaultWalk_S = Animate::create(Engine::getInstance()->createAnimation1("slime_default_walk_S_270.0_", 8, 0.1)); //13
    
    defaultWalk_SE = Animate::create(Engine::getInstance()->createAnimation1("slime_default_walk_SE_315.0_", 8, 0.1)); //15
    

    //Default Idle
    defaultIdle_E = Animate::create(Engine::getInstance()->createAnimation1("slime_default_idle_E_0.0_", 1, 0.1)); //1
  
    defaultIdle_NE = Animate::create(Engine::getInstance()->createAnimation1("slime_default_idle_NE_45.0_", 1, 0.1)); //3
  
    defaultIdle_N = Animate::create(Engine::getInstance()->createAnimation1("slime_default_idle_N_90.0_", 1, 0.1)); //5
   
    defaultIdle_NW = Animate::create(Engine::getInstance()->createAnimation1("slime_default_idle_NW_135.0_", 1, 0.1)); //7
   
    defaultIdle_W = Animate::create(Engine::getInstance()->createAnimation1("slime_default_idle_W_180.0_", 1, 0.1)); //9
    
    defaultIdle_SW = Animate::create(Engine::getInstance()->createAnimation1("slime_default_idle_SW_225.0_", 1, 0.1)); //11
    
    defaultIdle_S = Animate::create(Engine::getInstance()->createAnimation1("slime_default_idle_S_270.0_", 1, 0.1)); //13
    
    defaultIdle_SE = Animate::create(Engine::getInstance()->createAnimation1("slime_default_idle_SE_315.0_", 1, 0.1)); //15
    

    //special Dead
    specialDeath_E = Animate::create(Engine::getInstance()->createAnimation1("slime_special_death_E_0.0_", 7, 0.1)); //1
    
    specialDeath_NE = Animate::create(Engine::getInstance()->createAnimation1("slime_special_death_NE_45.0_", 7, 0.1)); //3
    
    specialDeath_N = Animate::create(Engine::getInstance()->createAnimation1("slime_special_death_N_90.0_", 7, 0.1)); //5
    
    specialDeath_NW = Animate::create(Engine::getInstance()->createAnimation1("slime_special_death_NW_135.0_", 7, 0.1)); //7
    
    specialDeath_W = Animate::create(Engine::getInstance()->createAnimation1("slime_special_death_W_180.0_", 7, 0.1)); //9
    
    specialDeath_SW = Animate::create(Engine::getInstance()->createAnimation1("slime_special_death_SW_225.0_", 7, 0.1)); //11
    
    specialDeath_S = Animate::create(Engine::getInstance()->createAnimation1("slime_special_death_S_270.0_", 7, 0.1)); //13
    
    specialDeath_SE = Animate::create(Engine::getInstance()->createAnimation1("slime_special_death_SE_315.0_", 7, 0.1)); //15
    

    //Default Walk
    defaultWalk_E->retain(); //1
   
    defaultWalk_NE->retain(); //3
   
    defaultWalk_N->retain(); //5
   
    defaultWalk_NW->retain(); //7
   
    defaultWalk_W->retain(); //9
    
    defaultWalk_SW->retain(); //11
    
    defaultWalk_S->retain(); //13
    
    defaultWalk_SE->retain(); //15
    

    //Default Idle
    defaultIdle_E->retain(); //1

    defaultIdle_NE->retain(); //3

    defaultIdle_N->retain(); //5

    defaultIdle_NW->retain(); //7

    defaultIdle_W->retain(); //9
 
    defaultIdle_SW->retain(); //11
 
    defaultIdle_S->retain(); //13
 
    defaultIdle_SE->retain(); //15
 

    //Default Dead
    specialDeath_E->retain(); //1
   
    specialDeath_NE->retain(); //3
   
    specialDeath_N->retain(); //5
   
    specialDeath_NW->retain(); //7
   
    specialDeath_W->retain(); //9
    
    specialDeath_SW->retain(); //11
    
    specialDeath_S->retain(); //13
    
    specialDeath_SE->retain(); //15
    
}