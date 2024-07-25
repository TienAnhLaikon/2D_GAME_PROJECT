#include "ThunderCastB.h"
#include "Player/Player.h"
#include "HelloWorldScene.h"
#include "Map/Map.h"
#include "Game/Game.h"
#include <string>
bool ThunderCastB::init() {
    skillTalent = new SkillTalent;
    skillTalent->skillName = "ThunderCastB";
    skillTalent->iconPath = "skill/SkillSprite/SkillIcon/Thunder/thunder_cast_b.png";
    skillTalent->description = "Active a thunder zone around oneself, increase damage, HP, MP and Armor. ";
    skillTalent->skillType = "Buff";
    skillTalent->unlockPoint = 20;
    skillTalent->upgradePoint = 20;
    skillTalent->parent = nullptr;
    skillTalent->children.push_back(SkillFactory::createSkill("ThunderBolt")->getSkillTalent());
    skillTalent->isLock = true;
    skillTalent->levelRequired = 0;
    this->skillName = "ThunderCastB";
    //Skill Icon...
    _iconSprite = Sprite::create("skill/SkillSprite/SkillIcon/Thunder/thunder_cast_b.png");
    _iconSprite->setScale(0.1);
    _iconSprite->retain();

    
    //Skill Sprite
    _skillSprite = Sprite::createWithSpriteFrameName("thunder_cast_b (1).png");
    _skillSprite->setAnchorPoint(Vec2(0.5, 0.35));
    _skillSprite->setScale(0.5);
    _skillSprite->retain();

    //Skill Animate
    _skillAnimate = Animate::create(Engine::createAnimation2("thunder_cast_b", 45, 0.05));
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
    hpExtra = 100;
    mpExtra = 100;
    dmgExtra = 15;
    armorExtra = 10;
    //- Attack Skill
    damageRaitoOfPlayer = 0.5f;
    skillDamage = 0;

    // + Bonus Stats when level up
    skillCooldown_reduct = 1.5;
    //- Buff Skill
    hpExtra_bonus = 100;
    mpExtra_bonus = 100;
    dmgExtra_bonus = 15;
    armorExtra_bonus = 10;
    //- Attack Skill
    skillDamage_bonus = 0;

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

   
    skillEffectiveIndicate->setPosition(Vec2(50, -50));
    skillEffectiveIndicate->setVisible(false);
    this->addChild(skillEffectiveIndicate, 10);

    // ng k s kin onTouch cho SkillButton
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(ThunderCastB::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(ThunderCastB::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(ThunderCastB::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, _skillButton);
    _touchListener = touchListener;


    return true;
}

bool ThunderCastB::onTouchBegan(Touch* touch, Event* event)
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

void ThunderCastB::onTouchMoved(Touch* touch, Event* event)
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

void ThunderCastB::onTouchEnded(Touch* touch, Event* event)
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

void ThunderCastB::performSkill(Vec2 target) {
    
    if (currentSkillCoolDown <= 0) {

        auto player = dynamic_cast<Player*>(this->getParent());
        if (player->getCurrentMP() < manaCost) {
            player->noManaAlert();
            return;
        }
        lightningSkillOnId = Audio::getInstance()->play2d("sound/sounds effect/lighting_skill_on.mp3", true, SettingsData::getInstance()->getSoundSlider() / 100.0f);
        UserDefault::getInstance()->setIntegerForKey("loop_effect2", lightningSkillOnId);
        // Dng m thanh sau 15 giy
        cocos2d::Director::getInstance()->getScheduler()->schedule([this](float dt) {
            cocos2d::AudioEngine::stop(this->lightningSkillOnId);
            }, this, 0, 0, 15.0f, false, "stop_lightning_skill_sound");
        schedule(CC_SCHEDULE_SELECTOR(ThunderCastB::update), 1.0f);
        schedule(CC_SCHEDULE_SELECTOR(ThunderCastB::updateCooldown), 1.0f);
        player->SwitchState(player->selectState);
        player->setCurrentMP(player->getCurrentMP() - manaCost);

        isActive = true;
        if (player) {
            player->setHP(player->getHP() + this->hpExtra);
            player->setCurrentHP(player->getCurrentHP() + this->hpExtra);
            player->setMP(player->getMP() + this->mpExtra);
            player->setCurrentMP(player->getCurrentMP() + this->mpExtra);
            player->setDamage(player->getDamage() + this->dmgExtra);
            player->setArmor(player->getArmor() + this->armorExtra);
        }

        this->addChild(_skillSprite, -1);

        auto sqe = Sequence::create(Repeat::create(_skillAnimate, 7), RemoveSelf::create(), nullptr);
        _skillSprite->runAction(sqe);

        currentSkillCoolDown = skillCooldown;

        _iconSprite->setOpacity(70);

        coolDownCountLable->setVisible(true);
    }
}
void ThunderCastB::update(float dt) {
    if (isActive) {
        if (!skillEffectiveIndicate->isVisible()) {
            skillEffectiveIndicate->setVisible(false);
        }
        int timeEffectToInt = std::floor(activeTime);
        timeEffectLabel->setString(std::to_string(timeEffectToInt));
        activeTime -= dt;
        CCLOG("Active Time = %f", activeTime);
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
    else  unschedule(CC_SCHEDULE_SELECTOR(ThunderCastB::update));

}

void ThunderCastB::updateCooldown(float dt) {
    if (currentSkillCoolDown >= 0) {
        currentSkillCoolDown -= dt;
        int coolDownToInt = std::floor(currentSkillCoolDown);
        if (coolDownToInt < 0 && coolDownCountLable->isVisible()) {
            _iconSprite->setOpacity(255);
            coolDownCountLable->setVisible(false);
            unschedule(CC_SCHEDULE_SELECTOR(ThunderCastB::updateCooldown));
        }

        coolDownCountLable->setString(StringUtils::format("%d", coolDownToInt));
    }
}