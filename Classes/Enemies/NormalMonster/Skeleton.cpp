#include "./Skeleton.h"
#include "../../Engine/Engine.h"
bool Skeleton::init() {
    AAcanDealDamage = false;
     direction = 1;
     enemyName = "Skeleton";
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
     MS = Director::getInstance()->getContentScaleFactor()*400;
     //AR
     attackRange = 20;
     //AS
     attackSpeed = 1.0;
     //Exp gain
     expGain = 60.45;

     //STATS BONUS
     HP_bonus = 1.1;
     MP_bonus = 1;
     HP_regen_bonus = 1;
     MP_regen_bonus = 1;
     Armor_bonus = 1.2;
     AD_bonus = 1.2;
     MS_bonus = 1;
     AR_bonus = 1;
     AS_bonus = 1;
     expGain_bonus = 1.5;

     setAttackSpeed(attackSpeed);
     //Detect Player Range of Boss
     detectionRadius = 200;

     //Load Animate
     this->loadAnimate();

     //
     isTargetInAttackRange = false;
     characterCurrentSprite = Sprite::createWithSpriteFrameName("skeleton_default_idle_E_0.0_0.png");

     Size size = Director::getInstance()->getVisibleSize();
     float scale = size.width / size.height;
     //// Khi to Background cho thanh mu
     backGroundBar = Sprite::create("res/background_bar.jpg");
     backGroundBar->setScale(1 / (scale * 8.0f), 1/(scale*2.0f));
     /*backGroundBar->setContentSize(Size(characterCurrentSprite->getBoundingBox().size.width / scale, 30 / scale));*/
     backGroundBar->setPosition(Vec2(0, 30));
     //backGroundBar->setScale(0.2);
     this->addChild(backGroundBar);

     // Khi to thanh mu
     healthBar = ui::LoadingBar::create("res/health_bar.jpg");
     healthBar->setDirection(ui::LoadingBar::Direction::LEFT); // t hng fill cho thanh mu
     healthBar->setPercent(100); // Gi tr ban u ca mu
     healthBar->setScale(1 / (scale * 8.0f), 1 / (scale * 2.0f));
     /*healthBar->setContentSize(Size(characterCurrentSprite->getBoundingBox().size.width / scale, 30 / scale));*/
     healthBar->setPosition(Vec2(0, 30));
     //healthBar->setScale(0.2);
     this->addChild(healthBar, 2);

     //Lable Name
     monsterName = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 7);
     monsterName->setColor(Color3B::GRAY);
     monsterName->setPosition(Vec2(0, 37));
     this->addChild(monsterName);
     monsterName->setString(StringUtils::format("%s", "Skeleton"));

     //Level Sprite
     levelSprite = Sprite::create("res/ab2_bonus_frame_globe_overlay_effects.png");
     levelSprite->setScale(0.1);
     levelSprite->setPosition(Vec2(-17, 30));
     this->addChild(levelSprite);

     //Level Label
     levelLabel = Label::createWithTTF("2", "fonts/Diablo Light.ttf", 35);
     levelLabel->setString("2");
     levelLabel->setColor(Color3B::GRAY);
     levelLabel->setPosition(Vec2(levelSprite->getContentSize().width / 2, levelSprite->getContentSize().height / 2));
     levelSprite->addChild(levelLabel);

     registerObserver(QuestManager::getInstance());

     maxLevel = 10;
     //Init physics
     auto physicsBody = PhysicsBody::createBox(Size(characterCurrentSprite->getContentSize().width/3, characterCurrentSprite->getContentSize().height/2), PhysicsMaterial(1.0f, 0.0f, 0.0f));
     physicsBody->setGravityEnable(false);
     physicsBody->setRotationEnable(false);

     physicsBody->setCategoryBitmask(0x02); // Category của Enemy
     physicsBody->setCollisionBitmask(0x01); // Va chạm với FireBall
     physicsBody->setContactTestBitmask(0x01); // Kiểm tra va chạm với FireBall

     physicsBody->setPositionOffset(Vec2(0, 300));

     this->setPhysicsBody(physicsBody);

     this->addChild(characterCurrentSprite);

     this->idleState = new NormalMonsterIdle(this);
     this->attackState = new NormalMonsterAttack(this);
     this->walkState = new NormalMonsterWalk(this);
     this->deadState = new NormalMonsterDead(this);

     schedule(CC_SCHEDULE_SELECTOR(NormalMonster::updateEnemy), 0.00f);

	return true;
}

