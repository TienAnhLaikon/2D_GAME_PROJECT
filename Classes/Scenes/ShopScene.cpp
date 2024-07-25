#include "ShopScene.h"
#include "Scenes/MainMenuScene.h"
#include "SettingsManager/SettingsManager.h"
#include "Scenes/ScenesManager.h"
#include "WorkingWithData/JSON/jsonManager.h"
#include "Game/Game.h"
#include "NotificationManager/NotificationManager.h"
Scene* ShopScene::createScene()
{
    return nullptr;
}

bool ShopScene::init()
{
    if (!Scene::init())
    {
        return false;
    }
    Audio::getInstance()->pause(Game::backgroundMusicId);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto transparentBackground = Sprite::create("res/black-background.png"); // Màu đen với độ trong suốt 128
    transparentBackground->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    //transparentBackground->setBlendFunc(BlendFunc::ALPHA_NON_PREMULTIPLIED); // Đặt chế độ hiển thị
    transparentBackground->setScale(visibleSize.width / transparentBackground->getContentSize().width, visibleSize.height / transparentBackground->getContentSize().height);

    this->addChild(transparentBackground,1);

   auto resumeButton = MenuItemImage::create(
       "res/button_orange.png",
       "res/button_orange-PUSH.png",
       CC_CALLBACK_1(ShopScene::unpauseCallback, this));
   resumeButton->setScale(0.2 * (visibleSize.width / visibleSize.height * 2));
   resumeButton->setPosition(Vec2(transparentBackground->getContentSize().width / 2, transparentBackground->getContentSize().height - 100));
   Label* playLabel = Label::createWithTTF("RESUME", "fonts/arial.ttf", 20);
   playLabel->setPosition(Vec2(resumeButton->getContentSize().width / 2, resumeButton->getContentSize().height / 2));
   resumeButton->addChild(playLabel, 2);

   this->addChild(resumeButton);

    return true;
}

void ShopScene::unpauseCallback(Ref* pSender)
{
    clickButtonSoundId = Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f);
    UserDefault::getInstance()->setIntegerForKey("sound_effect", clickButtonSoundId);
    Director::getInstance()->resume();
    Director::getInstance()->popScene();
    Audio::getInstance()->resume(Game::backgroundMusicId);
}


