#include "FireCastB.h"
#include "Player/Player.h"
#include "Enemies/Enemy.h"
#include "HelloWorldScene.h"
#include "Map/Map.h"
#include "Game/Game.h"
#include <string>
bool FireCastB::init() {

    skillTalent = new SkillTalent;
    skillTalent->skillName = "FireCastB";
    skillTalent->iconPath = "skill/SkillSprite/SkillIcon/Fire/fire_cast_b.png";
    skillTalent->unlockPoint = 20;
    skillTalent->upgradePoint = 20;
    skillTalent->description = "Activate a fire buff, increase damage, HP, Armor";
    skillTalent->skillType = "Buff";
    skillTalent->parent = nullptr;
    skillTalent->children.push_back(SkillFactory::createSkill("FireScorch")->getSkillTalent());
    skillTalent->isLock = true;
    skillTalent->levelRequired = 3;
    this->skillName = "FireCastB";
    //Skill Icon...
    _iconSprite = Sprite::create(skillTalent->iconPath);
    _iconSprite->setScale(0.1);
    _iconSprite->retain();

    
    //Skill Sprite
    _skillSprite = Sprite::createWithSpriteFrameName("fire_cast_b (1).png");
    _skillSprite->setAnchorPoint(Vec2(0.5, 0.35));
    _skillSprite->setScale(0.5);
    _skillSprite->retain();

    //Skill Animate
    _skillAnimate = Animate::create(Engine::createAnimation2("fire_cast_b", 45, 0.05));
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
    this->hpExtra = 100;
    this->mpExtra = 100;
    this->dmgExtra = 15;
    this->armorExtra = 10;
    //- Attack Skill
    this->damageRaitoOfPlayer = 0;
    this->skillDamage = 0;

    // + Bonus Stats when level up
    this->skillCooldown_reduct = 2;
    //- Buff Skill
    this->hpExtra_bonus = 100;
    this->mpExtra_bonus = 100;
    this->dmgExtra_bonus = 15;
    this->armorExtra_bonus = 10;
    //- Attack Skill
    this->skillDamage_bonus = 0;

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

    skillEffectiveIndicate->setPosition(Vec2(-50, -50));
    skillEffectiveIndicate->setVisible(false);
    this->addChild(skillEffectiveIndicate, 10);

    // ng k s kin onTouch cho SkillButton
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(FireCastB::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(FireCastB::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(FireCastB::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, _skillButton);
    _touchListener = touchListener;

    return true;
}

bool FireCastB::onTouchBegan(Touch* touch, Event* event)
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

void FireCastB::onTouchMoved(Touch* touch, Event* event)
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

void FireCastB::onTouchEnded(Touch* touch, Event* event)
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

void FireCastB::performSkill(Vec2 target) {
    
    if (currentSkillCoolDown <= 0) {

        auto player = dynamic_cast<Player*>(this->getParent());
        if (player->getCurrentMP() < manaCost) {
            player->noManaAlert();
            return;
        }
        flamesSkillOnId = Audio::getInstance()->play2d("sound/sounds effect/flames_skill_on.mp3", true, SettingsData::getInstance()->getSoundSlider() / 100.0f);
        UserDefault::getInstance()->setIntegerForKey("loop_effect4", flamesSkillOnId);

        // Dng m thanh sau 15 giy
        cocos2d::Director::getInstance()->getScheduler()->schedule([this](float dt) {
            cocos2d::AudioEngine::stop(this->flamesSkillOnId);
            }, this, 0, 0, 15.0f, false, "stop_flames_skill_sound");

        schedule(CC_SCHEDULE_SELECTOR(FireCastB::update), 1.0f);
        schedule(CC_SCHEDULE_SELECTOR(FireCastB::updateCooldown), 1.0f);

        player->SwitchState(player->selectState);
        player->setCurrentMP(player->getCurrentMP() - manaCost);
        isActive = true;
        if (player) {
            player->setHP(player->getHP() + player->getEquipmentHP() + this->hpExtra);
            player->setCurrentHP(player->getCurrentHP() + this->hpExtra);
            player->setMP(player->getMP() + player->getEquipmentMP() + this->mpExtra);
            player->setCurrentMP(player->getCurrentMP() + this->mpExtra);
            player->setDamage(player->getDamage() + player->getEquipmentDamage() + this->dmgExtra);
            player->setArmor(player->getArmor() + player->getEquipmentArmor() + this->armorExtra);
        }

        this->addChild(_skillSprite, -2);

        auto sqe = Sequence::create(Repeat::create(_skillAnimate, 7), RemoveSelf::create(), nullptr);
        _skillSprite->runAction(sqe);

        currentSkillCoolDown = skillCooldown*player->getCDR();
        _iconSprite->setOpacity(70);
        coolDownCountLable->setVisible(true);
    }
}

void FireCastB::update(float dt) {
    if (isActive) {
        if (!skillEffectiveIndicate->isVisible()) {
            skillEffectiveIndicate->setVisible(true);
        }
        int timeEffectToInt = std::floor(activeTime);
        timeEffectLabel->setString(std::to_string(timeEffectToInt));
        activeTime -= dt;
        if (activeTime < 0) {
            auto player = dynamic_cast<Player*>(this->getParent());
            if (player) {
                player->setHP(player->getHP() - this->hpExtra);
                player->setCurrentHP(player->getCurrentHP() - this->hpExtra);
                player->setMP(player->getMP() - this->mpExtra);
                player->setCurrentMP(player->getCurrentMP() - this->mpExtra);
                player->setDamage(player->getDamage() - this->dmgExtra);
                player->setArmor(player->getArmor() - this->armorExtra);
                isActive = false;
                activeTime = 15;
                skillEffectiveIndicate->setVisible(false);
            }
        }
    }
    else {
        unschedule(CC_SCHEDULE_SELECTOR(FireCastB::update));
    }
    
}

void FireCastB::updateCooldown(float dt) {
    if (currentSkillCoolDown >= 0) {
        currentSkillCoolDown -= dt;
        int coolDownToInt = std::floor(currentSkillCoolDown);
        if (coolDownToInt < 0 && coolDownCountLable->isVisible()) {
            _iconSprite->setOpacity(255);
            coolDownCountLable->setVisible(false);
            unschedule(CC_SCHEDULE_SELECTOR(FireCastB::updateCooldown));
        }

        coolDownCountLable->setString(StringUtils::format("%d", coolDownToInt));
    }
}