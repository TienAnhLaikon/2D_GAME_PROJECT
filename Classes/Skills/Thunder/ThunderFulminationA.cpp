﻿#include "ThunderFulminationA.h"
#include "Player/Player.h"
#include "Map/Map.h"
#include "HelloWorldScene.h"
#include <string>
bool ThunderFulminationA::init() {

    skillTalent = new SkillTalent;
    skillTalent->skillName = "ThunderFulminationA";
    skillTalent->iconPath = "skill/SkillSprite/SkillIcon/Thunder/thunder_fulmination_a.png";
    skillTalent->description = "Summon the multiple of Thunder Pillar dealing a tons of damage to enemies within the area of effect. ";
    skillTalent->skillType = "AOE";
    skillTalent->unlockPoint = 50;
    skillTalent->upgradePoint = 50;
    skillTalent->parent = nullptr;
    skillTalent->isLock = true;
    skillTalent->levelRequired = 0;
    this->skillName = "ThunderFulminationA";
    // Ly kch thc ca mn hnh
    Size visibleSize = Director::getInstance()->getVisibleSize();

    //AOE Range
    _aoeSprite = Sprite::create("skill/AOERangeSprite/CircleRange.png");
    
    _aoeSprite->setScale(visibleSize.width / _aoeSprite->getContentSize().width);
    //_aoeSprite->setScale(0.65);
    _aoeSprite->setOpacity(100);
    _aoeSprite->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(_aoeSprite);
    _aoeSprite->setVisible(false);

    //Skill Icon...
    _iconSprite = Sprite::create("skill/SkillSprite/SkillIcon/Thunder/thunder_fulmination_a.png");
    _iconSprite->setScale(0.1);
    _iconSprite->retain();

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("skill/SkillSprite/PlayerCharacterSkill/Thunder/thunder_fulmination_a/thunder_fulmination_a.plist", "skill/SkillSprite/PlayerCharacterSkill/Thunder/thunder_fulmination_a/thunder_fulmination_a.png");
    //Skill Sprite
    _skillSprite = Sprite::createWithSpriteFrameName("thunder_fulmination_a (1).png");
    _skillSprite->setAnchorPoint(Vec2(0.5, 0.5));
    _skillSprite->retain();

    //Skill Animate
    _skillAnimate = Animate::create(Engine::createAnimation2("thunder_fulmination_a", 130, 0.035));
    _skillAnimate->retain();

    //SkillButton
    _skillButton = SkillButton::create();
    _skillButton->setSkillButtonBorder(_iconSprite);
    _skillButton->retain();

    //SkillStats
    //Level
    currentlevelSkill = 1;
    maxLevelSkill = 5;
    // + Base Stats
    manaCost = 100;
    currentSkillCoolDown = 0;
    skillCooldown = 100;
    //- Buff Skill
    hpExtra = 0;
    mpExtra = 0;
    dmgExtra = 0;
    armorExtra = 0;
    //- Attack Skill
    damageRaitoOfPlayer = 1.5f;
    skillDamage = 650;

    // + Bonus Stats when level up
    skillCooldown_reduct = 10;
    //- Buff Skill
    hpExtra_bonus = 0;
    mpExtra_bonus = 0;
    dmgExtra_bonus = 0;
    armorExtra_bonus = 0;
    //- Attack Skill
    skillDamage_bonus = 100;

    this->coolDownCountLable = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 24);
    _skillButton->addChild(this->coolDownCountLable);
    this->coolDownCountLable->setVisible(false);

    // ng k s kin onTouch cho SkillButton
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(ThunderFulminationA::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(ThunderFulminationA::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(ThunderFulminationA::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, _skillButton);
    _touchListener = touchListener;
    schedule(CC_SCHEDULE_SELECTOR(ThunderFulminationA::update), 0.05f);

    return true;
}

bool ThunderFulminationA::onTouchBegan(Touch* touch, Event* event)
{
    Vec2 touchLocationInNode = _skillButton->convertToNodeSpace(touch->getLocation());

    if (_skillButton->skillButtonBtn->getBoundingBox().containsPoint(touchLocationInNode))
    {
        auto player = dynamic_cast<Player*>(this->getParent());
        if (player->getCurrentMP() < manaCost) {
            player->noManaAlert();
            return false;
        }

        if (currentSkillCoolDown < 0) {
            _skillButton->isPressed = true;
            this->_aoeSprite->setVisible(true);
            _skillButton->cancelButton->setVisible(true);

            return true;
        }
        else {
            NotificationManager::getInstance()->showMessageNotification(this->skillName + " on Cooldown", Vec2::ZERO, Color3B::WHITE, 10);
        }
    }
    return false;
}

void ThunderFulminationA::onTouchMoved(Touch* touch, Event* event)
{
    if (_skillButton->isPressed)
    {
        CCLOG("Moving");
        Vec2 touchLocationInNode = _skillButton->convertToNodeSpace(touch->getLocation());
        Vec2 direction = touchLocationInNode - _skillButton->centerPos;
        float distance = direction.length();

        float radius = 40;

        if (distance > radius)
        {
            direction.normalize();
            direction *= radius;
            _skillButton->skillButtonBtn->setPosition(_skillButton->centerPos + direction);
            _skillButton->currentPos = _skillButton->centerPos + direction;
        }
        else
        {
            _skillButton->skillButtonBtn->setPosition(touchLocationInNode);
            _skillButton->currentPos = touchLocationInNode;
        }

        Vec2 lastTouchLocationInNode = _skillButton->convertToNodeSpace(touch->getLocation());
        if (_skillButton->cancelButton->getBoundingBox().containsPoint(lastTouchLocationInNode)) {
            _skillButton->cancelButton->setOpacity(200);
        }
        else _skillButton->cancelButton->setOpacity(50);

    }
}

void ThunderFulminationA::onTouchEnded(Touch* touch, Event* event)
{
    if (_skillButton->isPressed) {
        _skillButton->prevPosBeforeRelease = _skillButton->skillButtonBtn->getPosition();
        _skillButton->skillButtonBtn->setPosition(_skillButton->centerPos);
        _skillButton->isPressed = false;

        Vec2 lastTouchLocationInNode = _skillButton->convertToNodeSpace(touch->getLocation());
        if (_skillButton->cancelButton->getBoundingBox().containsPoint(lastTouchLocationInNode)) {
            this->_aoeSprite->setVisible(false);
            _skillButton->cancelButton->setVisible(false);
            return;
        }

        performSkill(Vec2::ZERO);
    }
}

void ThunderFulminationA::performSkill(Vec2 target) {
    
        _skillButton->cancelButton->setVisible(false);
        _aoeSprite->setVisible(false);


        if (currentSkillCoolDown < 0) {

            auto player = dynamic_cast<Player*>(this->getParent());
            if (player->getCurrentMP() < manaCost) {
                player->noManaAlert();
                return;
            }
            UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/lightning_fire.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
            player->SwitchState(player->selectState);
            player->setCurrentMP(player->getCurrentMP() - manaCost);

        // Ly kch thc ca mn hnh
        Size visibleSize = Director::getInstance()->getVisibleSize();

        Vec2 applyPosition = this->getParent()->getParent()->convertToNodeSpace(target);
        // Thit lp kch thc ca sprite bng vi kch thc mn hnh
        _skillSprite->setScale(visibleSize.width / _skillSprite->getContentSize().width,
            visibleSize.height / _skillSprite->getContentSize().height);
        _skillSprite->setAnchorPoint(Vec2(0.5, 0.5));

        this->addChild(_skillSprite, -1);

        _skillSprite->runAction(Sequence::create(_skillAnimate,
            cocos2d::CallFunc::create([this]() {
                // Ly scene chnh t Director  kim tra c enemy no trong phm vi skill ko
                Scene* currentScene = Director::getInstance()->getRunningScene();
                if (currentScene) {
                    Game* game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
                    if (game) {
                        auto children = game->gameMap->getTiledMap()->getChildren();

                        for (const auto& child : children) {
                            auto enemy = dynamic_cast<Enemy*>(child);
                            if (enemy) {
                                auto player = dynamic_cast<Player*>(this->getParent());
                                //Kim tra xem enemy c nm trong vng nh hng ca skill hay khng
                                if (player->getPosition().distance(enemy->getPosition()) <= 300) {
                                    auto boss = dynamic_cast<Boss*>(enemy);
                                    if (boss && boss->currentState != boss->deadState) {
                                        boss->takeDamage(skillDamage);
                                        if (boss->getCurrentHP() == 0)player->gainExp(boss->getExpGain());
                                    }

                                    auto monster = dynamic_cast<NormalMonster*>(enemy);
                                    if (monster && monster->currentState != monster->deadState) {
                                        monster->takeDamage(skillDamage);
                                        if (monster->getCurrentHP() == 0)player->gainExp(monster->getExpGain());
                                    }
                                }
                            }
                        }
                    }
                }

                }), RemoveSelf::create(), nullptr));

        currentSkillCoolDown = skillCooldown;

        _iconSprite->setOpacity(70);

        coolDownCountLable->setVisible(true);
    }
}

void ThunderFulminationA::update(float dt) {
    if (currentSkillCoolDown >= 0) {
        currentSkillCoolDown -= dt;
        int coolDownToInt = std::floor(currentSkillCoolDown);
        if (coolDownToInt < 0 && coolDownCountLable->isVisible()) {
            _iconSprite->setOpacity(255);
            coolDownCountLable->setVisible(false);
        }

        coolDownCountLable->setString(StringUtils::format("%d", coolDownToInt));
    }
}