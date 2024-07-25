#include "ShowQuestButton.h"
#include "../../Inventory/Inventory.h"
#include "Audio/AudioEngine.h"
#include "SettingsManager/SettingsData.h"
#include "TutorialManager/TutorialManager.h"
#include "Game/Game.h"
QuestBtn* QuestBtn::create() {
    auto node = new QuestBtn();
    if (node && node->init()) {
        node->autorelease();
        return node;
    }
    CC_SAFE_DELETE(node);
    return nullptr;
}

bool QuestBtn::init() {
    if (!Node::init()) {
        return false;
    }

    btnSprite = Sprite::create("res/action_bar_btn3.png");
    btnSprite->setScale(2);
    Sprite* iconSprite = Sprite::create("res/quest-icon.png");
    iconSprite->setScale(0.25 * Director::getInstance()->getContentScaleFactor());
    iconSprite->setPosition(Vec2(btnSprite->getContentSize().width / 2, btnSprite->getContentSize().height / 2));
    this->addChild(btnSprite);
    btnSprite->addChild(iconSprite);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(QuestBtn::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, btnSprite);
    return true;
}

bool QuestBtn::onTouchBegan(Touch* touch, Event* event) {
    if (isVisible() && btnSprite->isVisible()/*&& btnSprite->isVisible()*/ && UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0) >= 2) {
        if (btnSprite->getBoundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation()))) {
            UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
        }
        return false;
    }
    return false;
}