void Skeleton::setAttackSpeed(float attackSpeed) {
    int numSprite = 8;
    float delay = (1 / attackSpeed) / numSprite;
    defaultAttack_E = Animate::create(Engine::getInstance()->createAnimation1("skeleton_default_attack_E_0.0_", numSprite, delay)); //1

    defaultAttack_NE = Animate::create(Engine::getInstance()->createAnimation1("skeleton_default_attack_NE_45.0_", numSprite, delay)); //3

    defaultAttack_N = Animate::create(Engine::getInstance()->createAnimation1("skeleton_default_attack_N_90.0_", numSprite, delay)); //5

    defaultAttack_NW = Animate::create(Engine::getInstance()->createAnimation1("skeleton_default_attack_NW_135.0_", numSprite, delay)); //7

    defaultAttack_W = Animate::create(Engine::getInstance()->createAnimation1("skeleton_default_attack_W_180.0_", numSprite, delay)); //9

    defaultAttack_SW = Animate::create(Engine::getInstance()->createAnimation1("skeleton_default_attack_SW_225.0_", numSprite, delay)); //11

    defaultAttack_S = Animate::create(Engine::getInstance()->createAnimation1("skeleton_default_attack_S_270.0_", numSprite, delay)); //13

    defaultAttack_SE = Animate::create(Engine::getInstance()->createAnimation1("skeleton_default_attack_SE_315.0_", numSprite, delay)); //15

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
void Skeleton::loadAnimate(){
    //Default Walk
    defaultWalk_E = Animate::create(Engine::getInstance()->createAnimation1("skeleton_default_walk_E_0.0_", 8, 0.1)); //
    defaultWalk_NE = Animate::create(Engine::getInstance()->createAnimation1("skeleton_default_walk_NE_45.0_", 8, 0.1)); //
    defaultWalk_N = Animate::create(Engine::getInstance()->createAnimation1("skeleton_default_walk_N_90.0_", 8, 0.1)); //5
    defaultWalk_NW = Animate::create(Engine::getInstance()->createAnimation1("skeleton_default_walk_NW_135.0_", 8, 0.1)); //7
    defaultWalk_W = Animate::create(Engine::getInstance()->createAnimation1("skeleton_default_walk_W_180.0_", 8, 0.1)); //9

    defaultWalk_SW = Animate::create(Engine::getInstance()->createAnimation1("skeleton_default_walk_SW_225.0_", 8, 0.1)); //11

    defaultWalk_S = Animate::create(Engine::getInstance()->createAnimation1("skeleton_default_walk_S_270.0_", 8, 0.1)); //13

    defaultWalk_SE = Animate::create(Engine::getInstance()->createAnimation1("skeleton_default_walk_SE_315.0_", 8, 0.1)); //15
    

    //Default Idle
    defaultIdle_E = Animate::create(Engine::getInstance()->createAnimation1("skeleton_default_idle_E_0.0_", 1, 0.1)); //
    defaultIdle_NE = Animate::create(Engine::getInstance()->createAnimation1("skeleton_default_idle_NE_45.0_", 1, 0.1)); //
    defaultIdle_N = Animate::create(Engine::getInstance()->createAnimation1("skeleton_default_idle_N_90.0_", 1, 0.1)); //5
    defaultIdle_NW = Animate::create(Engine::getInstance()->createAnimation1("skeleton_default_idle_NW_135.0_", 1, 0.1)); //7
    defaultIdle_W = Animate::create(Engine::getInstance()->createAnimation1("skeleton_default_idle_W_180.0_", 1, 0.1)); //9

    defaultIdle_SW = Animate::create(Engine::getInstance()->createAnimation1("skeleton_default_idle_SW_225.0_", 1, 0.1)); //11

    defaultIdle_S = Animate::create(Engine::getInstance()->createAnimation1("skeleton_default_idle_S_270.0_", 1, 0.1)); //13

    defaultIdle_SE = Animate::create(Engine::getInstance()->createAnimation1("skeleton_default_idle_SE_315.0_", 1, 0.1)); //15


    //Default Dead
    specialDeath_E = Animate::create(Engine::getInstance()->createAnimation1("skeleton_special_death_E_0.0_", 8, 0.1)); //1

    specialDeath_NE = Animate::create(Engine::getInstance()->createAnimation1("skeleton_special_death_NE_45.0_", 8, 0.1)); //3

    specialDeath_N = Animate::create(Engine::getInstance()->createAnimation1("skeleton_special_death_N_90.0_", 8, 0.1)); //5
 
    specialDeath_NW = Animate::create(Engine::getInstance()->createAnimation1("skeleton_special_death_NW_135.0_", 8, 0.1)); //7
 
    specialDeath_W = Animate::create(Engine::getInstance()->createAnimation1("skeleton_special_death_W_180.0_", 8, 0.1)); //9
  
    specialDeath_SW = Animate::create(Engine::getInstance()->createAnimation1("skeleton_special_death_SW_225.0_", 8, 0.1)); //11
  
    specialDeath_S = Animate::create(Engine::getInstance()->createAnimation1("skeleton_special_death_S_270.0_", 8, 0.1)); //13
  
    specialDeath_SE = Animate::create(Engine::getInstance()->createAnimation1("skeleton_special_death_SE_315.0_", 8, 0.1)); //15
  

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