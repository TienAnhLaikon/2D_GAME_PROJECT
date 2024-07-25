#include "Player.h"
#include "Engine/Engine.h"
#include "Map/Map.h"
#include "../Game/Game.h"
#include "../HelloWorldScene.h"
#include "SoundTimeCounter/Counter.h"
#include "./Item/PotionManager.h"
#include "UserInterface/InGame/InGameUI.h"
#include "WorkingWithData/JSON/jsonManager.h"
#include "CoinManager/CoinManager.h"
#include "UserInterface/InGame/LevelUpPopup.h"
#include "Scenes/SelectMapLayer.h"
bool isCanPlayTakeDmgSound = true;

Player* Player::createPlayer()
{
    auto player = new (std::nothrow) Player();
    if (player && player->init())
    {
        player->autorelease();
        return player;
    }
    else
    {
        delete player;
        return nullptr;
    }
}

bool Player::init() {
    if (!Node::init()) return false;
    //addSkill("FireSear");

    //for (auto skill : skills) {
    //    if (skill->getSkillTalent()->skillName == "FireSear") skill->setSkillIndex(5);
    //}
    //resetSkillSlotOrder();
    #pragma region Stats Base 

    //Load Iventory
    inventory = Inventory::createInventory(this);
    inventory->retain();
    upgradeInventory = UpgradeInventory::createUpgradeInventory(this);
    upgradeInventory->retain();
    subInventory = SubInventory::createSubInventory();
    subInventory->retain();
    //Create direction
    direction = 1;

    //Init Knight Sprite
    characterCurrentSprite = Sprite::createWithSpriteFrameName("knight_armed_idle_E_0.0_0.png");
    //Highlight'
    highlight = Sprite::create("playable character/player_highlight/highlight_yellow.png");
    highlight->setScale(0.7);
    this->addChild(highlight);
    
    //JsonManager::getInstance()->getBasePlayer()
    auto basePlayer = BasePlayer();

    //Level
    currentExp = 0;
    expRequiredToLevelUp = 280;
    level = 1;
    if (basePlayer.Max_level > 0) 
        maxLevel = basePlayer.Max_level;
    else 
        maxLevel = 15;
    //STATS
    //HP
    if (basePlayer.HP > 0)
        HP = basePlayer.HP;
    else
        HP = 570;
    //MP

    if(basePlayer.MP > 0) 
        MP = basePlayer.MP;
    else 
        MP = 340;
    //HP regen

    if(basePlayer.HP_regen > 0)
        HP_regen = basePlayer.HP_regen;
    else 
        HP_regen = 5.6;
    //MP regen

    if(basePlayer.MP_regen > 0)
        MP_regen = basePlayer.MP_regen;
    else 
        MP_regen = 7.9;
    //Armor

    if(basePlayer.Armor > 0)
        baseArmor = basePlayer.Armor;
    else 
        baseArmor = 35;
    //AD

    if(basePlayer.AD > 0)
        baseDamage = basePlayer.AD;
    else 
        baseDamage = 1000;

    //AP
    if(basePlayer.AP > 0)
        AP = basePlayer.AP;
    else 
        AP = 45;
    //MS

    if(basePlayer.MS > 0)
        MS = basePlayer.MS;
    else 
        MS = 80;
    //AR

    if(basePlayer.Attack_range > 0)
        attackRange = basePlayer.Attack_range;
    else    
        attackRange = 50;

    //AS
    if(basePlayer.Attack_speed > 0)
        attackSpeed = basePlayer.Attack_speed;
    else 
        attackSpeed = 5;
    //ExpGain

    if(basePlayer.Exp_gain > 0)
        expGain = basePlayer.Exp_gain;
    else 
        expGain = 0;
        

    if (basePlayer.CDR > 0)
        baseCDR = basePlayer.CDR;
    else
        baseCDR = 1;

    //STATS BONUS
    if (basePlayer.HP_bonus > 0)
        HP_bonus = basePlayer.HP_bonus;
    else
        HP_bonus = 1.2;

    if (basePlayer.MP_bonus > 0)
        MP_bonus = basePlayer.MP_bonus;
    else
        MP_bonus = 1.1;

    if (basePlayer.HP_regen_bonus > 0)
        HP_regen_bonus = basePlayer.HP_regen_bonus;
    else
        HP_regen_bonus = 1.1;

    if (basePlayer.MP_regen_bonus > 0)
        MP_regen_bonus = basePlayer.MP_regen_bonus;
    else
        MP_regen_bonus = 1.1;

    if (basePlayer.Armor_bonus > 0)
        Armor_bonus = basePlayer.Armor_bonus;
    else
        Armor_bonus = 1.1;

    if (basePlayer.AD_bonus > 0)
        AD_bonus = basePlayer.AD_bonus;
    else
        AD_bonus = 1.05;

    if (basePlayer.AP_bonus > 0)
        AP_bonus = basePlayer.AP_bonus;
    else
        AP_bonus = 1.2;

    if (basePlayer.MS_bonus > 0)
        MS_bonus = basePlayer.MS_bonus;
    else
        MS_bonus = 1.0;

    if (basePlayer.AR_bonus > 0)
        AR_bonus = basePlayer.AR_bonus;
    else
        AR_bonus = 1.0;

    if (basePlayer.AS_bonus > 0)
        AS_bonus = basePlayer.AS_bonus;
    else
        AS_bonus = 1.0;

    if (basePlayer.Exp_gain_bonus > 0)
        expGain_bonus = basePlayer.Exp_gain_bonus;
    else
        expGain_bonus = 0;

    //
    currentHP = HP;
    currentMP = MP;

    setAttackSpeed(attackSpeed);
    //Load Animate
    this->loadAnimate();
    //Init physics
    auto physicsBody = PhysicsBody::createBox(Size(20, 20), PhysicsMaterial(1.0f, 0.0f, 0.0f));
    physicsBody->setGravityEnable(false);
    physicsBody->setRotationEnable(false);
    physicsBody->setDynamic(false);
    this->setPhysicsBody(physicsBody);
    this->addChild(characterCurrentSprite);

    physicsBody->setCategoryBitmask(0x04); // Category của Player
    physicsBody->setCollisionBitmask(0x00); // Không va chạm với FireBall và Enemy
    physicsBody->setContactTestBitmask(0x00); // Không kiểm tra va chạm với FireBall và Enemy

    this->idleState = new PlayerIdle(this);
    this->attackState = new PlayerAttack(this);
    this->walkState = new PlayerWalk(this);
    this->deadState = new PlayerDead(this);
    this->selectState = new PlayerSelect(this);

    this->retain();
    ////update
    schedule(CC_SCHEDULE_SELECTOR(Player::regenStats), 1.0f);
    Scene* game = Director::getInstance()->getRunningScene();
    currentScene = dynamic_cast<Game*>(game->getChildByName("GameInstance"));
    #pragma endregion

    return true;
}

