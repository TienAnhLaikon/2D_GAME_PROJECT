#include "PauseMenuScene.h"
#include "Scenes/MainMenuScene.h"
#include "SettingsManager/SettingsManagerInPauseMenu.h"
#include "Scenes/ScenesManager.h"
#include "WorkingWithData/JSON/jsonManager.h"
#include "Game/Game.h"
#include "NotificationManager/NotificationManager.h"
#include "Map/Map.h"
Scene* PauseMenuScene::createScene()
{
    return nullptr;
}

bool PauseMenuScene::init()
{
    if (!Scene::init())
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Audio::getInstance()->pause(UserDefault::getInstance()->getIntegerForKey("background_music"));
    Audio::getInstance()->pause(UserDefault::getInstance()->getIntegerForKey("sound_effect"));
    Audio::getInstance()->pause(UserDefault::getInstance()->getIntegerForKey("loop_effect1"));
    Audio::getInstance()->pause(UserDefault::getInstance()->getIntegerForKey("loop_effect2"));
    Audio::getInstance()->pause(UserDefault::getInstance()->getIntegerForKey("loop_effect3"));
    Audio::getInstance()->pause(UserDefault::getInstance()->getIntegerForKey("loop_effect4"));


    auto transparentBackground = Sprite::create("res/black-background.png"); 
    transparentBackground->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    //transparentBackground->setBlendFunc(BlendFunc::ALPHA_NON_PREMULTIPLIED); 
    transparentBackground->setScale(visibleSize.width / transparentBackground->getContentSize().width, visibleSize.height / transparentBackground->getContentSize().height);

    this->addChild(transparentBackground, 1);

    resumeButton = MenuItemImage::create(
        "res/button_orange.png",
        "res/button_orange-PUSH.png",
        CC_CALLBACK_1(PauseMenuScene::unpauseCallback, this));
    resumeButton->setScale(0.8 * Director::getInstance()->getContentScaleFactor());
    //resumeButton->setPosition(Vec2(transparentBackground->getContentSize().width / 2, transparentBackground->getContentSize().height - 100));
    Label* playLabel = Label::createWithTTF("RESUME", "fonts/Diablo Light.ttf", static_cast<int>(6 * Director::getInstance()->getContentScaleFactor()));
    playLabel->setPosition(Vec2(resumeButton->getContentSize().width / 2, resumeButton->getContentSize().height / 2));
    resumeButton->addChild(playLabel, 2);

    saveGameButton = MenuItemImage::create(
        "res/button_orange.png",
        "res/button_orange-PUSH.png",
        CC_CALLBACK_1(PauseMenuScene::saveGameCallback, this));
    saveGameButton->setScale(0.8 * Director::getInstance()->getContentScaleFactor());
    //saveGameButton->setPosition(Vec2(transparentBackground->getContentSize().width / 2, transparentBackground->getContentSize().height - 180));
    Label* saveGameLabel = Label::createWithTTF("SAVE GAME", "fonts/Diablo Light.ttf", static_cast<int>(6 * Director::getInstance()->getContentScaleFactor()));
    saveGameLabel->setPosition(Vec2(saveGameButton->getContentSize().width / 2, saveGameButton->getContentSize().height / 2));
    saveGameButton->addChild(saveGameLabel, 2);

    settingsButton = MenuItemImage::create(
        "res/button_orange.png",
        "res/button_orange-PUSH.png",
        CC_CALLBACK_1(PauseMenuScene::settingsCallback, this));
    settingsButton->setScale(0.8 * Director::getInstance()->getContentScaleFactor());
    //settingsButton->setPosition(Vec2(transparentBackground->getContentSize().width / 2, transparentBackground->getContentSize().height - 260));
    Label* settingsLabel = Label::createWithTTF("SETTINGS", "fonts/Diablo Light.ttf", static_cast<int>(6 * Director::getInstance()->getContentScaleFactor()));
    settingsLabel->setPosition(Vec2(settingsButton->getContentSize().width / 2, settingsButton->getContentSize().height / 2));
    settingsButton->addChild(settingsLabel, 2);

     backToMainMenuButton = MenuItemImage::create(
        "res/button_orange.png",
        "res/button_orange-PUSH.png",
        CC_CALLBACK_1(PauseMenuScene::backToMainMenuCallback, this));
    backToMainMenuButton->setScale(0.8 * Director::getInstance()->getContentScaleFactor());
    //backToMainMenuButton->setPosition(Vec2(transparentBackground->getContentSize().width / 2, transparentBackground->getContentSize().height - 340));
    Label* backToMainMenuLabel = Label::createWithTTF("BACK TO MAIN MENU", "fonts/Diablo Light.ttf", static_cast<int>(6 * Director::getInstance()->getContentScaleFactor()));
    backToMainMenuLabel->setPosition(Vec2(backToMainMenuButton->getContentSize().width / 2, backToMainMenuButton->getContentSize().height / 2));
    backToMainMenuButton->addChild(backToMainMenuLabel, 2);

    float buttonSpacing = 1; 
    float buttonHeight = resumeButton->getContentSize().height * (0.8 * Director::getInstance()->getContentScaleFactor());

    resumeButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2) + Vec2(0, buttonHeight * 1.2));
    saveGameButton->setPosition(Vec2(visibleSize.width / 2, resumeButton->getPositionY() - buttonHeight));
    settingsButton->setPosition(Vec2(visibleSize.width / 2, saveGameButton->getPositionY() - buttonHeight));
    backToMainMenuButton->setPosition(Vec2(visibleSize.width / 2, settingsButton->getPositionY() - buttonHeight));

    auto menu = Menu::create(resumeButton, saveGameButton, settingsButton, backToMainMenuButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    settingsManager = SettingsManagerInPauseMenu::create();
    settingsManager->setContentSize(this->getContentSize());
    //settingsManager->setPosition(Vec2(0,0));
    this->addChild(settingsManager, 500);

    return true;
}

