#include "ThunderCurrent.h"
#include "Player/Player.h"
#include "Map/Map.h"
#include "HelloWorldScene.h"
#include <string>
bool ThunderCurrent::init() {
    skillTalent = new SkillTalent;
    skillTalent->skillName = "ThunderCurrent";
    skillTalent->iconPath = "skill/SkillSprite/SkillIcon/Thunder/thunder_current.png";
    skillTalent->description = "Summon a rain of thunder to fall within the area of effect, causing damage to all enemies hit by the ability. ";
    skillTalent->skillType = "AOE";
    skillTalent->unlockPoint = 50;
    skillTalent->upgradePoint = 50;
    skillTalent->parent = nullptr;
    skillTalent->isLock = true;
    skillTalent->levelRequired = 0;
    this->skillName = "ThunderCurrent";
    //AOE Range
    _aoeSprite = Sprite::create("skill/AOERangeSprite/RectangleRange.png");
    _aoeSprite->setScale(1);
    _aoeSprite->setOpacity(65);
    _aoeSprite->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(_aoeSprite);
    _aoeSprite->setVisible(false);

    //Skill Icon...
    _iconSprite = Sprite::create("skill/SkillSprite/SkillIcon/Thunder/thunder_current.png");
    _iconSprite->setScale(0.1);
    _iconSprite->retain();

    
    //Skill Sprite
    _skillSprite = Sprite::createWithSpriteFrameName("thunder_current (1).png");
    _skillSprite->setAnchorPoint(Vec2(0.5, 0.5));
    _skillSprite->retain();

    //Skill Animate
    _skillAnimate = Animate::create(Engine::createAnimation2("thunder_current", 60, 0.02));
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
    damageRaitoOfPlayer = 1.2f;
    skillDamage = 450;

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
    touchListener->onTouchBegan = CC_CALLBACK_2(ThunderCurrent::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(ThunderCurrent::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(ThunderCurrent::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, _skillButton);
    _touchListener = touchListener;

    return true;
}

bool ThunderCurrent::onTouchBegan(Touch* touch, Event* event)
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

void ThunderCurrent::onTouchMoved(Touch* touch, Event* event)
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

void ThunderCurrent::onTouchEnded(Touch* touch, Event* event)
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

void ThunderCurrent::performSkill(Vec2 target) {
    
    _skillButton->cancelButton->setVisible(false);
        _aoeSprite->setVisible(false);
        if (currentSkillCoolDown <= 0){

            auto player = dynamic_cast<Player*>(this->getParent());
            if (player->getCurrentMP() < manaCost) {
                player->noManaAlert();
                return;
            }
            UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/lightning_fire.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
            schedule(CC_SCHEDULE_SELECTOR(ThunderCurrent::update), 1.0f);
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

        _skillSprite->runAction(Sequence::create(_skillAnimate, RemoveSelf::create(), nullptr));

        // Ly scene chnh t Director  kim tra c enemy no trong phm vi skill ko
        Scene* currentScene = Director::getInstance()->getRunningScene();
        if (currentScene) {
            Game* game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
            if (game) {
                //get Player
                auto player = dynamic_cast<Player*>(this->getParent());

                // Quai thuong
                auto monsters = game->listOfMonster;
                for (auto& monster : monsters) {
                    if (monster && !monster->isDead && applyPosition.distance(monster->getPosition()) <= 100) {
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

                        //        schedule(CC_SCHEDULE_SELECTOR(ThunderCurrent::updateEffect), 1.0f);
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
                if (boss && !boss->isDead && applyPosition.distance(boss->getPosition()) <= 100) {
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

                    //        schedule(CC_SCHEDULE_SELECTOR(ThunderCurrent::updateEffect), 1.0f);
                    //        effectTime = 5.0f;
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

void ThunderCurrent::update(float dt) {
    if (currentSkillCoolDown >= 0) {
        currentSkillCoolDown -= dt;
        int coolDownToInt = std::floor(currentSkillCoolDown);
        if (coolDownToInt < 0 && coolDownCountLable->isVisible()) {
            _iconSprite->setOpacity(255);
            coolDownCountLable->setVisible(false);
            unschedule(CC_SCHEDULE_SELECTOR(ThunderCurrent::update));
        }

        coolDownCountLable->setString(StringUtils::format("%d", coolDownToInt));
    }
}

void ThunderCurrent::updateEffect(float dt) {
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
    //else unschedule(CC_SCHEDULE_SELECTOR(ThunderCurrent::updateEffect));

}