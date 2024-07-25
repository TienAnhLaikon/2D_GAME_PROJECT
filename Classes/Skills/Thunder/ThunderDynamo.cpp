#include "ThunderDynamo.h"
#include "Player/Player.h"
#include "HelloWorldScene.h"
#include "Map/Map.h"
#include <string>
bool ThunderDynamo::init() {
    skillTalent = new SkillTalent;
    skillTalent->skillName = "ThunderDynamo";
    skillTalent->iconPath = "skill/SkillSprite/SkillIcon/Thunder/thunder_dynamo.png";
    skillTalent->description = "Shoot a thunder ball with a direction and deal damage to all enemy get hit and then go back to the cast Position";
    skillTalent->skillType = "Skill Shot & AOE";
    skillTalent->unlockPoint = 30;
    skillTalent->upgradePoint = 30;
    skillTalent->parent = nullptr;
    skillTalent->isLock = true;
    skillTalent->levelRequired = 0;
    skillTalent->children.push_back(SkillFactory::createSkill("ThunderCurrent")->getSkillTalent());
    this->skillName = "ThunderDynamo";
    //AOE Range
    _aoeSprite = Sprite::create("skill/AOERangeSprite/SkillShotTarget.png");
    _aoeSprite->setOpacity(100);
    _aoeSprite->setScale(0.45);
    _aoeSprite->setAnchorPoint(Vec2(0, 0.5));
    this->addChild(_aoeSprite);
    _aoeSprite->setVisible(false);

    //Skill Icon...
    _iconSprite = Sprite::create("skill/SkillSprite/SkillIcon/Thunder/thunder_dynamo.png");
    _iconSprite->setScale(0.1);
    _iconSprite->retain();

    
    //Skill Sprite
    _skillSprite = Sprite::createWithSpriteFrameName("thunder_dynamo (1).png");
    _skillSprite->setAnchorPoint(Vec2(0.5, 0.5));
    _skillSprite->retain();

    //Skill Animate
    _skillAnimate = Animate::create(Engine::createAnimation2("thunder_dynamo", 90, 0.02));
    _skillAnimate->retain();

    //Physics For Skill
    auto physics = PhysicsBody::createBox(Size(20, 20), PhysicsMaterial(1.0f, 0.0f, 0.0f));
    physics->setGravityEnable(false);
    physics->setRotationEnable(false);

    //SkillButton
    _skillButton = SkillButton::create();
    _skillButton->setSkillButtonBorder(_iconSprite);
    _skillButton->retain();

    //SkillStats
    //Level
    currentlevelSkill = 1;
    maxLevelSkill = 5;
    // + Base Stats
    manaCost = 70;
    currentSkillCoolDown = 0;
    skillCooldown = 15;
    //- Buff Skill
    hpExtra = 0;
    mpExtra = 0;
    dmgExtra = 0;
    armorExtra = 0;
    //- Attack Skill
    damageRaitoOfPlayer = 0.5f;
    skillDamage = 100;

    // + Bonus Stats when level up
    skillCooldown_reduct = 1.5;
    //- Buff Skill
    hpExtra_bonus = 0;
    mpExtra_bonus = 0;
    dmgExtra_bonus = 0;
    armorExtra_bonus = 0;
    //- Attack Skill
    skillDamage_bonus = 30;

    this->coolDownCountLable = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 24);
    _skillButton->addChild(this->coolDownCountLable);
    this->coolDownCountLable->setVisible(false);

    // ng k s kin onTouch cho SkillButton
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(ThunderDynamo::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(ThunderDynamo::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(ThunderDynamo::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, _skillButton);
    _touchListener = touchListener;

    return true;
}

bool ThunderDynamo::onTouchBegan(Touch* touch, Event* event)
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

void ThunderDynamo::onTouchMoved(Touch* touch, Event* event)
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

        // Calculate the angle between the vector and the x-axis
        float angleRadians = atan2(direction.y, direction.x);
        float angleDegrees = CC_RADIANS_TO_DEGREES(-angleRadians); // Convert radians to degrees

        CCLOG("Angle: %f", angleDegrees);
        // Rotate the sprite to face the direction
        _aoeSprite->setRotation(angleDegrees);
        lastAngleRotate = angleDegrees;

        dir = direction;

    }
}

void ThunderDynamo::onTouchEnded(Touch* touch, Event* event)
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

        Vec2 pos = this->convertToWorldSpace(this->_aoeSprite->getPosition());
        _skillSprite->setRotation(lastAngleRotate);
        performSkill(pos);
        this->_aoeSprite->setVisible(false);
        _skillButton->cancelButton->setVisible(false);
    }
}

