#include "Angelos.h"

bool Angelos::init()
{
    direction = 1;

    //STATS
       //HP
    HP = 477;
    currentHP = HP;
    //MP
    MP = 0;
    //HP regen
    HP_regen = 0;
    //MP regen
    MP_regen = 0;
    //Armor
    baseArmor = 10;
    //AD
    baseDamage = 20;
    //MS
    MS = 65;
    //AR
    attackRange = 40;
    //AS
    attackSpeed = 1.2;
    //Exp gain
    expGain = 60.45;

    //STATS BONUS
    HP_bonus = 45.7;
    MP_bonus = 0;
    HP_regen_bonus = 0;
    MP_regen_bonus = 0;
    Armor_bonus = 0.87;
    AD_bonus = 2.8;
    MS_bonus = 0.1;
    AR_bonus = 1;
    AS_bonus = 1;
    expGain_bonus = 5.64;

    setAttackSpeed(attackSpeed);
    //Detect Player Range of Boss
    detectionRadius = 200;

    //Load Animate
    this->loadAnimate();

    //
    isTargetInAttackRange = false;
    characterCurrentSprite = Sprite::createWithSpriteFrameName("angelos_idle_E (1).png");

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
    auto monsterName = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 7);
    monsterName->setColor(Color3B::WHITE);
    monsterName->setPosition(Vec2(0, 50));
    this->addChild(monsterName);
    monsterName->setString(StringUtils::format("%s", "Angelos"));

    registerObserver(QuestManager::getInstance());
    //Init physics
    auto physicsBody = PhysicsBody::createBox(Size(20, 20), PhysicsMaterial(1.0f, 0.0f, 0.0f));
    physicsBody->setGravityEnable(false);
    physicsBody->setRotationEnable(false);
    physicsBody->setContactTestBitmask(0x01);
    this->setPhysicsBody(physicsBody);
    this->addChild(characterCurrentSprite);

    this->idleState = new NormalMonsterIdle(this);
    this->attackState = new NormalMonsterAttack(this);
    this->walkState = new NormalMonsterWalk(this);
    this->deadState = new NormalMonsterDead(this);

	return true;
}

void Angelos::loadAnimate()
{
    //Default Walk
    defaultWalk_E = Animate::create(Engine::getInstance()->createAnimation2("angelos_walk_E", 8, 0.05)); //
    defaultWalk_NE = Animate::create(Engine::getInstance()->createAnimation2("angelos_walk_NE", 8, 0.05)); //
    defaultWalk_N = Animate::create(Engine::getInstance()->createAnimation2("angelos_walk_N", 8, 0.05)); //5
    defaultWalk_NW = Animate::create(Engine::getInstance()->createAnimation2("angelos_walk_NW", 8, 0.05)); //7
    defaultWalk_W = Animate::create(Engine::getInstance()->createAnimation2("angelos_walk_W", 8, 0.05)); //9
    defaultWalk_SW = Animate::create(Engine::getInstance()->createAnimation2("angelos_walk_SW", 8, 0.05)); //11
    defaultWalk_S = Animate::create(Engine::getInstance()->createAnimation2("angelos_walk_S", 8, 0.05)); //13
    defaultWalk_SE = Animate::create(Engine::getInstance()->createAnimation2("angelos_walk_SE", 8, 0.05)); //15


    //Default Idle
    defaultIdle_E = Animate::create(Engine::getInstance()->createAnimation2("angelos_idle_E", 3, 0.1)); //
    defaultIdle_NE = Animate::create(Engine::getInstance()->createAnimation2("angelos_idle_NE", 3, 0.1)); //
    defaultIdle_N = Animate::create(Engine::getInstance()->createAnimation2("angelos_idle_N", 3, 0.1)); //5
    defaultIdle_NW = Animate::create(Engine::getInstance()->createAnimation2("angelos_idle_NW", 3, 0.1)); //7
    defaultIdle_W = Animate::create(Engine::getInstance()->createAnimation2("angelos_idle_W", 3, 0.1)); //9
    defaultIdle_SW = Animate::create(Engine::getInstance()->createAnimation2("angelos_idle_SW", 3, 0.1)); //11
    defaultIdle_S = Animate::create(Engine::getInstance()->createAnimation2("angelos_idle_S", 3, 0.1)); //13
    defaultIdle_SE = Animate::create(Engine::getInstance()->createAnimation2("angelos_idle_SE", 3, 0.1)); //15


    //Default Dead
    specialDeath_E = Animate::create(Engine::getInstance()->createAnimation1("angelos_death_E", 0, 0.1)); //1
    specialDeath_NE = Animate::create(Engine::getInstance()->createAnimation1("angelos_death_NE", 0, 0.1)); //3
    specialDeath_N = Animate::create(Engine::getInstance()->createAnimation1("angelos_death_N", 0, 0.1)); //5
    specialDeath_NW = Animate::create(Engine::getInstance()->createAnimation1("angelos_death_NW", 0, 0.1)); //7
    specialDeath_W = Animate::create(Engine::getInstance()->createAnimation1("angelos_death_W", 0, 0.1)); //9
    specialDeath_SW = Animate::create(Engine::getInstance()->createAnimation1("angelos_death_SW", 0, 0.1)); //11
    specialDeath_S = Animate::create(Engine::getInstance()->createAnimation1("angelos_death_S", 0, 0.1)); //13
    specialDeath_SE = Animate::create(Engine::getInstance()->createAnimation1("angelos_death_SE", 0, 0.1)); //15


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

void Angelos::setAttackSpeed(float attackSpeed)
{
    int numSprite = 3;
    float delay = 0.18;
    defaultAttack_E = Animate::create(Engine::getInstance()->createAnimation2("angelos_attack_E", numSprite, delay)); //1

    defaultAttack_NE = Animate::create(Engine::getInstance()->createAnimation2("angelos_attack_NE", numSprite, delay)); //3

    defaultAttack_N = Animate::create(Engine::getInstance()->createAnimation2("angelos_attack_N", numSprite, delay)); //5

    defaultAttack_NW = Animate::create(Engine::getInstance()->createAnimation2("angelos_attack_NW", numSprite, delay)); //7

    defaultAttack_W = Animate::create(Engine::getInstance()->createAnimation2("angelos_attack_W", numSprite, delay)); //9

    defaultAttack_SW = Animate::create(Engine::getInstance()->createAnimation2("angelos_attack_SW", numSprite, delay)); //11

    defaultAttack_S = Animate::create(Engine::getInstance()->createAnimation2("angelos_attack_S", numSprite, delay)); //13

    defaultAttack_SE = Animate::create(Engine::getInstance()->createAnimation2("angelos_attack_SE", numSprite, delay)); //15

    //Default Attack
    defaultAttack_E->retain(); //1

    defaultAttack_NE->retain(); //3

    defaultAttack_N->retain(); //5

    defaultAttack_NW->retain(); //7

    defaultAttack_W->retain(); //9

    defaultAttack_SW->retain(); //11

    defaultAttack_S->retain(); //13

    defaultAttack_SE->retain(); //15
}