Player::~Player() {
    upgradeInventory->release();
    if(upgradeInventory)  delete upgradeInventory;

    inventory->release();
    if (inventory) delete inventory;

    upgradeInventory = nullptr;
    inventory = nullptr;

    //Arm Walk
    armWalk_E->release();
    armWalk_NE->release();
    armWalk_N->release();
    armWalk_NW->release();
    armWalk_W->release();
    armWalk_SW->release(); //
    armWalk_S->release(); //
    armWalk_SE->release(); //

    //Default Walk
    defaultWalk_E->release(); //1

    defaultWalk_NE->release(); //3

    defaultWalk_N->release(); //5

    defaultWalk_NW->release(); //7

    defaultWalk_W->release(); //9

    defaultWalk_SW->release(); //11

    defaultWalk_S->release(); //13

    defaultWalk_SE->release(); //15


    //Special Dead
    specialDeath_E->release(); //1

    specialDeath_NE->release(); //3

    specialDeath_N->release(); //5

    specialDeath_NW->release(); //7

    specialDeath_W->release(); //9

    specialDeath_SW->release(); //11

    specialDeath_S->release(); //13

    specialDeath_SE->release(); //15


    //Special Select
    specialSelect_E->release(); //1

    specialSelect_NE->release(); //3

    specialSelect_N->release(); //5

    specialSelect_NW->release(); //7

    specialSelect_W->release(); //9

    specialSelect_SW->release(); //11

    specialSelect_S->release(); //13

    specialSelect_SE->release(); //15


    armIdle_E->release();
    armIdle_NE->release();
    armIdle_N->release();
    armIdle_NW->release();
    armIdle_W->release();
    armIdle_SW->release(); //
    armIdle_S->release(); //
    armIdle_SE->release(); //
    //Arm Attack
    armAttack_E->release(); //
    armAttack_NE->release(); //
    armAttack_N->release(); //
    armAttack_NW->release(); //
    armAttack_W->release(); //9
    armAttack_SW->release(); //11
    armAttack_S->release(); //13
    armAttack_SE->release(); //15
}

#pragma region Animate
void Player::setAttackSpeed(float attackSpeed) {
    int numSprite = 8;
    float delay = (1 / attackSpeed) / numSprite;
    //Arm Attack  
    armAttack_E = Animate::create(Engine::getInstance()->createAnimation1("knight_armed_attack_E_0.0_", numSprite, delay)); //1

    armAttack_NE = Animate::create(Engine::getInstance()->createAnimation1("knight_armed_attack_NE_45.0_", numSprite, delay)); //3

    armAttack_N = Animate::create(Engine::getInstance()->createAnimation1("knight_armed_attack_N_90.0_", numSprite, delay)); //5

    armAttack_NW = Animate::create(Engine::getInstance()->createAnimation1("knight_armed_attack_NW_135.0_", numSprite, delay)); //7

    armAttack_W = Animate::create(Engine::getInstance()->createAnimation1("knight_armed_attack_W_180.0_", numSprite, delay)); //9

    armAttack_SW = Animate::create(Engine::getInstance()->createAnimation1("knight_armed_attack_SW_225.0_", numSprite, delay)); //11

    armAttack_S = Animate::create(Engine::getInstance()->createAnimation1("knight_armed_attack_S_270.0_", numSprite, delay)); //13

    armAttack_SE = Animate::create(Engine::getInstance()->createAnimation1("knight_armed_attack_SE_315.0_", numSprite, delay)); //15

    //Arm Attack
    armAttack_E->retain(); //
    armAttack_NE->retain(); //
    armAttack_N->retain(); //
    armAttack_NW->retain(); //
    armAttack_W->retain(); //9
    armAttack_SW->retain(); //11
    armAttack_S->retain(); //13
    armAttack_SE->retain(); //15
}