void ThunderDynamo::performSkill(Vec2 target) {
   
    if (currentSkillCoolDown <= 0) {

        auto player = dynamic_cast<Player*>(this->getParent());
        if (player->getCurrentMP() < manaCost) {
            player->noManaAlert();
            return;
        }
        UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/lightning_fire.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));

        if (lastAngleRotate >= -45 && lastAngleRotate < 0) { player->setCurrentDirection(1); } //E
        if (lastAngleRotate >= -90 && lastAngleRotate < -45) { player->setCurrentDirection(2); } //NE
        if (lastAngleRotate >= -135 && lastAngleRotate < -90) { player->setCurrentDirection(3); } //N
        if (lastAngleRotate >= -180 && lastAngleRotate < -135) { player->setCurrentDirection(4); } //NW
        if (lastAngleRotate >= 135 && lastAngleRotate < 180) { player->setCurrentDirection(5); } //W
        if (lastAngleRotate >= 90 && lastAngleRotate < 135) { player->setCurrentDirection(6); } //SW
        if (lastAngleRotate >= 45 && lastAngleRotate < 90) { player->setCurrentDirection(7); } //S
        if (lastAngleRotate >= 0 && lastAngleRotate < 45) { player->setCurrentDirection(8); } //SE

        schedule(CC_SCHEDULE_SELECTOR(ThunderDynamo::update), 0.5f);
        schedule(CC_SCHEDULE_SELECTOR(ThunderDynamo::updateCooldown), 1.0f);

        player->SwitchState(player->selectState);
        player->setCurrentMP(player->getCurrentMP() - manaCost);

        Vec2 applyPosition = this->getParent()->getParent()->convertToNodeSpace(target);
        _skillSprite->setPosition(applyPosition);
        _skillSprite->setScale(0.5);
        auto scene = this->getParent()->getScene();
        //if(_skillSprite->getParent())
        scene->addChild(_skillSprite);

        dir.normalize();
        Vec2 tar = _skillSprite->getPosition() + dir * _aoeSprite->getBoundingBox().size.width;

        auto moveTo = MoveTo::create(0.8, tar);

        auto spawn = Spawn::create(moveTo, _skillAnimate, nullptr);

        auto moveBack = MoveTo::create(0.8, this->getParent()->getPosition());

        auto spawn1 = Spawn::create(moveBack, _skillAnimate, nullptr);

        _skillSprite->runAction(Sequence::create(spawn,spawn1, RemoveSelf::create(), nullptr));

        currentSkillCoolDown = skillCooldown;

        _iconSprite->setOpacity(70);

        coolDownCountLable->setVisible(true);
    }
}

void ThunderDynamo::update(float dt) {
    // Ly scene chnh t Director  kim tra c enemy no trong phm vi skill ko
    if (_skillSprite->getParent()) {
        Scene* currentScene = Director::getInstance()->getRunningScene();
        if (currentScene) {
            Game* game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
            if (game) {
                auto monsters = game->listOfMonster;
                //get Player
                auto player = dynamic_cast<Player*>(this->getParent());

                for (auto& monster : monsters) {
                    if (monster && !monster->isDead && monster->getPosition().distance(_skillSprite->getPosition()) <= 60) {
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

                        //        schedule(CC_SCHEDULE_SELECTOR(ThunderDynamo::updateEffect), 1.0f);
                        //        effectTime = 5.0f;
                        //    }
                        //}
                        if (monster->getCurrentHP() <= 0) {
                            monster->die();
                        }
                    }
                }
                //Boss
                auto boss = game->boss;
                if (boss && !boss->isDead && boss->getPosition().distance(_skillSprite->getPosition()) <= 60) {
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

                    //        schedule(CC_SCHEDULE_SELECTOR(ThunderDynamo::updateEffect), 1.0f);
                    //        effectTime = 5.0f;
                    //    }
                    //}
                    if (boss->getCurrentHP() <= 0) {
                        boss->die();
                    }
                }
            }
        }
    }
    else unschedule(CC_SCHEDULE_SELECTOR(ThunderDynamo::update));
}

void ThunderDynamo::updateCooldown(float dt) {
    if (currentSkillCoolDown >= 0) {
        currentSkillCoolDown -= dt;
        int coolDownToInt = std::floor(currentSkillCoolDown);
        if (coolDownToInt < 0 && coolDownCountLable->isVisible()) {
            _iconSprite->setOpacity(255);
            coolDownCountLable->setVisible(false);
            unschedule(CC_SCHEDULE_SELECTOR(ThunderDynamo::updateCooldown));
        }

        coolDownCountLable->setString(StringUtils::format("%d", coolDownToInt));
    }
}
void ThunderDynamo::updateEffect(float dt) {
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
    //else unschedule(CC_SCHEDULE_SELECTOR(ThunderDynamo::updateEffect));

}