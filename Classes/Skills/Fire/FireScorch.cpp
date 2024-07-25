#include "FireScorch.h"
#include "HelloWorldScene.h"
#include "Enemies/NormalMonster/NormalMonster.h"
#include "Enemies/Boss/Boss.h"
#include "Game/Game.h"
#include "Map/Map.h"

bool FireScorch::init() {
    skillTalent = new SkillTalent;
    skillTalent->skillName = "FireScorch";
    skillTalent->iconPath = "skill/SkillSprite/SkillIcon/Fire/fire_scorch.png";
    skillTalent->unlockPoint = 30;
    skillTalent->upgradePoint = 30;
    skillTalent->description = "Summon multiple fire pillar to every enemy in area of effect.";
    skillTalent->skillType = "AOE & Target";
    skillTalent->isLock = true;
    skillTalent->parent = nullptr;
    skillTalent->levelRequired = 0;
    this->skillName = "FireScorch";
    applyRange = 95;
    //AOE Range
    _aoeSprite = Sprite::create("skill/AOERangeSprite/CircleRange.png");
    _aoeSprite->setOpacity(100);
    _aoeSprite->setScale(2);
    this->addChild(_aoeSprite);
    _aoeSprite->setVisible(false);


    //Skill Icon...
    _iconSprite = Sprite::create("skill/SkillSprite/SkillIcon/Fire/fire_scorch.png");
    _iconSprite->setScale(0.1);
    _iconSprite->retain();


    //Skill Sprite
    _skillSprite = Sprite::createWithSpriteFrameName("fire_scorch (1).png");
    _skillSprite->setScale(0.1);
    _skillSprite->setAnchorPoint(Vec2(0.5, 0.25));
    _skillSprite->retain();

    //Skill Animate
    _skillAnimate = Animate::create(Engine::createAnimation2("fire_scorch", 72, 0.1));
    _skillAnimate->retain();

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
    skillDamage = 400;

    // + Bonus Stats when level up
    skillCooldown_reduct = 1.5;
    //- Buff Skill
    hpExtra_bonus = 0;
    mpExtra_bonus = 0;
    dmgExtra_bonus = 0;
    armorExtra_bonus = 0;
    //- Attack Skill
    skillDamage_bonus = 60;

    coolDownCountLable = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 24);
    _skillButton->addChild(coolDownCountLable);
    coolDownCountLable->setVisible(false);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(FireScorch::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(FireScorch::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(FireScorch::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, _skillButton);
    _touchListener = touchListener;

    return true;
}

bool FireScorch::onTouchBegan(Touch* touch, Event* event)
{

    Vec2 touchLocationInNode = _skillButton->convertToNodeSpace(touch->getLocation());
    auto player = dynamic_cast<Player*>(this->getParent());

    if (_skillButton->skillButtonBtn->getBoundingBox().containsPoint(touchLocationInNode) && player->currentState != player->selectState)
    {
        auto player = dynamic_cast<Player*>(this->getParent());
        if (player->getCurrentMP() < manaCost) {
            player->noManaAlert();
            return false;
        }

        if (currentSkillCoolDown <= 0) {
            schedule(CC_SCHEDULE_SELECTOR(FireScorch::updateEnemies), 0.00);
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

void FireScorch::onTouchMoved(Touch* touch, Event* event)
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

        //Detect all NormalMonster and Boss be added to scene
          // Ly scene chnh t Director
        Scene* currentScene = Director::getInstance()->getRunningScene();
        if (currentScene) {
            Game* game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
            if (game) {
                auto children = game->gameMap->getTiledMap()->getChildren();
                //Danh sch nhng enemy s xa khi NormalMonster
                std::vector<Enemy*> enemiesRemove;
                for (const auto& child : children) {
                    auto enemy = dynamic_cast<Enemy*>(child); //Tm nhng enemy c trong scene
                    if (enemy && enemy->getCurrentHP() > 0) {
                        Vec2 enemyInWorldCoor = enemy->getParent()->convertToWorldSpace(enemy->getPosition());
                        Vec2 enemyInSkillCoor = this->convertToNodeSpace(enemyInWorldCoor);

                        float distance = enemyInSkillCoor.distance(this->getPosition());
                        //Nu enemy trong tm thi trin V cha c thm Target th thm vo list enemy
                        if (distance <= applyRange) {
                            auto sprite = Sprite::create("skill/AOERangeSprite/TargetNew.png");
                            sprite->setName("Target");
                            bool isAdded = false;
                            for (auto enemyCheck : enemies) {
                                if (enemy == enemyCheck)isAdded = true;
                            }
                            if (!isAdded) {
                                enemy->addChild(sprite);
                                sprite->setVisible(true);
                                enemies.push_back(enemy);
                            }
                        }
                        else enemiesRemove.push_back(enemy);

                    }
                }
                //Thc hin n target cho nhng enemy  i ra khi tm thi trin
                //ng thi xa lun khi danh sch NormalMonster
                if (enemies.size() > 0) {
                    for (auto enemyRemove : enemiesRemove) {
                        if (enemyRemove->getChildByName("Target")) {
                            enemyRemove->removeChildByName("Target");
                            auto it = std::find(enemies.begin(), enemies.end(), enemyRemove);
                            if (it != enemies.end()) {
                                enemies.erase(it);
                            }
                        }
                    }
                }
            }
        }
    }
    CCLOG("%d", enemies.size());
}

void FireScorch::onTouchEnded(Touch* touch, Event* event)
{
    if (_skillButton->isPressed) {
        _skillButton->prevPosBeforeRelease = _skillButton->skillButtonBtn->getPosition();
        _skillButton->skillButtonBtn->setPosition(_skillButton->centerPos);
        _skillButton->isPressed = false;

        Vec2 lastTouchLocationInNode = _skillButton->convertToNodeSpace(touch->getLocation());
        if (_skillButton->cancelButton->getBoundingBox().containsPoint(lastTouchLocationInNode)) {
            this->_aoeSprite->setVisible(false);
            _skillButton->cancelButton->setVisible(false);

            std::vector<Enemy*> enemiesRemove;
            for (auto enemy : enemies) {
                enemiesRemove.push_back(enemy);
            }

            for (auto enemyRemove : enemiesRemove) {
                enemyRemove->removeChildByName("Target");
                auto it = std::find(enemies.begin(), enemies.end(), enemyRemove);
                if (it != enemies.end()) {
                    enemies.erase(it);
                }
            }


            return;

        }
        else {
            Vec2 pos = this->convertToWorldSpace(this->_aoeSprite->getPosition());
            performSkill(pos);
            this->_aoeSprite->setVisible(false);
            _skillButton->cancelButton->setVisible(false);
        }
        unschedule(CC_SCHEDULE_SELECTOR(FireScorch::updateEnemies));
    }
}

void FireScorch::performSkill(Vec2 target) {

    if (enemies.size() == 0)return;
    auto player = dynamic_cast<Player*>(this->getParent());
    if (player->getCurrentMP() < manaCost) {
        player->noManaAlert();
        return;
    }
    if (currentSkillCoolDown <= 0 && enemies.size() > 0) {
        player->SwitchState(player->selectState);
        player->setCurrentMP(player->getCurrentMP() - manaCost);
        schedule(CC_SCHEDULE_SELECTOR(FireScorch::update), 1.0f);
        UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/flames_fire.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
    }
    std::vector<Enemy*> NormalMonsterRemove;
    for (auto enemy : enemies) {
        CCLOG("LULE");
        auto sprite = enemy->getChildByName("Target");
        auto skillSprite = Sprite::createWithSpriteFrameName("fire_scorch (1).png");
        skillSprite->setAnchorPoint(Vec2(0.5, 0.2));
        skillSprite->setScale(applyRange / skillSprite->getContentSize().width, applyRange / skillSprite->getContentSize().height);
        skillSprite->setPosition(sprite->getPosition());
        enemy->removeChild(sprite);
        enemy->addChild(skillSprite);

        //DealDamage
        if (currentSkillCoolDown <= 0) {
            enemy->takeDamage((skillDamage + player->getDamage() * damageRaitoOfPlayer) / enemies.size());
            ////Do effect
            //if (player->getEquipment("Weapon")->getElement() == player->getEquipment("Weapon")->FIRE) {
            //    if (!enemy->getChildByName("FireEffect")) {
            //        //Skill Effect Sprite
            //        auto effect = Sprite::createWithSpriteFrameName("fire_sear (1).png");
            //        effect->setName("FireEffect");
            //        effect->setScale(0.2);

            //        //Skill Effect Animate
            //        auto animate = Animate::create(Engine::createAnimation2("fire_sear", 30, 0.05));
            //        enemy->addChild(effect);

            //        //Effect to Monster
            //        effect->setPosition(Vec2(0, 40));
            //        effect->runAction(RepeatForever::create(animate));

            //        schedule(CC_SCHEDULE_SELECTOR(FireScorch::updateEffect), 1.0f);
            //        effectTime = 5.0f;
            //    }
            //}
        }
        //Skill Animate
        Animate* skillAnimate = Animate::create(Engine::createAnimation2("fire_scorch", 72, 0.02));

        auto sqe = Sequence::create(skillAnimate, RemoveSelf::create(), nullptr);

        if (currentSkillCoolDown <= 0)skillSprite->runAction(sqe);
        NormalMonsterRemove.push_back(enemy);

        auto boss = dynamic_cast<Boss*>(enemy);
        if (boss && boss->currentState != boss->deadState) {
            if (boss->getCurrentHP() == 0) {
                boss->die();
            }
        }

        auto monster = dynamic_cast<NormalMonster*>(enemy);
        if (monster && monster->currentState != monster->deadState) {
            if (monster->getCurrentHP() == 0) {
                monster->die();
            }
        }
    }
    for (auto enemyRemove : NormalMonsterRemove) {
        //Remove from NormalMonster
        auto it = std::find(enemies.begin(), enemies.end(), enemyRemove);
        // If found, erase it
        if (it != enemies.end()) {
            enemies.erase(it);
        }
    }

    if (currentSkillCoolDown <= 0)
        currentSkillCoolDown = skillCooldown;

    _iconSprite->setOpacity(70);

    coolDownCountLable->setVisible(true);

}

void FireScorch::update(float dt) {
    if (currentSkillCoolDown >= 0) {
        currentSkillCoolDown -= dt;
        int coolDownToInt = std::floor(currentSkillCoolDown);
        if (coolDownToInt < 0 && coolDownCountLable->isVisible()) {
            _iconSprite->setOpacity(255);
            coolDownCountLable->setVisible(false);
            unschedule(CC_SCHEDULE_SELECTOR(FireScorch::update));
        }

        coolDownCountLable->setString(StringUtils::format("%d", coolDownToInt));
    }
}

void FireScorch::updateEnemies(float dt) {
    //Thc hin xa cc enemy khi enemies khi  ht mu
    std::vector<Enemy*> enemiesRemove;
    for (auto enemy : enemies) {
        if (enemy->getCurrentHP() <= 0)enemiesRemove.push_back(enemy);
    }
    for (auto enemyRemove : enemiesRemove) {
        auto it = std::find(enemies.begin(), enemies.end(), enemyRemove);
        if (it != enemies.end()) {
            enemies.erase(it);
        }
    }
}

void FireScorch::updateEffect(float dt) {
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
    //else unschedule(CC_SCHEDULE_SELECTOR(FireScorch::updateEffect));

}