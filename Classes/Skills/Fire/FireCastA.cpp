#include "FireCastA.h"
#include "Player/Player.h"
#include "Enemies/Enemy.h"
#include "HelloWorldScene.h"
#include "Map/Map.h"
#include "Game/Game.h"
#include <string>
bool FireCastA::init() {

    skillTalent = new SkillTalent;
    skillTalent->skillName = "FireCastA";
    skillTalent->iconPath = "skill/SkillSprite/SkillIcon/Fire/fire_cast_a.png";
    skillTalent->unlockPoint = 20;
    skillTalent->upgradePoint = 20;
    skillTalent->description = "Active a Fire ring to player and Deal Damage to enemies around.";
    skillTalent->skillType = "AOE";
    skillTalent->parent = nullptr;
    skillTalent->children.push_back(SkillFactory::createSkill("FireCombust")->getSkillTalent()); 
    skillTalent->isLock = true;
    skillTalent->levelRequired = 3;
    this->skillName = "FireCastA";
    //Skill Icon...
    _iconSprite = Sprite::create(skillTalent->iconPath);
    _iconSprite->setScale(0.1);
    _iconSprite->retain();

    
    //Skill Sprite
    _skillSprite = Sprite::createWithSpriteFrameName("fire_cast_a (1).png");
    _skillSprite->setAnchorPoint(Vec2(0.5, 0.5));
    _skillSprite->setScale(1);
    _skillSprite->retain();

    //Skill Animate
    _skillAnimate = Animate::create(Engine::createAnimation2("fire_cast_a", 30, 0.01));
    _skillAnimate->retain();

    //SkillButton
    _skillButton = SkillButton::create();
    _skillButton->setSkillButtonBorder(_iconSprite);
    _skillButton->retain();

    activeTime = 15;
    //SkillStats
    //Level
    this->currentlevelSkill = 1;
    this->maxLevelSkill = 5;
    // + Base Stats
    this->manaCost = 70;
    this->currentSkillCoolDown = 0;
    this->skillCooldown = 30;
    //- Buff Skill
    this->hpExtra = 0;
    this->mpExtra = 0;
    this->dmgExtra = 0;
    this->armorExtra = 0;
    //- Attack Skill
    this->damageRaitoOfPlayer = 0.45;
    this->skillDamage = 50;

    // + Bonus Stats when level up
    this->skillCooldown_reduct = 2;
    //- Buff Skill
    this->hpExtra_bonus = 0;
    this->mpExtra_bonus = 0;
    this->dmgExtra_bonus = 0;
    this->armorExtra_bonus = 0;
    //- Attack Skill
    this->skillDamage_bonus = 10;

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

    skillEffectiveIndicate->setPosition(Vec2(-75, -50));
    skillEffectiveIndicate->setVisible(false);
    this->addChild(skillEffectiveIndicate, 10);

    // ng k s kin onTouch cho SkillButton
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(FireCastA::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(FireCastA::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(FireCastA::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, _skillButton);
    _touchListener = touchListener;

    return true;
}

bool FireCastA::onTouchBegan(Touch* touch, Event* event)
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


void FireCastA::onTouchMoved(Touch* touch, Event* event)
{
        if (_skillButton->isPressed)
        {
            //CCLOG("Moving");
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

void FireCastA::onTouchEnded(Touch* touch, Event* event)
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

void FireCastA::performSkill(Vec2 target) {
    
    if (currentSkillCoolDown <= 0) {

        auto player = dynamic_cast<Player*>(this->getParent());
        if (player->getCurrentMP() < manaCost) {
            player->noManaAlert();
            return;
        }
        flamesSkillOnId = Audio::getInstance()->play2d("sound/sounds effect/flames_skill_on.mp3", true, SettingsData::getInstance()->getSoundSlider() / 100.0f);
        UserDefault::getInstance()->setIntegerForKey("loop_effect3", flamesSkillOnId);

        // Dng m thanh sau 15 giy
        cocos2d::Director::getInstance()->getScheduler()->schedule([this](float dt) {
            cocos2d::AudioEngine::stop(this->flamesSkillOnId);
            }, this, 0, 0, 15.0f, false, "stop_flames_skill_sound");

        schedule(CC_SCHEDULE_SELECTOR(FireCastA::update), 0.5f);
        schedule(CC_SCHEDULE_SELECTOR(FireCastA::updateCooldown), 1.0f);

        player->SwitchState(player->selectState);
        player->setCurrentMP(player->getCurrentMP() - manaCost);
        isActive = true;

        this->addChild(_skillSprite, -1);

        auto sqe = Sequence::create(Repeat::create(_skillAnimate, 50), RemoveSelf::create(), nullptr);
        _skillSprite->runAction(sqe);

        currentSkillCoolDown = skillCooldown * player->getCDR();

        _iconSprite->setOpacity(70);

        coolDownCountLable->setVisible(true);
    }
}
void FireCastA::update(float dt) {
    
    if (isActive) {
        if (!skillEffectiveIndicate->isVisible()) {
            skillEffectiveIndicate->setVisible(true);
        }
        int timeEffectToInt = std::floor(activeTime);
        timeEffectLabel->setString(std::to_string(timeEffectToInt));
        activeTime -= dt;
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
                        //if (player->getEquipment("Weapon")->getElement() == player->getEquipment("Weapon")->FIRE) {
                        //    if (!monster->getChildByName("FireEffect")) {
                        //        //Skill Effect Sprite
                        //        auto effect = Sprite::createWithSpriteFrameName("fire_sear (1).png");
                        //        effect->setName("FireEffect");
                        //        effect->setScale(0.2);

                        //        //Skill Effect Animate
                        //        auto animate = Animate::create(Engine::createAnimation2("fire_sear", 30, 0.05));
                        //        monster->addChild(effect);

                        //        //Effect to Monster
                        //        effect->setPosition(Vec2(0, 40));
                        //        effect->runAction(RepeatForever::create(animate));

                        //        
                        //        effectTime = 5.0f;
                        //        schedule(CC_SCHEDULE_SELECTOR(FireCastA::updateEffect), 1.0f);
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
                    //if (player->getEquipment("Weapon")->getElement() == player->getEquipment("Weapon")->FIRE) {
                    //    if (!boss->getChildByName("FireEffect")) {
                    //        //Skill Effect Sprite
                    //        auto effect = Sprite::createWithSpriteFrameName("fire_sear (1).png");
                    //        effect->setName("FireEffect");
                    //        effect->setScale(0.2);

                    //        //Skill Effect Animate
                    //        auto animate = Animate::create(Engine::createAnimation2("fire_sear", 30, 0.05));
                    //        boss->addChild(effect);

                    //        //Effect to Monster
                    //        effect->setPosition(Vec2(0, 40));
                    //        effect->runAction(RepeatForever::create(animate));

                    //       
                    //        effectTime = 5.0f;
                    //        schedule(CC_SCHEDULE_SELECTOR(FireCastA::updateEffect), 1.0f);
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
            skillEffectiveIndicate->setVisible(false);
            isActive = false;
            activeTime = 15;
            unschedule(CC_SCHEDULE_SELECTOR(FireCastA::update));
        }
    }
    
}

void FireCastA::updateCooldown(float dt) {
    if (currentSkillCoolDown >= 0) {
        currentSkillCoolDown -= dt;
        int coolDownToInt = std::floor(currentSkillCoolDown);
        if (coolDownToInt < 0 && coolDownCountLable->isVisible()) {
            _iconSprite->setOpacity(255);
            coolDownCountLable->setVisible(false);
            unschedule(CC_SCHEDULE_SELECTOR(FireCastA::updateCooldown));
        }

        coolDownCountLable->setString(StringUtils::format("%d", coolDownToInt));
    }
}

void FireCastA::updateEffect(float dt) {
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
    //                        if (monster->getChildByName("FireEffect"))monster->removeChildByName("FireEffect");
    //                    }
    //                    else {
    //                        if (monster->getChildByName("FireEffect")) {
    //                            monster->takeDamage(0.05 * (skillDamage + player->getEquipmentSkillDamage() + player->getAPDamage()));
    //                            if (monster->getCurrentHP() <= 0) {
    //                                if (monster->getChildByName("FireEffect"))monster->removeChildByName("FireEffect");
    //                            }
    //                        }
    //                    }
    //                }
    //            }


    //            //auto boss = game->boss;
    //            //if (boss) {
    //            //    //Delete effect sprite when no longer effect
    //            //    if (effectTime < 0) {
    //            //        if (boss->getChildByName("FireEffect"))boss->removeChildByName("FireEffect");
    //            //    }
    //            //    else {
    //            //        if (boss->getChildByName("FireEffect")) {
    //            //            boss->takeDamage(0.05 * (skillDamage + player->getEquipmentSkillDamage() + player->getAPDamage()));
    //            //            if (boss->getCurrentHP() <= 0) {
    //            //                if (boss->getChildByName("FireEffect"))boss->removeChildByName("FireEffect");
    //            //            }
    //            //        }
    //            //    }
    //            //}


    //        }
    //    }
    //}
    //else unschedule(CC_SCHEDULE_SELECTOR(FireCastA::updateEffect));

}