void Player::loadAnimate() {

    ////Arm Walk
    armWalk_E = Animate::create(Engine::getInstance()->createAnimation1("knight_armed_walk_E_0.0_", 8, 0.08)); //1

    armWalk_NE = Animate::create(Engine::getInstance()->createAnimation1("knight_armed_walk_NE_45.0_", 8, 0.08)); //3
    armWalk_N = Animate::create(Engine::getInstance()->createAnimation1("knight_armed_walk_N_90.0_", 8, 0.08)); //5

    armWalk_NW = Animate::create(Engine::getInstance()->createAnimation1("knight_armed_walk_NW_135.0_", 8, 0.08)); //7

    armWalk_W = Animate::create(Engine::getInstance()->createAnimation1("knight_armed_walk_W_180.0_", 8, 0.08)); //9

    armWalk_SW = Animate::create(Engine::getInstance()->createAnimation1("knight_armed_walk_SW_225.0_", 8, 0.08)); //11

    armWalk_S = Animate::create(Engine::getInstance()->createAnimation1("knight_armed_walk_S_270.0_", 8, 0.08)); //13

    armWalk_SE = Animate::create(Engine::getInstance()->createAnimation1("knight_armed_walk_SE_315.0_", 8, 0.08)); //15

    //knight_armed_walk_SE_315.0_

   //Default Walk
    defaultWalk_E = Animate::create(Engine::getInstance()->createAnimation1("knight_default_walk_E_0.0_", 8, 0.05)); //1

    defaultWalk_NE = Animate::create(Engine::getInstance()->createAnimation1("knight_default_walk_NE_45.0_", 8, 0.05)); //3

    defaultWalk_N = Animate::create(Engine::getInstance()->createAnimation1("knight_default_walk_N_90.0_", 8, 0.05)); //5

    defaultWalk_NW = Animate::create(Engine::getInstance()->createAnimation1("knight_default_walk_NW_135.0_", 8, 0.05)); //7

    defaultWalk_W = Animate::create(Engine::getInstance()->createAnimation1("knight_default_walk_W_180.0_", 8, 0.05)); //9

    defaultWalk_SW = Animate::create(Engine::getInstance()->createAnimation1("knight_default_walk_SW_225.0_", 8, 0.05)); //11

    defaultWalk_S = Animate::create(Engine::getInstance()->createAnimation1("knight_default_walk_S_270.0_", 8, 0.05)); //13

    defaultWalk_SE = Animate::create(Engine::getInstance()->createAnimation1("knight_default_walk_SE_315.0_", 8, 0.05)); //15


    //Special Dead
    specialDeath_E = Animate::create(Engine::getInstance()->createAnimation1("knight_special_death_E_0.0_", 8, 0.1)); //1

    specialDeath_NE = Animate::create(Engine::getInstance()->createAnimation1("knight_special_death_NE_45.0_", 8, 0.1)); //3

    specialDeath_N = Animate::create(Engine::getInstance()->createAnimation1("knight_special_death_N_90.0_", 8, 0.1)); //5

    specialDeath_NW = Animate::create(Engine::getInstance()->createAnimation1("knight_special_death_NW_135.0_", 8, 0.1)); //7

    specialDeath_W = Animate::create(Engine::getInstance()->createAnimation1("knight_special_death_W_180.0_", 8, 0.1)); //9

    specialDeath_SW = Animate::create(Engine::getInstance()->createAnimation1("knight_special_death_SW_225.0_", 8, 0.1)); //11

    specialDeath_S = Animate::create(Engine::getInstance()->createAnimation1("knight_special_death_S_270.0_", 8, 0.1)); //13

    specialDeath_SE = Animate::create(Engine::getInstance()->createAnimation1("knight_special_death_SE_315.0_", 8, 0.1)); //15


    //Special Select
    specialSelect_E = Animate::create(Engine::getInstance()->createAnimation1("knight_special_select_E_0.0_", 8, 0.06)); //1

    specialSelect_NE = Animate::create(Engine::getInstance()->createAnimation1("knight_special_select_NE_45.0_", 8, 0.06)); //3

    specialSelect_N = Animate::create(Engine::getInstance()->createAnimation1("knight_special_select_N_90.0_", 8, 0.06)); //5

    specialSelect_NW = Animate::create(Engine::getInstance()->createAnimation1("knight_special_select_NW_135.0_", 8, 0.06)); //7

    specialSelect_W = Animate::create(Engine::getInstance()->createAnimation1("knight_special_select_W_180.0_", 8, 0.06)); //9

    specialSelect_SW = Animate::create(Engine::getInstance()->createAnimation1("knight_special_select_SW_225.0_", 8, 0.06)); //11

    specialSelect_S = Animate::create(Engine::getInstance()->createAnimation1("knight_special_select_S_270.0_", 8, 0.06)); //13

    specialSelect_SE = Animate::create(Engine::getInstance()->createAnimation1("knight_special_select_SE_315.0_", 8, 0.06)); //15



    //Armed Idle
    armIdle_E = Animate::create(Engine::getInstance()->createAnimation1("knight_armed_idle_E_0.0_", 1, 0.1)); //1

    armIdle_NE = Animate::create(Engine::getInstance()->createAnimation1("knight_armed_idle_NE_45.0_", 1, 0.1)); //3

    armIdle_N = Animate::create(Engine::getInstance()->createAnimation1("knight_armed_idle_N_90.0_", 1, 0.1)); //5

    armIdle_NW = Animate::create(Engine::getInstance()->createAnimation1("knight_armed_idle_NW_135.0_", 1, 0.1)); //7

    armIdle_W = Animate::create(Engine::getInstance()->createAnimation1("knight_armed_idle_W_180.0_", 1, 0.1)); //9

    armIdle_SW = Animate::create(Engine::getInstance()->createAnimation1("knight_armed_idle_SW_225.0_", 1, 0.1)); //11

    armIdle_S = Animate::create(Engine::getInstance()->createAnimation1("knight_armed_idle_S_270.0_", 1, 0.1)); //13

    armIdle_SE = Animate::create(Engine::getInstance()->createAnimation1("knight_armed_idle_SE_315.0_", 1, 0.1)); //15

    //Arm Walk
    armWalk_E->retain();
    armWalk_NE->retain();
    armWalk_N->retain();
    armWalk_NW->retain();
    armWalk_W->retain();
    armWalk_SW->retain(); //
    armWalk_S->retain(); //
    armWalk_SE->retain(); //

    //Default Walk
    defaultWalk_E->retain(); //1

    defaultWalk_NE->retain(); //3

    defaultWalk_N->retain(); //5

    defaultWalk_NW->retain(); //7

    defaultWalk_W->retain(); //9

    defaultWalk_SW->retain(); //11

    defaultWalk_S->retain(); //13

    defaultWalk_SE->retain(); //15


    //Special Dead
    specialDeath_E->retain(); //1

    specialDeath_NE->retain(); //3

    specialDeath_N->retain(); //5

    specialDeath_NW->retain(); //7

    specialDeath_W->retain(); //9

    specialDeath_SW->retain(); //11

    specialDeath_S->retain(); //13

    specialDeath_SE->retain(); //15


    //Special Select
    specialSelect_E->retain(); //1

    specialSelect_NE->retain(); //3

    specialSelect_N->retain(); //5

    specialSelect_NW->retain(); //7

    specialSelect_W->retain(); //9

    specialSelect_SW->retain(); //11

    specialSelect_S->retain(); //13

    specialSelect_SE->retain(); //15


    armIdle_E->retain();
    armIdle_NE->retain();
    armIdle_N->retain();
    armIdle_NW->retain();
    armIdle_W->retain();
    armIdle_SW->retain(); //
    armIdle_S->retain(); //
    armIdle_SE->retain(); //
}

