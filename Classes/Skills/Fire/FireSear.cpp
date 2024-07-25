#include "FireSear.h"
#include "Player/Player.h"
#include "Enemies/Enemy.h"
#include "HelloWorldScene.h"
#include "Map/Map.h"
#include "TutorialManager/TutorialManager.h"
#include <string>
bool FireSear::init() {

    skillTalent = new SkillTalent;
    skillTalent->skillName = "FireSear";
    skillTalent->iconPath = "res/attack_button.png";
    skillTalent->unlockPoint = 0;
    skillTalent->upgradePoint = 0;
    skillTalent->description = "";
    skillTalent->skillType = "AOE & Target";
    skillTalent->isLock = true;
    skillTalent->levelRequired = 0;
    this->skillName = "FireSear";
    skillTalent->parent = nullptr;
    //Skill Icon...
    _iconSprite = Sprite::create("res/attack_button.png");
    _iconSprite->setOpacity(70);
    _iconSprite->setScale(0.27 * Director::getInstance()->getContentScaleFactor());
    _iconSprite->retain();

    //SkillButton
    _skillButton = SkillButton::create();
    
    _skillButton->setSkillButtonBorder(_iconSprite);
    _skillButton->skillButtonBtn->setScale(1*Director::getInstance()->getContentScaleFactor());
    _skillButton->skillButtonBtn->setOpacity(0);
    _skillButton->retain();

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("skill/SkillSprite/PlayerCharacterSkill/NormalAttackEffect/normal_attack_effect.plist", "skill/SkillSprite/PlayerCharacterSkill/NormalAttackEffect/normal_attack_effect.png");
    //Skill Sprite
    _skillSprite = Sprite::createWithSpriteFrameName("normal_attack_effect (1).png");
    _skillSprite->setScale(1);
    _skillSprite->setOpacity(100);
    _skillSprite->setAnchorPoint(Vec2(0.5, 0.25));
    _skillSprite->retain();

    //Skill Animate
    _skillAnimate = Animate::create(Engine::createAnimation2("normal_attack_effect", 25, 0.02));
    _skillAnimate->retain();
    //SkillTree...

    // ng k s kin onTouch cho SkillButton
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(FireSear::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(FireSear::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(FireSear::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, _skillButton);
    _touchListener = touchListener;
    schedule(CC_SCHEDULE_SELECTOR(FireSear::update), 0.05f);

    return true;
}

bool FireSear::onTouchBegan(Touch* touch, Event* event)
{

    Vec2 touchLocationInNode = _skillButton->convertToNodeSpace(touch->getLocation());

    if (_skillButton->skillButtonBtn->getBoundingBox().containsPoint(touchLocationInNode))
    {
        _skillButton->isPressed = true;
        _iconSprite->setOpacity(255);
        _iconSprite->setScale(0.3 * Director::getInstance()->getContentScaleFactor());

        performSkill(Vec2::ZERO);
        return true;
    }
    return false;
}

void FireSear::onTouchMoved(Touch* touch, Event* event)
{
    if (_skillButton->isPressed)
    {
        CCLOG("Moving");
        Vec2 touchLocationInNode = _skillButton->convertToNodeSpace(touch->getLocation());
        Vec2 direction = touchLocationInNode - _skillButton->centerPos;
        float distance = direction.length();
    }
}

void FireSear::onTouchEnded(Touch* touch, Event* event)
{
    if (_skillButton->isPressed) {
        _skillButton->prevPosBeforeRelease = _skillButton->skillButtonBtn->getPosition();
        _skillButton->skillButtonBtn->setPosition(_skillButton->centerPos);
        _skillButton->isPressed = false;

        _iconSprite->setOpacity(70);
        _iconSprite->setScale(0.27*Director::getInstance()->getContentScaleFactor());
    }

}

void FireSear::performSkill(Vec2 target) {
    auto player = dynamic_cast<Player*>(this->getParent());
    if (player->currentState != player->deadState && player->currentState != player->attackState && player->currentState != player->selectState) {
        player->SwitchState(player->attackState);
        if (_skillSprite->getParent())_skillSprite->removeFromParentAndCleanup(true);
        player->addChild(_skillSprite);
        if (_skillSprite->getNumberOfRunningActions() > 0)_skillSprite->stopAllActions();
        _skillSprite->runAction(Sequence::create(_skillAnimate, RemoveSelf::create(), nullptr));

        Scene* currentScene = Director::getInstance()->getRunningScene();

        if (currentScene) {
            Game* game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
            if (game) {
                auto children = game->listOfMonster;

                for (auto& monster : children) {
                        if (player->getPosition().distance(monster->getPosition()) <= player->getAttackRange()) {
                            if (monster && !monster->isDead) {
                                monster->takeDamage(player->getDamage() + player->getEquipmentSkillDamage());
                                if (monster->getCurrentHP() <= 0) {
                                    monster->die();
                                }
                            }
                        }
                    
                }
                //Boss
                auto boss = game->boss;
                if (boss && !boss->isDead && player->getPosition().distance(boss->getPosition()) <= player->getAttackRange()) {
                    boss->takeDamage(player->getDamage() + player->getEquipmentSkillDamage());
                    if (boss->getCurrentHP() <= 0) {
                        boss->die();
                    }
                }
            }
        }

    }
    // tutorial
    int currentTutorial = UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0);
    if (currentTutorial == 1) {
        TutorialManager::getInstance()->nextTutorial();
    }
}

//void FireSear::deleteEnemies(float dt) {
//    if (!monstersRemove.empty()) {
//        for (auto monster : monstersRemove) {
//            if (monster->getCurrentSprite()->getOpacity() == 0) {
//                monster->removeFromParentAndCleanup(true);
//            }
//
//        }
//    }
//    else {
//        if (isDeleteEnemiesScheduled) {
//            this->unschedule(CC_SCHEDULE_SELECTOR(FireSear::deleteEnemies));
//            isDeleteEnemiesScheduled = false;
//        }
//    }
//}