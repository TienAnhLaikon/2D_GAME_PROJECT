#include "TalentButton.h"
#include "TalentCarousel.h"
#include "Audio/AudioEngine.h"
#include "SettingsManager/SettingsData.h"
#include "TutorialManager/TutorialManager.h"
#include "Game/Game.h"
TalentButton* TalentButton::create(TalentCarousel* talentCarousel) {
    auto node = new TalentButton();
    if (node && node->init(talentCarousel)) {
        node->autorelease();
        return node;
    }
    CC_SAFE_DELETE(node);
    return nullptr;
}

bool TalentButton::init(TalentCarousel* talentCarousel) {
    if (!Node::init()) {
        return false;
    }
    talent = talentCarousel;
    btnSprite = Sprite::create("res/action_bar_btn3.png");
    btnSprite->setScale(2);
    Sprite* iconSprite = Sprite::create("res/book_icon.png");
    iconSprite->setScale(0.25 * Director::getInstance()->getContentScaleFactor());
    iconSprite->setPosition(Vec2(btnSprite->getContentSize().width / 2, btnSprite->getContentSize().height / 2));
    this->addChild(btnSprite);
    btnSprite->addChild(iconSprite);
    btnSprite->retain();

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(TalentButton::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, btnSprite);


    return true;
}

bool TalentButton::onTouchBegan(Touch* touch, Event* event) {
    if (isVisible() && btnSprite->isVisible()&& this->getParent()->isVisible() && UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0) >= 4) {
        if (btnSprite->getBoundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation()))) {
            UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));

            Scene* currentScene = Director::getInstance()->getRunningScene();
            Game* game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
            game->pauseGame();

            int currentTutorial = UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0);
            if (currentTutorial == 4) {
                TutorialManager::getInstance()->nextTutorial();
            }

            if (talent->isVisible())
                talent->setVisible(false);
            else
                talent->showTalentCarousel();
            return true;
        }
        return false;
    }
    return false;
}
