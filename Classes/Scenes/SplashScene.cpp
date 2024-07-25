#include "SplashScene.h"
#include "./MainMenuScene.h"
#include "./Scenes/ScenesManager.h"
#include "./UserInterface/InGame/EquipmentDetails.h"
Scene* SplashScene::createScene()
{
    auto scene = Scene::create();
    auto layer = SplashScene::create();
    scene->addChild(layer);
    return scene;
}

bool SplashScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    UserDefault::getInstance()->setBoolForKey("SkeletonInCache", false); // SkeletonInCache
    UserDefault::getInstance()->setBoolForKey("EffectInCache", false);
    UserDefault::getInstance()->setBoolForKey("FighterInCache", false); // FighterInCache
    UserDefault::getInstance()->setBoolForKey("KnightInCache", false);
    UserDefault::getInstance()->setBoolForKey("WarriorInCache", false);
    UserDefault::getInstance()->setBoolForKey("BabySpiderInCache", false);
    UserDefault::getInstance()->setBoolForKey("HellBeastInCache", false);
    UserDefault::getInstance()->setBoolForKey("IsLoadedGame", false);



    auto background = Sprite::create("res/black-background.png");
    background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    background->setScale(visibleSize.width / background->getContentSize().width, visibleSize.height / background->getContentSize().height);
    this->addChild(background);

    auto logo = Sprite::create("res/cocos2d-logo.png");
    logo->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    logo->setScale(0.6);
    this->addChild(logo);

    background->setOpacity(0);
    auto fadeIn = FadeIn::create(1.0f);
    auto delay = DelayTime::create(1.0f);
    auto sequence = Sequence::create(fadeIn, delay, nullptr);
    background->runAction(sequence);
    auto moveToMainMenu = CallFunc::create([]() {
        SceneManager::getInstance()->runMainMenuScene();
        });
    auto delayAction = DelayTime::create(3.0f); 
    auto sequence2 = Sequence::create(delayAction, moveToMainMenu, nullptr);
    this->runAction(sequence2);

    return true;
}
