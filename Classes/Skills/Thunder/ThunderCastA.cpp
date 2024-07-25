#include "ThunderCastA.h"
#include "Player/Player.h"
#include "HelloWorldScene.h"
#include "Map/Map.h"
#include "Game/Game.h"
#include <string>
bool ThunderCastA::init() {

    skillTalent = new SkillTalent;
    skillTalent->skillName = "ThunderCastA";
    skillTalent->iconPath = "skill/SkillSprite/SkillIcon/Thunder/thunder_cast_a.png";
    skillTalent->description = "Active a Ring of Thunder, rotate around player and causing damage to enemies it affect in waves.";
    skillTalent->skillType = "AOE";
    skillTalent->unlockPoint = 20;
    skillTalent->upgradePoint = 20;
    skillTalent->parent = nullptr;
    skillTalent->children.push_back(SkillFactory::createSkill("ThunderShock")->getSkillTalent());
    skillTalent->isLock = true;
    skillTalent->levelRequired = 0;
    this->skillName = "ThunderCastA";
    //Skill Icon...
    _iconSprite = Sprite::create("skill/SkillSprite/SkillIcon/Thunder/thunder_cast_a.png");
    _iconSprite->setScale(0.1);
    _iconSprite->retain();

    
    //Skill Sprite
    _skillSprite = Sprite::createWithSpriteFrameName("thunder_cast_a (1).png");
    _skillSprite->setAnchorPoint(Vec2(0.5, 0.5));
    _skillSprite->setScale(1);
    _skillSprite->retain();

    //Skill Animate
    _skillAnimate = Animate::create(Engine::createAnimation2("thunder_cast_a", 30, 0.01));
    _skillAnimate->retain();

    //SkillButton
    _skillButton = SkillButton::create();
    _skillButton->setSkillButtonBorder(_iconSprite);
    _skillButton->retain();

    activeTime = 15;
    //SkillStats
    //Level
    currentlevelSkill = 1;
    maxLevelSkill = 5;
    // + Base Stats
    manaCost = 70;
    currentSkillCoolDown = 0;
    skillCooldown = 30;
    //- Buff Skill
    hpExtra = 0;
    mpExtra = 0;
    dmgExtra = 0;
    armorExtra = 0;
    //- Attack Skill
    damageRaitoOfPlayer = 0.5f;
    skillDamage = 50;

    // + Bonus Stats when level up
    skillCooldown_reduct = 1.5;
    //- Buff Skill
    hpExtra_bonus = 0;
    mpExtra_bonus = 0;
    dmgExtra_bonus = 0;
    armorExtra_bonus = 0;
    //- Attack Skill
    skillDamage_bonus = 10;

    this->coolDownCountLable = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 24);
    _skillButton->addChild(this->coolDownCountLable);
    this->coolDownCountLable->setVisible(false);

    //Effective Time Indicate
    skillEffectiveIndicate = Sprite::create(skillTalent->iconPath);
    skillEffectiveIndicate->setOpacity(150);
    timeEffectLabel = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 15);
    timeEffectLabel->setColor(Color3B::WHITE);
    skillEffectiveIndicate->setContentSize(Size(timeEffectLabel->getContentSize().height, timeEffectLabel->getContentSize().height));
    timeEffectLabel->setString("0");
    timeEffectLabel->setPosition(Vec2(skillEffectiveIndicate->getBoundingBox().size.width / 2, skillEffectiveIndicate->getBoundingBox().size.height / 2));
    skillEffectiveIndicate->addChild(timeEffectLabel, 1);
    skillEffectiveIndicate->retain();


    skillEffectiveIndicate->setPosition(Vec2(75, -50));
    skillEffectiveIndicate->setVisible(false);
    this->addChild(skillEffectiveIndicate, 10);

    // ng k s kin onTouch cho SkillButton
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(ThunderCastA::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(ThunderCastA::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(ThunderCastA::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, _skillButton);
    _touchListener = touchListener;


    return true;
}

bool ThunderCastA::onTouchBegan(Touch* touch, Event* event)
{

    Vec2 touchLocationInNode = _skillButton->convertToNodeSpace(touch->getLocation());

    if (_skillButton->skillButtonBtn->getBoundingBox().containsPoint(touchLocationInNode))
    {
        auto player = dynamic_cast<Player*>(this->getParent());
        if (player->getCurrentMP() < manaCost) {
            player->noManaAlert();
            return false;
        }

        if (currentSkillCoolDown <= 0) {
            _skillButton->isPressed = true;
            _skillButton->cancelButton->setVisible(true);

            return true;
        }
        else {
            NotificationManager::getInstance()->showMessageNotification(this->skillName + " on Cooldown", Vec2::ZERO, Color3B::WHITE, 10);
        }
    }
    return false;
}

void ThunderCastA::onTouchMoved(Touch* touch, Event* event)
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

void ThunderCastA::onTouchEnded(Touch* touch, Event* event)
{
    if (_skillButton->isPressed) {
        _skillButton->prevPosBeforeRelease = _skillButton->skillButtonBtn->getPosition();
        _skillButton->skillButtonBtn->setPosition(_skillButton->centerPos);
        _skillButton->isPressed = false;

        Vec2 lastTouchLocationInNode = _skillButton->convertToNodeSpace(touch->getLocation());
        if (_skillButton->cancelButton->getBoundingBox().containsPoint(lastTouchLocationInNode)) {
            _skillButton->cancelButton->setVisible(false);
            return;
        }

        performSkill(Vec2::ZERO);
        _skillButton->cancelButton->setVisible(false);
    }
}

void ThunderCastA::performSkill(Vec2 target) {
    
    if (currentSkillCoolDown <= 0) {
        auto player = dynamic_cast<Player*>(this->getParent());
        if (player->getCurrentMP() < manaCost) {
            player->noManaAlert();
            return;
        }
        lightningSkillOnId = Audio::getInstance()->play2d("sound/sounds effect/lighting_skill_on.mp3", true, SettingsData::getInstance()->getSoundSlider() / 100.0f);
        UserDefault::getInstance()->setIntegerForKey("loop_effect1", lightningSkillOnId);
        // Dng m thanh sau 15 giy
        cocos2d::Director::getInstance()->getScheduler()->schedule([this](float dt) {
            cocos2d::AudioEngine::stop(this->lightningSkillOnId);
            }, this, 0, 0, 15.0f, false, "stop_sound_skill_on_sound");

        schedule(CC_SCHEDULE_SELECTOR(ThunderCastA::updateCooldown), 1.0f);
        schedule(CC_SCHEDULE_SELECTOR(ThunderCastA::update), 0.5f);

        player->SwitchState(player->selectState);
        player->setCurrentMP(player->getCurrentMP() - manaCost);

        isActive = true;

        this->addChild(_skillSprite, -1);

        auto sqe = Sequence::create(Repeat::create(_skillAnimate, 50), RemoveSelf::create(), nullptr);
        _skillSprite->runAction(sqe);

        currentSkillCoolDown = skillCooldown;

        _iconSprite->setOpacity(70);

        coolDownCountLable->setVisible(true);
    }
}
void ThunderCastA::update(float dt) {
    if (isActive) {
        if (!skillEffectiveIndicate->isVisible()) {
            skillEffectiveIndicate->setVisible(true);
        }
        int timeEffectToInt = std::floor(activeTime);
        timeEffectLabel->setString(std::to_string(timeEffectToInt));
        activeTime -= dt;
        CCLOG("Active Time = %f", activeTime);

        // Ly scene chnh t Director  kim tra c enemy no trong phm vi skill ko
        Scene* currentScene = Director::getInstance()->getRunningScene();
        if (currentScene) {
            Game* game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
            if (game) {
                auto monsters = game->listOfMonster;
                //get Player
                auto player = dynamic_cast<Player*>(this->getParent());

                for (auto& monster : monsters) {
                    if (monster && !monster->isDead && monster->getPosition().distance(player->getPosition()) <= 70) {
                        monster->takeDamage((skillDamage + player->getEquipmentSkillDamage() + player->getAPDamage()));
                        ////Do effect
                        //if (player->getEquipment("Weapon")->getElement() == player->getEquipment("Weapon")->THUNDER) {
                        //    if (!monster->getChildByName("ThunderEffect")) {
                        //        //Skill Effect Sprite
                        //        auto effect = Sprite::createWithSpriteFrameName("thunder_spark (1).png");
                        //        effect->setName("ThunderEffect");
                        //        effect->setScale(0.2);

                        //        //Skill Effect Animate
                        //        auto animate = Animate::create(Engine::createAnimation2("thunder_spark", 30, 0.05));
                        //        monster->addChild(effect);

                        //        //Effect to Monster
                        //        effect->setPosition(Vec2(0, 40));
                        //        effect->runAction(RepeatForever::create(animate));

                        //        schedule(CC_SCHEDULE_SELECTOR(ThunderCastA::updateEffect), 1.0f);
                        //        effectTime = 5.0f;
                        //    }
                        //}
                        if (monster->getCurrentHP() <= 0) {
                            monster->die();
                        }
                    }
                }

                auto boss = game->boss;
                if (boss && !boss->isDead && boss->getPosition().distance(player->getPosition()) <= 70) {
                    boss->takeDamage((skillDamage + player->getEquipmentSkillDamage() + player->getAPDamage()));
                    ////Do effect
                    //if (player->getEquipment("Weapon")->getElement() == player->getEquipment("Weapon")->THUNDER) {
                    //    if (!boss->getChildByName("ThunderEffect")) {
                    //        //Skill Effect Sprite
                    //        auto effect = Sprite::createWithSpriteFrameName("thunder_spark (1).png");
                    //        effect->setName("ThunderEffect");
                    //        effect->setScale(0.2);

                    //        //Skill Effect Animate
                    //        auto animate = Animate::create(Engine::createAnimation2("thunder_spark", 30, 0.05));
                    //        boss->addChild(effect);

                    //        //Effect to Monster
                    //        effect->setPosition(Vec2(0, 40));
                    //        effect->runAction(RepeatForever::create(animate));

                    //        schedule(CC_SCHEDULE_SELECTOR(ThunderCastA::updateEffect), 1.0f);
                    //        effectTime = 5.0f;
                    //    }
                    //}
                    if (boss->getCurrentHP() <= 0) {
                        boss->die();
                    }
                }
            }
        }

        //Deactive Skill nu nh ht hiu lc 
        if (activeTime < 0) {
            isActive = false;
            activeTime = 15;
            skillEffectiveIndicate->setVisible(false);
        }
    }
    else unschedule(CC_SCHEDULE_SELECTOR(ThunderCastA::update));

}

void ThunderCastA::updateCooldown(float dt) {
    if (currentSkillCoolDown >= 0) {
        currentSkillCoolDown -= dt;
        int coolDownToInt = std::floor(currentSkillCoolDown);
        if (coolDownToInt < 0 && coolDownCountLable->isVisible()) {
            _iconSprite->setOpacity(255);
            coolDownCountLable->setVisible(false);
            unschedule(CC_SCHEDULE_SELECTOR(ThunderCastA::updateCooldown));
        }

        coolDownCountLable->setString(StringUtils::format("%d", coolDownToInt));
    }
}
void ThunderCastA::updateEffect(float dt) {
    //if (effectTime >= 0) {
    //    auto player = dynamic_cast<Player*>(this->getParent());

    //    effectTime -= dt;
    //    //Check every monster in map
    //    Scene* currentScene = Director::getInstance()->getRunningScene();
    //    if (currentScene) {
    //        Game* game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
    //        if (game) {
    //            auto monsters = game->listOfMonster;
    //            for (auto& monster : monsters) {
    //                if (monster) {
    //                    //Delete effect sprite when no longer effect
    //                    if (effectTime < 0) {
    //                        if (monster->getChildByName("ThunderEffect"))monster->removeChildByName("ThunderEffect");
    //                    }
    //                    else {
    //                        if (monster->getChildByName("ThunderEffect")) {
    //                            monster->takeDamage(0.05 * (skillDamage + player->getEquipmentSkillDamage() + player->getAPDamage()));
    //                            if (monster->getCurrentHP() <= 0) {
    //                                if (monster->getChildByName("ThunderEffect"))monster->removeChildByName("ThunderEffect");
    //                            }
    //                        }
    //                    }
    //                }
    //            }


    //            auto boss = game->boss;
    //            if (boss) {
    //                //Delete effect sprite when no longer effect
    //                if (effectTime < 0) {
    //                    if (boss->getChildByName("ThunderEffect"))boss->removeChildByName("ThunderEffect");
    //                }
    //                else {
    //                    if (boss->getChildByName("ThunderEffect")) {
    //                        boss->takeDamage(0.05 * (skillDamage + player->getEquipmentSkillDamage() + player->getAPDamage()));
    //                        if (boss->getCurrentHP() <= 0) {
    //                            if (boss->getChildByName("ThunderEffect"))boss->removeChildByName("ThunderEffect");
    //                        }
    //                    }
    //                }
    //            }


    //        }
    //    }
    //}
    //else unschedule(CC_SCHEDULE_SELECTOR(ThunderCastA::updateEffect));

}