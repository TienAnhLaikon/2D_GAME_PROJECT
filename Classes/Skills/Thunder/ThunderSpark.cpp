#include "ThunderSpark.h"
#include "Player/Player.h"
#include "Skills/SkillFactory.h"
#include "HelloWorldScene.h"
#include "Map/Map.h"
#include <string>
bool ThunderSpark::init() {
    skillTalent = new SkillTalent;
    skillTalent->skillName = "ThunderSpark";
    skillTalent->iconPath = "skill/SkillSprite/SkillIcon/Thunder/thunder_spark.png";
    skillTalent->description = "Perform an area of Thunder Explode, dealing damage to all enemy within the area of effect.";
    skillTalent->skillType = "AOE & Target";
    skillTalent->unlockPoint = 10;
    skillTalent->isLock = true;
    skillTalent->parent = nullptr;
    skillTalent->levelRequired = 0;
    this->skillName = "ThunderSpark";
    //AOE Range
    _aoeSprite = Sprite::create("skill/AOERangeSprite/CircleRange.png");
    _aoeSprite->setOpacity(100);
    this->addChild(_aoeSprite);
    _aoeSprite->setVisible(false);

    //Skill Icon...
    _iconSprite = Sprite::create("skill/SkillSprite/SkillIcon/Thunder/thunder_spark.png");
    _iconSprite->setScale(0.1);
    _iconSprite->retain();


    //Skill Sprite
    _skillSprite = Sprite::createWithSpriteFrameName("thunder_spark (1).png");
    _skillSprite->setAnchorPoint(Vec2(0.5, 0.5));
    _skillSprite->retain();

    //Skill Animate
    _skillAnimate = Animate::create(Engine::createAnimation2("thunder_spark", 30, 0.030));
    _skillAnimate->retain();

    //SkillButton
    _skillButton = SkillButton::create();
    _skillButton->setSkillButtonBorder(_iconSprite);
    _skillButton->retain();

    //SkillTree...

    // ng k s kin onTouch cho SkillButton
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(ThunderSpark::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(ThunderSpark::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(ThunderSpark::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, _skillButton);
    _touchListener = touchListener;
    schedule(CC_SCHEDULE_SELECTOR(ThunderSpark::update), 0.05f);

    return true;
}

bool ThunderSpark::onTouchBegan(Touch* touch, Event* event)
{

    Vec2 touchLocationInNode = _skillButton->convertToNodeSpace(touch->getLocation());

    if (_skillButton->skillButtonBtn->getBoundingBox().containsPoint(touchLocationInNode))
    {
        _skillButton->isPressed = true;
        this->_aoeSprite->setVisible(true);
        _skillButton->cancelButton->setVisible(true);

        return true;
    }
    return false;
}

void ThunderSpark::onTouchMoved(Touch* touch, Event* event)
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

    }
}

void ThunderSpark::onTouchEnded(Touch* touch, Event* event)
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
    performSkill(pos);
    this->_aoeSprite->setVisible(false);
    _skillButton->cancelButton->setVisible(false);
}

void ThunderSpark::performSkill(Vec2 target) {
    UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/lightning_fire.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
    Vec2 applyPosition = this->getParent()->getParent()->convertToNodeSpace(target);
    _skillSprite->setPosition(applyPosition);
    auto scene = this->getParent()->getScene();
    scene->addChild(_skillSprite);

    auto sqe = Sequence::create(_skillAnimate, RemoveSelf::create(), nullptr);
    _skillSprite->runAction(sqe);

    // Ly scene chnh t Director  kim tra c enemy no trong phm vi skill ko
    Scene* currentScene = Director::getInstance()->getRunningScene();
    if (currentScene) {
        Game* game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
        if (game) {
            auto children = game->gameMap->getTiledMap()->getChildren();
            //get Player
            auto player = dynamic_cast<Player*>(this->getParent());

            for (const auto& child : children) {
                auto enemy = dynamic_cast<Enemy*>(child);
                if (enemy) {
                    //Kim tra xem enemy c nm trong vng nh hng ca skill hay khng
                    if (applyPosition.distance(enemy->getPosition()) <= 60) {
                        enemy->takeDamage(skillDamage);
                        if (enemy->getCurrentHP() == 0) {
                            player->gainExp(enemy->getExpGain());

                        }
                    }
                }
            }
        }
    }
}

void ThunderSpark::update(float dt) {

}