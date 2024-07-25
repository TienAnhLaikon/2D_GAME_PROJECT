#include "HamburgerMenuButton.h"
#include "../../Scenes/PauseMenuScene.h"
#include "Scenes/ScenesManager.h"
#include "Audio/AudioEngine.h"
#include "SettingsManager/SettingsData.h"
#include "Game/Game.h"
HamburgerMenuButton *HamburgerMenuButton::create(HamburgerMenu* hamburgerMenu)
{
    auto node = new HamburgerMenuButton();
    if (node && node->init(hamburgerMenu))
    {
        node->autorelease();
        return node;
    }
    CC_SAFE_DELETE(node);
    return nullptr;
}

bool HamburgerMenuButton::init(HamburgerMenu* hamburgerMenu)
{
    if (!Node::init())
    {
        return false;
    }
    this->hamburgerMenu = hamburgerMenu;
    btnSprite = Sprite::create("res/pause-button.png");
    btnSprite->setScale(0.65);
    this->addChild(btnSprite);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(HamburgerMenuButton::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, btnSprite);

    return true;
}

bool HamburgerMenuButton::onTouchBegan(Touch *touch, Event *event)
{
    if (isVisible() && btnSprite->isVisible() && this->getParent()->isVisible()) {
        if (btnSprite->getBoundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation())))
        {
            UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
            SceneManager::getInstance()->runPauseMenuScene();
            return true;
        }
        return false;
    }
    return false;
}