void PauseMenuScene::unpauseCallback(Ref* pSender)
{

    UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));

    Scene* currentScene = Director::getInstance()->getRunningScene();
    if (currentScene)
        Game* game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
    if (game)
        game->getPlayer()->getCurrentSprite()->resume();

    //Director::getInstance()->resume(); 
    Director::getInstance()->popScene();
    if(SceneManager::getInstance()->game)
        SceneManager::getInstance()->game->resumeGame();
    Audio::getInstance()->resume(UserDefault::getInstance()->getIntegerForKey("background_music"));
    Audio::getInstance()->resume(UserDefault::getInstance()->getIntegerForKey("sound_effect"));
    Audio::getInstance()->resume(UserDefault::getInstance()->getIntegerForKey("loop_effect1"));
    Audio::getInstance()->resume(UserDefault::getInstance()->getIntegerForKey("loop_effect2"));
    Audio::getInstance()->resume(UserDefault::getInstance()->getIntegerForKey("loop_effect3"));
    Audio::getInstance()->resume(UserDefault::getInstance()->getIntegerForKey("loop_effect4"));



}

void PauseMenuScene::saveGameCallback(Ref* pSender)
{
    UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
    auto game = SceneManager::getInstance()->game;
    if (game) {
        game->save();
    }
    auto notificationLabel = Label::createWithTTF("SAVED!", "fonts/Diablo Light.ttf", 35);
    notificationLabel->setColor(Color3B::GREEN);
    notificationLabel->setScale(1 * Director::getInstance()->getContentScaleFactor());
    auto backgroundSprite = Sprite::create("res/black-background.png");
    backgroundSprite->setTextureRect(Rect(0, 0, notificationLabel->getContentSize().width, notificationLabel->getContentSize().height));
    backgroundSprite->addChild(notificationLabel, 1);
    notificationLabel->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2 + notificationLabel->getContentSize().width / 2, Director::getInstance()->getVisibleSize().height / 2 + notificationLabel->getContentSize().height / 2));

    backgroundSprite->setPosition(Vec2(0, 0));
    this->addChild(backgroundSprite, 100);

    //Action
    auto fadeOut = FadeOut::create(1.0f);
    notificationLabel->runAction(fadeOut);
    backgroundSprite->runAction(Sequence::create(fadeOut, RemoveSelf::create(), nullptr));
    
}

void PauseMenuScene::settingsCallback(Ref* pSender)
{
    UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
    if (!settingsManager->isMenuShow()) {
        resumeButton->setVisible(false);
        saveGameButton->setVisible(false);
        settingsButton->setVisible(false);
        backToMainMenuButton->setVisible(false);
        settingsManager->showMenuList();
    }
}

void PauseMenuScene::backToMainMenuCallback(Ref* pSender)
{
    clickButtonSoundId = Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f);
    UserDefault::getInstance()->setIntegerForKey("sound_effect", clickButtonSoundId);
    //Director::getInstance()->resume();
    auto game = SceneManager::getInstance()->game;
    if (game) {
        game->removeFromParentAndCleanup(true);
        game = nullptr;
    }
    SceneManager::getInstance()->runMainMenuScene();
}









































































