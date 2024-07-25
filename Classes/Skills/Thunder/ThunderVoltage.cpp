#include "ThunderVoltage.h"
#include "Player/Player.h"
#include "HelloWorldScene.h"
#include "Map/Map.h"
#include <string>
bool ThunderVoltage::init() {
    skillTalent = new SkillTalent;
    skillTalent->skillName = "ThunderVoltage";
    skillTalent->iconPath = "skill/SkillSprite/SkillIcon/Thunder/thunder_voltage.png";
    skillTalent->description = "Shoot a Thunder bullet to a direction. If it collide to an enemy, it will explode to an area of thunder, dealing damage to them.";
    skillTalent->skillType = "AOE";
    skillTalent->unlockPoint = 30;
    skillTalent->parent = nullptr;
    skillTalent->upgradePoint = 30;
    skillTalent->isLock = true;
    skillTalent->levelRequired = 0;
    this->skillName = "ThunderVoltage";
    //AOE Range
    _aoeSprite = Sprite::create("skill/AOERangeSprite/SkillShotTarget.png");
    _aoeSprite->setOpacity(100);
    _aoeSprite->setScale(0.4);
    _aoeSprite->setAnchorPoint(Vec2(0, 0.5));
    this->addChild(_aoeSprite);
    _aoeSprite->setVisible(false);

    //Skill Icon...
    _iconSprite = Sprite::create("skill/SkillSprite/SkillIcon/Thunder/thunder_voltage.png");
    _iconSprite->setScale(0.1);
    _iconSprite->retain();


    //Skill Sprite
    _skillSprite = Sprite::createWithSpriteFrameName("thunder_voltage_flying (1).png");
    _skillSprite->setAnchorPoint(Vec2(0.5, 0.5));
    _skillSprite->retain();

    //Skill Animate
    _flyingAnimate = Animate::create(Engine::createAnimation2("thunder_voltage_flying", 26, 0.025));
    _flyingAnimate->retain();

    _explodeAnimate = Animate::create(Engine::createAnimation2("thunder_voltage_explode", 60, 0.015));
    _explodeAnimate->retain();
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
    skillDamage = 150;

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

    //Phics
    auto physicsBody = PhysicsBody::createBox(Size(40, 20), PhysicsMaterial(1.0f, 0.0f, 0.0f));
    physicsBody->setRotationEnable(false);
    physicsBody->setGravityEnable(false);

    physicsBody->setCategoryBitmask(0x01); // Category của FireBall
    physicsBody->setCollisionBitmask(0x02); // Va chạm với quái
    physicsBody->setContactTestBitmask(0x02); // Kiểm tra va chạm với quái

    _skillSprite->setPhysicsBody(physicsBody);

    // ng k s kin onTouch cho SkillButton
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(ThunderVoltage::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(ThunderVoltage::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(ThunderVoltage::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, _skillButton);
    _touchListener = touchListener;

    // To b lng nghe s kin va chm
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(ThunderVoltage::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    return true;
}

bool ThunderVoltage::onContactBegin(PhysicsContact& contact) {
    auto spriteA = contact.getShapeA()->getBody()->getNode();
    auto spriteB = contact.getShapeB()->getBody()->getNode();

    // Kim tra va chm gia spriteA v spriteB
    if ((spriteA == _skillSprite && dynamic_cast<Enemy*>(spriteB)) ||
        (spriteB == _skillSprite && dynamic_cast<Enemy*>(spriteA))) {
        auto enemyA = dynamic_cast<Enemy*>(spriteA);
        auto enemyB = dynamic_cast<Enemy*>(spriteB);

        auto player = dynamic_cast<Player*>(this->getParent());

        if (enemyA) {
            auto boss = dynamic_cast<Boss*>(enemyA);
            if (boss && boss->currentState != boss->deadState) {
                boss->takeDamage(skillDamage + player->getEquipmentSkillDamage() + player->getAPDamage());
                if (boss->getCurrentHP() == 0) {
                    player->gainExp(boss->getExpGain());
                    boss->die();
                }
            }

            auto monster = dynamic_cast<NormalMonster*>(enemyA);
            if (monster && monster->currentState != monster->deadState) {
                monster->takeDamage(skillDamage + player->getEquipmentSkillDamage() + player->getAPDamage());
                if (monster->getCurrentHP() == 0) {
                    player->gainExp(monster->getExpGain());
                    monster->die();
                }
            }
        }
        if (enemyB) {
            auto boss = dynamic_cast<Boss*>(enemyB);
            if (boss && boss->currentState != boss->deadState) {
                boss->takeDamage(skillDamage + player->getEquipmentSkillDamage() + player->getAPDamage());
                if (boss->getCurrentHP() == 0) {
                    player->gainExp(boss->getExpGain());
                    boss->die();
                }
            }

            auto monster = dynamic_cast<NormalMonster*>(enemyB);
            if (monster && monster->currentState != monster->deadState) {
                monster->takeDamage(skillDamage + player->getEquipmentSkillDamage() + player->getAPDamage());
                if (monster->getCurrentHP() == 0) {
                    player->gainExp(monster->getExpGain());
                    monster->die();
                }
            }
        }

        if (_skillSprite->getNumberOfRunningActions() > 0)_skillSprite->stopAllActions();
        _skillSprite->runAction(Sequence::create(_explodeAnimate, RemoveSelf::create(), nullptr));
        // Ly scene chnh t Director  kim tra c enemy no trong phm vi skill ko
        Scene* currentScene = Director::getInstance()->getRunningScene();
        if (currentScene) {
            Game* game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
            if (game) {
                auto children = game->gameMap->getTiledMap()->getChildren();

                for (const auto& child : children) {
                    auto enemy = dynamic_cast<Enemy*>(child);
                    if (enemy) {
                        //Kim tra xem enemy c nm trong vng nh hng ca skill hay khng
                        if (_skillSprite->getPosition().distance(enemy->getPosition()) <= 60) {
                            auto boss = dynamic_cast<Boss*>(enemy);
                            if (boss && boss->currentState != boss->deadState) {
                                boss->takeDamage(skillDamage + player->getEquipmentSkillDamage() + player->getAPDamage());
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

                                //        schedule(CC_SCHEDULE_SELECTOR(ThunderVoltage::updateEffect), 1.0f);
                                //        effectTime = 5.0f;
                                //    }
                                //}
                                if (boss->getCurrentHP() == 0) {
                                    boss->die();
                                }
                            }

                            auto monster = dynamic_cast<NormalMonster*>(enemy);
                            if (monster && monster->currentState != monster->deadState) {
                                monster->takeDamage(skillDamage + player->getEquipmentSkillDamage() + player->getAPDamage());
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

                                //        schedule(CC_SCHEDULE_SELECTOR(ThunderVoltage::updateEffect), 1.0f);
                                //        effectTime = 5.0f;
                                //    }
                                //}
                                if (monster->getCurrentHP() == 0) {
                                    monster->die();
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return true;
}

bool ThunderVoltage::onTouchBegan(Touch* touch, Event* event)
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

void ThunderVoltage::onTouchMoved(Touch* touch, Event* event)
{
    if (_skillButton->isPressed)
    {
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

        // Rotate the sprite to face the direction
        _aoeSprite->setRotation(angleDegrees);
        lastAngleRotate = angleDegrees;

        dir = direction;

    }
}

void ThunderVoltage::onTouchEnded(Touch* touch, Event* event)
{
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

void ThunderVoltage::performSkill(Vec2 target) {

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

        player->SwitchState(player->selectState);
        player->setCurrentMP(player->getCurrentMP() - manaCost);
        schedule(CC_SCHEDULE_SELECTOR(ThunderVoltage::update), 1.0f);

        Vec2 applyPosition = this->getParent()->getParent()->convertToNodeSpace(target);
        _skillSprite->setPosition(applyPosition);
        _skillSprite->setScale(0.7);
        auto scene = this->getParent()->getScene();
        //if(_skillSprite->getParent())
        scene->addChild(_skillSprite);

        dir.normalize();
        Vec2 tar = _skillSprite->getPosition() + dir * _aoeSprite->getBoundingBox().size.width;

        auto moveTo = MoveTo::create(1.0f, tar);

        auto spawn = Spawn::create(moveTo, _flyingAnimate, nullptr);

        _skillSprite->runAction(Sequence::create(spawn, RemoveSelf::create(), nullptr));

        currentSkillCoolDown = skillCooldown;

        _iconSprite->setOpacity(70);

        coolDownCountLable->setVisible(true);
    }
    else {
        NotificationManager::getInstance()->showMessageNotification(this->skillName + " on Cooldown", Vec2::ZERO, Color3B::WHITE, 10);
    }
}

void ThunderVoltage::update(float dt) {
    if (currentSkillCoolDown >= 0) {
        currentSkillCoolDown -= dt;
        int coolDownToInt = std::floor(currentSkillCoolDown);
        if (coolDownToInt < 0 && coolDownCountLable->isVisible()) {
            _iconSprite->setOpacity(255);
            coolDownCountLable->setVisible(false);
            unschedule(CC_SCHEDULE_SELECTOR(ThunderVoltage::update));
        }

        coolDownCountLable->setString(StringUtils::format("%d", coolDownToInt));
    }
}

void ThunderVoltage::updateEffect(float dt) {
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
    //else unschedule(CC_SCHEDULE_SELECTOR(ThunderVoltage::updateEffect));

}