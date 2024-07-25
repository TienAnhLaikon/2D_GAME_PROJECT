#include "FireConflagration.h"
#include "Player/Player.h"
#include "Skills/SkillFactory.h"
#include "HelloWorldScene.h"
#include "Map/Map.h"
#include "Game/Game.h"
#include <string>
bool FireConflagration::init() {

    skillTalent = new SkillTalent;
    skillTalent->skillName = "FireConflagration";
    skillTalent->iconPath = "skill/SkillSprite/SkillIcon/Fire/fire_conflagration.png";
    skillTalent->unlockPoint = 30;
    skillTalent->upgradePoint = 30;
    skillTalent->description = "Shout out an area of napalm and then detonate it, causing damage to enemies within its blast radius.";
    skillTalent->skillType = "AOE";
    skillTalent->parent = nullptr;
    skillTalent->isLock = true;
    skillTalent->children.push_back(SkillFactory::createSkill("FireFlare")->getSkillTalent());
    skillTalent->levelRequired = 0;
    this->skillName = "FireConflagration";
    //AOE Range
    _aoeSprite = Sprite::create("skill/AOERangeSprite/CircleRange.png");
    _aoeSprite->setOpacity(100);
    this->addChild(_aoeSprite);
    _aoeSprite->setVisible(false);

    //Skill Icon...
    _iconSprite = Sprite::create(skillTalent->iconPath);
    _iconSprite->setScale(0.1);
    _iconSprite->retain();

    
    //Skill Sprite
    _skillSprite = Sprite::createWithSpriteFrameName("fire_conflagration (1).png");
    _skillSprite->setAnchorPoint(Vec2(0.5, 0.5));
    _skillSprite->retain();

    //Skill Animate
    _skillAnimate = Animate::create(Engine::createAnimation2("fire_conflagration", 90, 0.01));
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
    skillDamage = 350;

    // + Bonus Stats when level up
    skillCooldown_reduct = 1.5;
    //- Buff Skill
    hpExtra_bonus = 0;
    mpExtra_bonus = 0;
    dmgExtra_bonus = 0;
    armorExtra_bonus = 0;
    //- Attack Skill
    skillDamage_bonus = 35;

    coolDownCountLable = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 24);
    _skillButton->addChild(coolDownCountLable);
    coolDownCountLable->setVisible(false);

    // ng k s kin onTouch cho SkillButton
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(FireConflagration::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(FireConflagration::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(FireConflagration::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, _skillButton);
    _touchListener = touchListener;

    return true;
}

bool FireConflagration::onTouchBegan(Touch* touch, Event* event)
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

void FireConflagration::onTouchMoved(Touch* touch, Event* event)
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

        float rate = _skillButton->getCurrentPos().distance(_skillButton->getCenterPos()) / 40;

        auto dir = _skillButton->getDirection();
        dir.normalize();

        Vec2 out = this->getPosition() + dir * rate * 100;
        this->_aoeSprite->setPosition(out);

        // Calculate the angle between the vector and the x-axis
        float angleRadians = atan2(direction.y, direction.x);
        float angleDegrees = CC_RADIANS_TO_DEGREES(-angleRadians); // Convert radians to degrees
        lastAngleRotate = angleDegrees;
    }
}

void FireConflagration::onTouchEnded(Touch* touch, Event* event)
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
        performSkill(pos);
        this->_aoeSprite->setVisible(false);
        _skillButton->cancelButton->setVisible(false);
    }
}

void FireConflagration::performSkill(Vec2 target) {
    //if () {
    if (currentSkillCoolDown <= 0) {

        auto player = dynamic_cast<Player*>(this->getParent());
        if (player->getCurrentMP() < manaCost) {
            player->noManaAlert();
            return;
        }
        UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/flames_fire.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));

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
        schedule(CC_SCHEDULE_SELECTOR(FireConflagration::updateCooldown), 1.0f);

        Vec2 applyPosition = this->getParent()->getParent()->convertToNodeSpace(target);
        _skillSprite->setPosition(applyPosition);
        auto scene = this->getParent()->getScene();
        scene->addChild(_skillSprite);

        auto sqe = Sequence::create(_skillAnimate, RemoveSelf::create(), nullptr);
        _skillSprite->runAction(sqe);


        Scene* currentScene = Director::getInstance()->getRunningScene();
        if (currentScene) {
            Game* game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
            if (game) {
                //get Player
                auto player = dynamic_cast<Player*>(this->getParent());

                // Quai thuong
                auto monsters = game->listOfMonster;
                for (auto& monster : monsters) {
                    if (monster && !monster->isDead && applyPosition.distance(monster->getPosition()) <= 60) {
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
                        //        schedule(CC_SCHEDULE_SELECTOR(FireConflagration::updateEffect), 1.0f);
                        //    }
                        //}
                        if (monster->getCurrentHP() <= 0) {
                            monster->die();
                        }
                    }
                }

                //Boss
                auto boss = game->boss;
                if (boss && !boss->isDead && applyPosition.distance(boss->getPosition()) <= 60) {
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
                    //        schedule(CC_SCHEDULE_SELECTOR(FireConflagration::updateEffect), 1.0f);
                    //    }
                    //}
                    if (boss->getCurrentHP() <= 0) {
                        boss->die();
                    }
                }
            }
        }
        currentSkillCoolDown = skillCooldown;

        _iconSprite->setOpacity(70);

        coolDownCountLable->setVisible(true);
    }
}

void FireConflagration::updateCooldown(float dt) {
    if (currentSkillCoolDown >= 0) {
        currentSkillCoolDown -= dt;
        int coolDownToInt = std::floor(currentSkillCoolDown);
        if (coolDownToInt < 0 && coolDownCountLable->isVisible()) {
            _iconSprite->setOpacity(255);
            coolDownCountLable->setVisible(false);
            unschedule(CC_SCHEDULE_SELECTOR(FireConflagration::updateCooldown));
        }

        coolDownCountLable->setString(StringUtils::format("%d", coolDownToInt));
    }
}

void FireConflagration::updateEffect(float dt) {
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
    //else unschedule(CC_SCHEDULE_SELECTOR(FireConflagration::updateEffect));

}