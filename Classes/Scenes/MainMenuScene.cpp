#include "MainMenuScene.h"
#include "Game/Game.h"
#include "Scenes/ScenesManager.h"
#include "Scenes/TutorialScene.h"
#include "Scenes/AnimaticScene.h"
#include "SettingsManager/SettingsManager.h"
#include "SettingsManager/SettingsData.h"
#include "Audio/AudioEngine.h"
#include "CoinManager/CoinManager.h"
#include "TutorialManager/TutorialManager.h"

Scene* MainMenuScene::createScene()
{
    auto scene = Scene::create();
    auto layer = Game::create();
    scene->addChild(layer);
    return scene;
}

bool MainMenuScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    if (mainMenuMusicId == -1 || !Audio::getInstance()->isPlaying(mainMenuMusicId)) {
        UserDefault::getInstance()->setIntegerForKey("background_music", Audio::getInstance()->play2d("sound/musics/main_menu_music1.mp3", true, SettingsData::getInstance()->getMusicSlider() / 100.0f));
        CCLOG("CHua phat nhac");
    }
    else {
        CCLOG("Dang phat nhac khong can phat ");
    }
    auto background = Sprite::create("res/background-img.png");
    background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    background->setScale(visibleSize.width / background->getContentSize().width, visibleSize.height / background->getContentSize().height);
    this->addChild(background);

    auto logo = Sprite::create("res/soul-slayer-icon.png");
    logo->setPosition(Vec2(visibleSize.width / 2 - 100, visibleSize.height /2.3));
    logo->setScale(1.25 * Director::getInstance()->getContentScaleFactor());
    this->addChild(logo);

    background->setOpacity(0);
    auto fadeIn = FadeIn::create(1.0f);
    auto delay = DelayTime::create(1.0f);
    auto sequence = Sequence::create(fadeIn, delay, nullptr);
    background->runAction(sequence);

    auto moveToMainMenu = CallFunc::create([this, visibleSize, origin]() {
         playButton = MenuItemImage::create(
            "res/button_orange.png",
            "res/button_orange-PUSH.png",
            CC_CALLBACK_1(MainMenuScene::menuPlayCallback, this));
        playButton->setScale(0.5 * Director::getInstance()->getContentScaleFactor());

         tutorialButton = MenuItemImage::create(
            "res/button_orange.png",
            "res/button_orange-PUSH.png",
            CC_CALLBACK_1(MainMenuScene::tutorialCallback, this));
        tutorialButton->setScale(0.5 * Director::getInstance()->getContentScaleFactor());

         settingsButton = MenuItemImage::create(
            "res/button_orange.png",
            "res/button_orange-PUSH.png",
            CC_CALLBACK_1(MainMenuScene::settingsCallback, this));
        settingsButton->setScale(0.5 * Director::getInstance()->getContentScaleFactor());

         exitButton = MenuItemImage::create(
            "res/button_orange.png",
            "res/button_orange-PUSH.png",
            CC_CALLBACK_1(MainMenuScene::menuCloseCallback, this));
        exitButton->setScale(0.5 * Director::getInstance()->getContentScaleFactor());

        float buttonSpacing = 1; 
        float buttonHeight = playButton->getContentSize().height * playButton->getScale();
        float totalHeight = 4 * buttonHeight + 3 * buttonSpacing;
        float startY = (visibleSize.height + totalHeight) / 2 - 75;

        playButton->setPosition(Vec2(visibleSize.width / 2+ 400, startY - buttonHeight / 2));
        tutorialButton->setPosition(Vec2(visibleSize.width / 2+ 400, startY - buttonHeight * 1.5 - buttonSpacing));
        settingsButton->setPosition(Vec2(visibleSize.width / 2+ 400, startY - buttonHeight * 2.5 - 2 * buttonSpacing));
        exitButton->setPosition(Vec2(visibleSize.width / 2+ 400, startY - buttonHeight * 3.5 - 3 * buttonSpacing));

         playLabel = Label::createWithTTF("PLAY", "fonts/Diablo Light.ttf", 15);
        playLabel->setPosition(playButton->getPosition());
        this->addChild(playLabel, 2);

         tutorialLabel = Label::createWithTTF("TUTORIAL", "fonts/Diablo Light.ttf", 15);
        tutorialLabel->setPosition(tutorialButton->getPosition());
        this->addChild(tutorialLabel, 2);

         settingsLabel = Label::createWithTTF("SETTINGS", "fonts/Diablo Light.ttf", 15);
        settingsLabel->setPosition(settingsButton->getPosition());
        this->addChild(settingsLabel, 2);

         exitLabel = Label::createWithTTF("EXIT", "fonts/Diablo Light.ttf", 15);
        exitLabel->setPosition(exitButton->getPosition());
        this->addChild(exitLabel, 2);

        auto menu = Menu::create(playButton, tutorialButton, settingsButton, exitButton, nullptr);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, 1);
        });

    auto delayAction = DelayTime::create(2.0f);
    auto sequence2 = Sequence::create(delayAction, moveToMainMenu, nullptr);
    settingsManager = SettingsManager::create();
    this->addChild(settingsManager, 500);

    this->runAction(sequence2);

    //////Coin Manager
    CoinManager::getInstance();

    return true;

}

void MainMenuScene::menuPlayCallback(Ref * pSender)
{
    /*auto scene = Game::createGame();
    Director::getInstance()->replaceScene(scene);*/
    UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
    Audio::getInstance()->stop(mainMenuMusicId);

    Game* game = dynamic_cast<Game*>(Director::getInstance()->getRunningScene()->getChildByName("GameInstance"));;
    if (game) game->removeAllChildrenWithCleanup(true);
    SceneManager::getInstance()->runGameScene();
}

void MainMenuScene::menuCloseCallback(Ref * pSender)
{
    UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
    CoinManager::getInstance()->destroyInstance();
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void MainMenuScene::tutorialCallback(Ref* pSender)
{
    UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
    CCLOG("TUTORIAL");
    SceneManager::getInstance()->runTutorialScene();
}

void MainMenuScene::settingsCallback(Ref* pSender)
{
    UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
    settingsManager->showMenuList();
    playButton->setVisible(false);
    tutorialButton->setVisible(false);
    settingsButton->setVisible(false);
    exitButton->setVisible(false);
    playLabel->setVisible(false);
    tutorialLabel->setVisible(false);
    settingsLabel->setVisible(false);
    exitLabel->setVisible(false);

}
