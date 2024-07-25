#include "FireBall.h"
#include "Player/Player.h"
#include "HelloWorldScene.h"
#include "Map/Map.h"
#include <string>
bool FireBall::init() {

    skillTalent = new SkillTalent();
    skillTalent->skillName = "FireBall";
    skillTalent->iconPath = "skill/SkillSprite/SkillIcon/Fire/fire_ball.png";
    skillTalent->description = "Shoot a fire ball with a direction and deal damage to all enemy get hit.";
    skillTalent->skillType = "Skill Shot & AOE";
    skillTalent->unlockPoint = 0;
    skillTalent->upgradePoint = 10;
    skillTalent->parent = nullptr;
    skillTalent->children.push_back(SkillFactory::createSkill("FireCastA")->getSkillTalent());
    skillTalent->children.push_back(SkillFactory::createSkill("FireCastB")->getSkillTalent());
    skillTalent->children.push_back(SkillFactory::createSkill("FireConflagration")->getSkillTalent());
    skillTalent->isLock = true;
    skillTalent->levelRequired = 1;
    this->skillName = "FireBall";
    //AOE Range
    _aoeSprite = Sprite::create("skill/AOERangeSprite/SkillShotTarget.png");
    _aoeSprite->setScale(0.35);
    _aoeSprite->setOpacity(100);
    _aoeSprite->setAnchorPoint(Vec2(0, 0.5));
    this->addChild(_aoeSprite);
    _aoeSprite->setVisible(false);

    //Skill Icon...
    _iconSprite = Sprite::create(skillTalent->iconPath);
    _iconSprite->setScale(0.1);
    _iconSprite->retain();

    
    //Skill Sprite
    _skillSprite = Sprite::createWithSpriteFrameName("fire_ball (1).png");
    _skillSprite->setAnchorPoint(Vec2(0.5, 0.5));
    _skillSprite->retain();

    //Skill Animate
    _flyingAnimate = Animate::create(Engine::createAnimation2("fire_ball", 30, 0.025));
    _flyingAnimate->retain();



    //SkillButton
    _skillButton = SkillButton::create();
    _skillButton->setSkillButtonBorder(_iconSprite);
    _skillButton->retain();

    //SkillStats
    //Level
    currentlevelSkill = 1;
    maxLevelSkill = 5;
    // + Base Stats
    manaCost = 50;
    currentSkillCoolDown = 0;
    skillCooldown = 10;
    //- Buff Skill
    hpExtra = 0;
    mpExtra = 0;
    dmgExtra = 0;
    armorExtra = 0;
    //- Attack Skill
    damageRaitoOfPlayer = 0.5f;
    skillDamage = 120;

    // + Bonus Stats when level up
    skillCooldown_reduct = 1.5;
    //- Buff Skill
    hpExtra_bonus = 0;
    mpExtra_bonus = 0;
    dmgExtra_bonus = 0;
    armorExtra_bonus = 0;
    //- Attack Skill
    skillDamage_bonus = 20;

    //Phics
    physicsBody = PhysicsBody::createCircle(30, PhysicsMaterial(1.0f, 0.0f, 0.0f));
    physicsBody->setRotationEnable(false);
    physicsBody->setGravityEnable(false);

    physicsBody->setCategoryBitmask(0x01); // Category của FireBall
    physicsBody->setCollisionBitmask(0x02); // Va chạm với quái
    physicsBody->setContactTestBitmask(0x02); // Kiểm tra va chạm với quái


    physicsBody->retain();


    this->coolDownCountLable = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 24);
    _skillButton->addChild(this->coolDownCountLable);
    this->coolDownCountLable->setVisible(false);

    // ng k s kin onTouch cho SkillButton
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(FireBall::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(FireBall::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(FireBall::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, _skillButton);
    _touchListener = touchListener;


    return true;
}

bool FireBall::onTouchBegan(Touch* touch, Event* event)
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

void FireBall::onTouchMoved(Touch* touch, Event* event)
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

            //CCLOG("Angle: %f", angleDegrees);
            // Rotate the sprite to face the direction
            _aoeSprite->setRotation(angleDegrees);
            lastAngleRotate = angleDegrees;

            dir = direction;
        }
}

void FireBall::onTouchEnded(Touch* touch, Event* event)
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

