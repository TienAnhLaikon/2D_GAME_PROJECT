#include "SelectMapButton.h"
#include "UserInterface/InGame/InGameUI.h"
#include "Scenes/SelectMapLayer.h"
#include "Game/Game.h"
SelectMapButton* SelectMapButton::create(SelectMapLayer* selectMapLayer) {
    auto node = new SelectMapButton();
    if (node && node->init(selectMapLayer)) {
        node->autorelease();
        return node;
    }
    CC_SAFE_DELETE(node);
    return nullptr;
}

bool SelectMapButton::init(SelectMapLayer* selectMapLayer) {
    if (!Node::init()) {
        return false;
    }
    _selectMapLayer = selectMapLayer;
    btnSprite = Sprite::create("res/action_bar_btn2.png");
    btnSprite->setScale(3);
    Sprite* iconSprite = Sprite::create("res/mp_icon_map.png");
    iconSprite->setScale(0.25 * Director::getInstance()->getContentScaleFactor());
    iconSprite->setPosition(Vec2(btnSprite->getContentSize().width / 2, btnSprite->getContentSize().height / 2));
    this->addChild(btnSprite);
    btnSprite->addChild(iconSprite);
    hideButton();

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(SelectMapButton::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, btnSprite);
    return true;
}

bool SelectMapButton::onTouchBegan(Touch* touch, Event* event) {
    if (isVisible()) {
        auto startPoint = this->convertToNodeSpace(touch->getLocation());
        if (_selectMapLayer && _selectMapLayer->isShow() == false && btnSprite->getBoundingBox().containsPoint(startPoint))
        {
            if (UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0) > 10) {
                Scene* currentScene = Director::getInstance()->getRunningScene();
                Game* game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
                _selectMapLayer->show();
                game->pauseGame();
            }
            else {
                NotificationManager::getInstance()->showMessageNotification("You need to complete tutorial before select map!", Vec2::ZERO, Color3B::RED, 16);
            }
            
            return true;
        }
        return false;
    }
    return false;
}

void SelectMapButton::showButton() {
    this->setVisible(true);
}

void SelectMapButton::hideButton() {
    this->setVisible(false);
}