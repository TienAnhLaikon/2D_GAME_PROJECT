#include "QuestButton.h"
#include "../../Quest/Quest.h"
#include "Audio/AudioEngine.h"
#include "TutorialManager/TutorialManager.h"
#include "SettingsManager/SettingsData.h"

QuestButton *QuestButton::create()
{
    auto node = new QuestButton();
    if (node && node->init())
    {
        node->autorelease();
        return node;
    }
    CC_SAFE_DELETE(node);
    return nullptr;
}

bool QuestButton::init()
{
    if (!Node::init())
    {
        return false;
    }
    btnSprite = Sprite::create("res/im_container.png");
    btnSprite->setPosition(this->getPosition() );
    btnSprite->setScale(0.6 * Director::getInstance()->getContentScaleFactor());
    btnSprite->setScaleX(1.2 * Director::getInstance()->getContentScaleFactor());
    btnSprite->setScaleY(0.7 * Director::getInstance()->getContentScaleFactor());
    this->addChild(btnSprite);
    btnSprite->retain();

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(QuestButton::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, btnSprite);

    return true;
}

bool QuestButton::onTouchBegan(Touch *touch, Event *event)
{
    if (isVisible() && btnSprite->isVisible() && this->getParent()->isVisible()) {
        if (btnSprite->getBoundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation())) && UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0) >= 11)
        {
            UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));

            //Scene* currentScene = Director::getInstance()->getRunningScene();
            //Game* game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
            //game->pauseGame();

            int currentTutorial = UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0);
            if (currentTutorial == 11) {
                TutorialManager::getInstance()->nextTutorial();
            }
            if (isShow()) {
                QuestManager::getInstance()->generalDisplay();

                CCLOG("SHOWED");
            }
            else {
                QuestManager::getInstance()->detailDisplay();

                CCLOG("NOT SHOWED");
            }
            return true;
        }
        return false;
    }
    return false;
}