void FireBall::performSkill(Vec2 target) {
    if (currentSkillCoolDown <= 0) {
       
        auto player = dynamic_cast<Player*>(this->getParent());

        if (player->getCurrentMP() < manaCost) {
            player->noManaAlert();
            return;
        }
        _skillSprite->setPhysicsBody(physicsBody);
        UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/flames_fire.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));

        schedule(CC_SCHEDULE_SELECTOR(FireBall::update), 0.05f);
        schedule(CC_SCHEDULE_SELECTOR(FireBall::updateCooldown), 1.0f);

        player->setCurrentMP(player->getCurrentMP() - manaCost);

        if (lastAngleRotate >= -45 && lastAngleRotate < 0) { player->setCurrentDirection(1); } //E
        if (lastAngleRotate >= -90 && lastAngleRotate < -45) { player->setCurrentDirection(2); } //NE
        if (lastAngleRotate >= -135 && lastAngleRotate < -90) { player->setCurrentDirection(3); } //N
        if (lastAngleRotate >= -180 && lastAngleRotate < -135) { player->setCurrentDirection(4); } //NW
        if (lastAngleRotate >= 135 && lastAngleRotate < 180) { player->setCurrentDirection(5); } //W
        if (lastAngleRotate >= 90 && lastAngleRotate < 135) { player->setCurrentDirection(6); } //SW
        if (lastAngleRotate >= 45 && lastAngleRotate < 90) { player->setCurrentDirection(7); } //S
        if (lastAngleRotate >= 0 && lastAngleRotate < 45) { player->setCurrentDirection(8); } //SE

        player->SwitchState(player->selectState);

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

        auto removeSkillSprite = CallFunc::create([this]() {
            if (_skillSprite->getPhysicsBody()) {
                _skillSprite->removeComponent(_skillSprite->getPhysicsBody());
            }
            _skillSprite->removeFromParentAndCleanup(true);
            });
        performEffect = Sequence::create(spawn, removeSkillSprite, nullptr);

        _skillSprite->runAction(performEffect);
        performEffect->retain();
        currentSkillCoolDown = skillCooldown*player->getCDR();
        _iconSprite->setOpacity(70);
        coolDownCountLable->setVisible(true);
    }
}

void FireBall::update(float dt) {

    if (_skillSprite->getParent()) {
        // Lấy scene chính từ Director để kiểm tra có enemy nào trong phạm vi skill ko
        Scene* currentScene = Director::getInstance()->getRunningScene();
        if (currentScene) {
            Game* game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
            if (game) {
                auto monsters = game->listOfMonster;
                //get Player
                auto player = dynamic_cast<Player*>(this->getParent());

                for (auto& monster : monsters) {
                    if (monster && !monster->isDead && monster->getPosition().distance(_skillSprite->getPosition()) <= 60) {
                        _skillSprite->runAction(RemoveSelf::create());
                        monster->takeDamage((skillDamage + player->getEquipmentSkillDamage() + player->getAPDamage()));
                        if (monster->getCurrentHP() <= 0) {
                            monster->die();
                        }
                    }
                }

                auto boss = game->boss;
                if (boss && !boss->isDead && boss->getPosition().distance(_skillSprite->getPosition()) <= 60) {
                    _skillSprite->runAction(RemoveSelf::create());
                    boss->takeDamage((skillDamage + player->getEquipmentSkillDamage() + player->getAPDamage()));
                    
                    if (boss->getCurrentHP() <= 0) {
                        boss->die();
                    }
                }
            }
        }
    }
    else unschedule(CC_SCHEDULE_SELECTOR(FireBall::update));
}

void FireBall::updateCooldown(float dt) {
    if (currentSkillCoolDown >= 0) {
        currentSkillCoolDown -= dt;
        int coolDownToInt = std::floor(currentSkillCoolDown);
        if (coolDownToInt < 0 && coolDownCountLable->isVisible()) {
            _iconSprite->setOpacity(255);
            coolDownCountLable->setVisible(false);
            unschedule(CC_SCHEDULE_SELECTOR(FireBall::updateCooldown));
        }

        coolDownCountLable->setString(StringUtils::format("%d", coolDownToInt));
    }
}

void FireBall::updateEffect(float dt) {
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


    //            auto boss = game->boss;
    //            if (boss) {
    //                //Delete effect sprite when no longer effect
    //                if (effectTime < 0) {
    //                    if (boss->getChildByName("FireEffect"))boss->removeChildByName("FireEffect");
    //                }
    //                else {
    //                    if (boss->getChildByName("FireEffect")) {
    //                        boss->takeDamage(0.05 * (skillDamage + player->getEquipmentSkillDamage() + player->getAPDamage()));
    //                        if (boss->getCurrentHP() <= 0) {
    //                            if (boss->getChildByName("FireEffect"))boss->removeChildByName("FireEffect");
    //                        }
    //                    }
    //                }
    //            }


    //        }
    //    }
    //}
    //else unschedule(CC_SCHEDULE_SELECTOR(FireBall::updateEffect));

}