#pragma endregion


void Player::increaseStat(float amount, std::string statName) {
    if (statName == "Health Potion") {
        auto amt = this->getHPRegen() + amount;
        this->setHPRegen(amt);
    }
    else if (statName == "Mana Potion") {
        auto amt = this->getMPRegen() + amount;
        this->setMPRegen(amt);
    }
    else if(statName == "Speed Potion") {
        auto amt = this->getMovementSpeed() + amount;
        this->setMovementSpeed(amt);
    }
}

void Player::decreaseStat(float amount, std::string statName) {
    if (statName == "Health Potion") {
        this->setHPRegen(amount);
    }
    else if (statName == "Mana Potion") {
        this->setMPRegen(amount);
    }
    else if(statName == "Speed Potion") {
        this->setMovementSpeed(amount);
    }
}

void Player::usePotion(std::string potionName) {
    auto potion = ItemFactory::createItem(potionName);
    float prevStat = 0;
    if (potion->getItemName() == "Health Potion" && currentScene ) {
        if (getCurrentHP() < getHP()) {

        }
        else {
            prevStat = this->HP_regen;
        }
    } 
    else if (potion->getItemName() == "Speed Potion" && currentScene) {
        prevStat = this->MS;
    }
    else if (potion->getItemName() == "Mana Potion" && currentScene) {
        prevStat = this->MP_regen;
    }
    else if (potion->getItemName() == "Scroll Paper" && currentScene) {
        InGameUI::getInstance(this)->selectMapLayer->show();
    }
    
    UserDefault::getInstance()->setIntegerForKey("sound_effect",Audio::getInstance()->play2d("sound/sounds effect/use_potion.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
    this->increaseStat(potion->increaseAmount, potionName);
    this->scheduleOnce([this, potionName, prevStat] (float dt) {
        this->decreaseStat(prevStat, potionName);
        }, potion->effectTime, "end_effect_callback");
    potion->currentCooldown = potion->cooldown;
}

#pragma region Inventory

void Player::pickUpEquipment(std::string equipmentName, int equipmentLv) {
    inventory->addEquipment(equipmentName, equipmentLv, 1);
    upgradeInventory->addEquipment(equipmentName, equipmentLv, 1);
}

void Player::pickUpItem(std::string itemName) {
    if (itemName != "Gold") {
        inventory->addItem(itemName, 1);
        setMessage(itemName);
        auto item = ItemFactory::createItem(itemName);
        if (item->getItemType() == BaseItem::Type::UPGRADE_ITEM) {
            upgradeInventory->addItem(itemName, 1);
        }
    }

    //Gold 
    if (itemName == "Gold") {
        CoinManager::getInstance()->increaseCoin(200);
    }
}

void Player::dropItem(std::string itemName) {
    if (itemName == "") return;
    UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/drop_item.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
    inventory->dropItem(itemName);
    upgradeInventory->dropItem(itemName);
}


std::pair<std::string, int> Player::getItem(std::string itemName) {
    return inventory->getItem(itemName);
}

void Player::dropEquipment(std::string itemName) {
    UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/drop_item.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
    inventory->dropEquipment(itemName);
    upgradeInventory->dropEquipment(itemName);
}

std::vector<std::string> Player::getAllItem() {
    return inventory->getAllItem();
}

void Player::upgradeEquipment(std::string equipmentName) {
    if (equipmentName.length() < 1) return;
    inventory->upgradeEquipment(equipmentName);
}

std::vector<BaseEquipment::EquipmentData> Player::getAllEquipment() {
    return inventory->getAllEquipment();
}

void Player::setEquipment(std::string equipmentName, int equipmentLevel, std::string equipmentType) {
    if (equipmentType == "Weapon") {
        this->weapon = EquipmentFactory::createEquipment(equipmentName, equipmentLevel);
        this->equipmentDamage += this->weapon->getDamage();
        this->equipmentSkillDamage += this->weapon->getSkillDamage();
        this->equipmentArmor += this->weapon->getArmor();
        this->equipmentCDR += this->weapon->getCDR();
        this->equipmentMS += this->weapon->getMovementSpeed();
        this->equipmentMP += this->weapon->getMP();
        _playerUsingEquipment.weaponNLevel = std::make_tuple(equipmentName, equipmentLevel, equipmentType);
    }
    else if (equipmentType == "Ring") {
        this->ring = EquipmentFactory::createEquipment(equipmentName, equipmentLevel);
        this->equipmentDamage += this->ring->getDamage();
        this->equipmentSkillDamage += this->ring->getSkillDamage();
        this->equipmentArmor += this->ring->getArmor();
        this->equipmentCDR += this->ring->getCDR();
        this->equipmentMS += this->ring->getMovementSpeed();
        this->equipmentMP += this->ring->getMP();
        _playerUsingEquipment.ringNLevel = std::make_tuple(equipmentName, equipmentLevel, equipmentType);
    }
    else if (equipmentType == "Helmet") {
        this->helmet = EquipmentFactory::createEquipment(equipmentName, equipmentLevel);
        this->equipmentDamage += this->helmet->getDamage();
        this->equipmentSkillDamage += this->helmet->getSkillDamage();
        this->equipmentArmor += this->helmet->getArmor();
        this->equipmentCDR += this->helmet->getCDR();
        this->equipmentMS += this->helmet->getMovementSpeed();
        this->equipmentMP += this->helmet->getMP();
        _playerUsingEquipment.helmetNLevel = std::make_tuple(equipmentName, equipmentLevel, equipmentType);
    }
    else if (equipmentType == "Shoe") {
        this->shoe = EquipmentFactory::createEquipment(equipmentName, equipmentLevel);
        this->equipmentDamage += this->shoe->getDamage();
        this->equipmentSkillDamage += this->shoe->getSkillDamage();
        this->equipmentArmor += this->shoe->getArmor();
        this->equipmentCDR += this->shoe->getCDR();
        this->equipmentMS += this->shoe->getMovementSpeed();
        this->equipmentMP += this->shoe->getMP();
        _playerUsingEquipment.shoeNLevel = std::make_tuple(equipmentName, equipmentLevel, equipmentType);
    }
    else if (equipmentType == "Armor") {
        this->armor = EquipmentFactory::createEquipment(equipmentName, equipmentLevel);
        this->equipmentDamage += this->armor->getDamage();
        this->equipmentSkillDamage += this->armor->getSkillDamage();
        this->equipmentArmor += this->armor->getArmor();
        this->equipmentCDR += this->armor->getCDR();
        this->equipmentMS += this->armor->getMovementSpeed();
        this->equipmentMP += this->armor->getMP();
        _playerUsingEquipment.armorNLevel = std::make_tuple(equipmentName, equipmentLevel, equipmentType);
    }
    else if (equipmentType == "Shield") {
        this->shield = EquipmentFactory::createEquipment(equipmentName, equipmentLevel);
        this->equipmentDamage += this->shield->getDamage();
        this->equipmentSkillDamage += this->shield->getSkillDamage();
        this->equipmentArmor += this->shield->getArmor();
        this->equipmentCDR += this->shield->getCDR();
        this->equipmentMS += this->shield->getMovementSpeed();
        this->equipmentMP += this->shield->getMP();
        _playerUsingEquipment.shieldNLevel = std::make_tuple(equipmentName, equipmentLevel, equipmentType);
    }
}

void Player::equipWeapon(std::string name, int level) {
    if (name == "") return;
    this->setEquipment(name, level, "Weapon");
    inventory->equipWeapon(name, level);
}
void Player::equipShoe(std::string name, int level) {
    if (name == "") return;
    this->setEquipment(name, level, "Shoe");
    inventory->equipShoe(name, level);
}
void Player::equipArmor(std::string name, int level) {
    if (name == "") return;
    this->setEquipment(name, level, "Armor");
    inventory->equipArmor(name, level);
}
void Player::equipRing(std::string name, int level) {
    if (name == "") return;
    this->setEquipment(name, level, "Ring");
    inventory->equipRing(name, level);
}
void Player::equipShield(std::string name, int level) {
    if (name == "") return;
    this->setEquipment(name, level, "Shield");
    inventory->equipShield(name, level);
}
void Player::equipHelmet(std::string name, int level) {
    if (name == "") return;
    this->setEquipment(name, level, "Helmet");
    inventory->equipHelmet(name, level);
}
void Player::equipItem(std::string name, int quantity) {
    if (name == "") return;
    for (auto itemNode : inventory->inventoryNodes) {
        if (itemNode->getBaseItem() && itemNode->getBaseItem()->getItemName() == name) {
            itemNode->removeBaseItem();
            //inventory->get
            break;
        }
    }
    subInventory->addItem(name, quantity);
}

BaseEquipment* Player::getEquipment(std::string equipmentType) {
    if (equipmentType == "Weapon") {
        return this->weapon;
    }
    else if (equipmentType == "Ring") {
        return this->ring;
    }
    else if (equipmentType == "Helmet") {
        return this->helmet;
    }
    else if (equipmentType == "Shoe") {
        return this->shoe;
    }
    else if (equipmentType == "Armor") {
        return this->armor;
    }
    else if (equipmentType == "Shield") {
        return this->shield;
    }
}

void Player::removeEquipment(std::string equipmentType) {
    if (equipmentType == "Weapon") {
        this->equipmentDamage -= this->weapon->getDamage();
        this->equipmentSkillDamage -= this->weapon->getSkillDamage();
        this->equipmentArmor -= this->weapon->getArmor();
        this->equipmentCDR -= this->weapon->getCDR();
        this->equipmentMS -= this->weapon->getMovementSpeed();
        this->equipmentMP -= this->weapon->getMP();
    }
    else if (equipmentType == "Ring") {
        this->equipmentDamage -= this->ring->getDamage();
        this->equipmentSkillDamage -= this->ring->getSkillDamage();
        this->equipmentArmor -= this->ring->getArmor();
        this->equipmentCDR -= this->ring->getCDR();
        this->equipmentMS -= this->ring->getMovementSpeed();
        this->equipmentMP -= this->ring->getMP();
    }
    else if (equipmentType == "Helmet") {
        this->equipmentDamage -= this->helmet->getDamage();
        this->equipmentSkillDamage -= this->helmet->getSkillDamage();
        this->equipmentArmor -= this->helmet->getArmor();
        this->equipmentCDR -= this->helmet->getCDR();
        this->equipmentMS -= this->helmet->getMovementSpeed();
        this->equipmentMP -= this->helmet->getMP();
    }
    else if (equipmentType == "Shoe") {
        this->equipmentDamage -= this->shoe->getDamage();
        this->equipmentSkillDamage -= this->shoe->getSkillDamage();
        this->equipmentArmor -= this->shoe->getArmor();
        this->equipmentCDR -= this->shoe->getCDR();
        this->equipmentMS -= this->shoe->getMovementSpeed();
        this->equipmentMP -= this->shoe->getMP();
    }
    else if (equipmentType == "Armor") {
        this->equipmentDamage -= this->armor->getDamage();
        this->equipmentSkillDamage -= this->armor->getSkillDamage();
        this->equipmentArmor -= this->armor->getArmor();
        this->equipmentCDR -= this->armor->getCDR();
        this->equipmentMS -= this->armor->getMovementSpeed();
        this->equipmentMP -= this->armor->getMP();
    }
    else if (equipmentType == "Shield") {
        this->equipmentDamage -= this->shield->getDamage();
        this->equipmentSkillDamage -= this->shield->getSkillDamage();
        this->equipmentArmor -= this->shield->getArmor();
        this->equipmentCDR -= this->shield->getCDR();
        this->equipmentMS -= this->shield->getMovementSpeed();
        this->equipmentMP -= this->shield->getMP();
    }
}
#pragma endregion


float Player::calculateAngle(const Vec2& vectorA, const Vec2& vectorB) {
    // Tnh gc gia hai vector s dng atan2
    float angle = vectorA.getAngle(vectorB);

    // Chuyn gc t radian sang 
    auto degree = CC_RADIANS_TO_DEGREES(angle);


    return degree;
}

void Player::setVisibleSkillButton(bool Visible)
{
    for (auto skill : skills) {
        skill->_skillButton->setVisible(Visible);
    }
}
int count = 0;
void Player::handleMove(float deltaTime) {
    bool _isHideObj = false;
    count += 1;
    if (count % 8 == 0) {
        Counter::increase(1);
    }
    if (this->currentState != this->attackState && this->currentState != this->deadState && this->currentState != this->selectState) {
        if (moving != true) moving = true;
        /* mouseClickPos = targetPosition;
         Vec2 currentPosition = this->getPosition();*/
        Vec2 direction = movementDirection;
        Vec2 newPos = this->getPosition() + direction * this->MS * deltaTime;
        _isHideObj = isHideObject(this->getPosition());
        if (_isHideObj) {
            if(currentScene->getPlayer()->getLocalZOrder() != 2)
                    currentScene->getPlayer()->setLocalZOrder(2);
        }
        else {
            if (currentScene->getPlayer()->getLocalZOrder() != 20)
                currentScene->getPlayer()->setLocalZOrder(20);
        }

        if (isShowObjectRoom(this->getPosition())) {
            if (currentScene->gameMap && currentScene->gameMap->getTiledMap()) {
                if (currentScene->gameMap->getTiledMap()->getLayer("topgrass") && currentScene->gameMap->getTiledMap()->getLayer("topgrass")->isVisible() == true)
                    currentScene->gameMap->getTiledMap()->getLayer("topgrass")->setVisible(false);
                if (currentScene->gameMap->getTiledMap()->getLayer("topwall") && currentScene->gameMap->getTiledMap()->getLayer("topwall")->isVisible() == true)
                    currentScene->gameMap->getTiledMap()->getLayer("topwall")->setVisible(false);
                if (currentScene->gameMap->getTiledMap()->getLayer("topshadow") && currentScene->gameMap->getTiledMap()->getLayer("topshadow")->isVisible() == true)
                    currentScene->gameMap->getTiledMap()->getLayer("topshadow")->setVisible(false);
            }
        }
        else {
            if (currentScene->gameMap && currentScene->gameMap->getTiledMap()) {
                if(currentScene->gameMap->getTiledMap()->getLayer("topgrass") && currentScene->gameMap->getTiledMap()->getLayer("topgrass")->isVisible() == false)
                    currentScene->gameMap->getTiledMap()->getLayer("topgrass")->setVisible(true);
                if (currentScene->gameMap->getTiledMap()->getLayer("topwall") && currentScene->gameMap->getTiledMap()->getLayer("topwall")->isVisible() == false)
                    currentScene->gameMap->getTiledMap()->getLayer("topwall")->setVisible(true);
                if (currentScene->gameMap->getTiledMap()->getLayer("topshadow") && currentScene->gameMap->getTiledMap()->getLayer("topshadow")->isVisible() == false)
                    currentScene->gameMap->getTiledMap()->getLayer("topshadow")->setVisible(true);
            }
        }


        if (isCanMove(newPos))
            this->setPosition(newPos);

        if (isInHidePlayerZone(newPos)) {
            this->setVisible(false);
        }
        else {
            this->setVisible(true);
        }
        

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

void Player::addSkill(std::string skillName) {
    Size size = Director::getInstance()->getVisibleSize();
    float scale = size.width / size.height * 2;
    auto skill = SkillFactory::createSkill(skillName);
    skills.push_back(skill);
    skill->_skillButton->setScale(0.6 * Director::getInstance()->getContentScaleFactor());

    InGameUI::getInstance(this)->addChild(skill->_skillButton);
    skill->_skillButton->setPosition(Vec2(1000, 1000));
    skill->_skillButton->setVisible(false);
    skill->setSkillIndex(0);
    this->addChild(skill);
    resetSkillSlotOrder();
}
void Player::setSkill(SkillBase* skill) {
    if (skill->getSkillIndex() == 5) return;
    Size size = Director::getInstance()->getVisibleSize();
    float scale = size.width / size.height * 2;
    skills.push_back(skill);
    skill->_skillButton->setScale(0.6 * Director::getInstance()->getContentScaleFactor());
    skill->_skillButton->setPosition(Vec2(1000, 1000));
    InGameUI::getInstance(this)->addChild(skill->_skillButton);
    skill->_skillButton->setVisible(false);

    this->addChild(skill);
    resetSkillSlotOrder();
}

void Player::gainExp(float amountExp) {
    currentExp += amountExp;
    expToConsume += amountExp;

    if (currentExp >= expRequiredToLevelUp) {
        //So snh expRemain vi cc expRequired ca cc level sau, 
        // nu cn ln hn th tr n bao gi nh hn th thi
        // v s ln phi tr s tng ng vi s ln ln level dm
        while (true) {
            expRemain = currentExp - expRequiredToLevelUp;
            this->levelUp();
            currentExp = expRemain;
            if (currentExp < expRequiredToLevelUp)break;
        }
    }
    InGameUI::getInstance(this)->expBar->updateExpBar(currentExp, expRequiredToLevelUp);
}

void Player::gainExpFromData(float amountExp) {
    currentExp += amountExp;
    expToConsume += amountExp;

    if (currentExp >= expRequiredToLevelUp) {
        //So snh expRemain vi cc expRequired ca cc level sau, 
        // nu cn ln hn th tr n bao gi nh hn th thi
        // v s ln phi tr s tng ng vi s ln ln level dm
        while (true) {
            expRemain = currentExp - expRequiredToLevelUp;
            this->levelUpFromData();
            currentExp = expRemain;
            if (currentExp < expRequiredToLevelUp)break;
        }
    }
    InGameUI::getInstance(this)->expBar->updateExpBar(currentExp, expRequiredToLevelUp);
}

//Level Manager
void Player::levelUp() {
    if (level < maxLevel) {
        level++;
        //Gain Stats and Recover Full HP and MP
        gainAllStats();
        backToFullHealthAndMana();
        //Gain Exp Required To Level Up
        setExpRequiredToLevelUp();
        //InGameUI::getInstance(this)->levelUpPopup->show(this->level);

        //
        if (this->getChildByName("LevelUpPopup")) {
            if(this->getChildByName("LevelUpPopup")->getNumberOfRunningActions() > 0)this->getChildByName("LevelUpPopup")->stopAllActions();
            this->removeChildByName("LevelUpPopup");
        }
        auto nodeLevelUp = Node::create();
        nodeLevelUp ->setPosition(Vec2(0, 50));
        nodeLevelUp->setScale(0.5);
        nodeLevelUp->setName("LevelUpPopup");
        auto levelUpSprite = Sprite::create("res/level-up-popup.png");
        levelUpSprite->setScale(0.12 * Director::getInstance()->getContentScaleFactor());
        auto levelUpMsg = Label::createWithTTF("Well done on reaching a new level!" + std::to_string(level), "fonts/Diablo Light.ttf", 10);
        levelUpMsg->setTextColor(Color4B::GREEN);
        levelUpMsg->setPosition(Vec2(0, -levelUpSprite->getContentSize().height * levelUpSprite->getScaleY() + 12));
        auto levelUpLabel = Label::createWithTTF("LV " + std::to_string(level), "fonts/Diablo Light.ttf", 16);
        nodeLevelUp->addChild(levelUpSprite);
        nodeLevelUp->addChild(levelUpMsg);
        nodeLevelUp->addChild(levelUpLabel);
        levelUpLabel->setString("" + std::to_string(level));
        this->addChild(nodeLevelUp, 10);

        auto sqe = Sequence::create(FadeIn::create(0.8), DelayTime::create(3.5), FadeOut::create(1.5), DelayTime::create(0.5), RemoveSelf::create(), nullptr);
        nodeLevelUp->runAction(sqe);
    }
}

void Player::levelUpFromData() {
    if (level < maxLevel) {
        level++;
        //Gain Stats and Recover Full HP and MP
        gainAllStats();
        backToFullHealthAndMana();
        //Gain Exp Required To Level Up
        setExpRequiredToLevelUp();
        //InGameUI::getInstance(this)->levelUpPopup->show(this->level);
    }
}
void Player::regenStats(float dt) {
    if(currentHP <= HP) regenHP(HP_regen);
    if (currentMP <= MP) regenMP(MP_regen);
    InGameUI::getInstance(this)->healthBar->updateHealthBar(this->currentHP, this->currentMP, this->HP, this->MP);
}

void Player::setCurrentHP(float newCurrentHP) {
    if (newCurrentHP >= HP) newCurrentHP = HP;
    this->currentHP = newCurrentHP;
    InGameUI::getInstance(this)->healthBar->updateHealthBar(this->currentHP, this->currentMP, this->HP, this->MP);
}
void Player::takeDamage(float amountDamage) {
    float damageActuallyTake = amountDamage * (100 / (100 + this->baseArmor + equipmentArmor));
    if (this->currentHP <= damageActuallyTake) {
        currentHP = 0;
    }
    else {
        currentHP -= damageActuallyTake;
    }

    InGameUI::getInstance(this)->healthBar->updateHealthBar(this->currentHP, this->currentMP, this->HP, this->MP);
    //Lable
    auto dmgTakeLable = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 24);
    dmgTakeLable->setColor(Color3B::RED);
    this->addChild(dmgTakeLable);
    if (Counter::SOUND_COUNTER > 3) {
        UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/player_take_damage.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
        Counter::SOUND_COUNTER = 0;
    }

    int dmgToInt = std::floor(damageActuallyTake);
    dmgTakeLable->setString(StringUtils::format("-%d", dmgToInt));
    Vec2 pos = Vec2(0, characterCurrentSprite->getBoundingBox().size.height / 2);
    auto moveTo = MoveTo::create(1.0f, pos);
    auto fadeOut = FadeOut::create(1.0f);
    auto spawn = Spawn::create(fadeOut, moveTo, nullptr);
    auto sqe = Sequence::create(spawn, RemoveSelf::create(), nullptr);

    dmgTakeLable->runAction(sqe);
}

void Player::update(float dt) {
    if (this->getCurrentHP() <= 0 && this->currentState != this->deadState) {
        this->SwitchState(this->deadState);
    }
    this->currentState->UpdateState();

}
bool Player::isCanMove(const Vec2& newPosition)
{
    if (!listOfCollision.empty()) {
        for (auto i : listOfCollision) {
            if (i->getBoundingBox().containsPoint(newPosition)) {
                return false;
            } 
        }
    }
    return true;
}

bool Player::isHideObject(const Vec2& newPosition) {
    if (!listOfHideObject.empty()) {
        for (auto i : listOfHideObject) {
            if (i->getBoundingBox().containsPoint(newPosition)) {
                return true;
            }
        }
    }
    return false;
}

bool Player::isInHidePlayerZone(const Vec2& newPosition)
{
    auto hidePlayerLayer = currentScene->gameMap->getTiledMap()->getObjectGroup("hideplayer");
    if (hidePlayerLayer) {
        auto& objects = hidePlayerLayer->getObjects();
        for (const auto& obj : objects) {
            auto dict = obj.asValueMap();
            float x = dict["x"].asFloat();
            float y = dict["y"].asFloat();
            float width = dict["width"].asFloat();
            float height = dict["height"].asFloat();

            Rect boundingBox(x, y, width, height);
            if (boundingBox.containsPoint(newPosition)) {
                return true;
            }
        }
    }
    return false;
}

bool Player::isShowObjectRoom(const Vec2& newPosition) {
    if (!listOfShowObjectRoom.empty()) {
        for (auto i : listOfShowObjectRoom) {
            if (i->getBoundingBox().containsPoint(newPosition)) {
                return true;
            }
        }
    }
    return false;
}
Player::PlayerData Player::getPlayerData() {
    PlayerData playerData;
    playerData.positionX = this->getPositionX();
    playerData.positionY = this->getPositionY();
    playerData.health = this->getCurrentHP();
    playerData.mana = this->getCurrentMP();
    playerData.exp = (280 * (pow(1.5, this->getLevel() - 1) - 1) / 0.5) + currentExp; 
    playerData.expToConsume = this->getExpToConsume();
    playerData.currentQuest = 1;
    if(this->currentMap)
        playerData.currentMap = this->currentMap;
    else
        playerData.currentMap = 0;
    if(this->prevMap)
        playerData.prevMap = this->prevMap;
    else
        playerData.prevMap = 0;
    

    return playerData;
}


