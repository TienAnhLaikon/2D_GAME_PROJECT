#include "Dropdown.h"
#include "UserInterface/InGame/InGameUI.h"
#include "UserInterface/InGame/InventoryButton.h"
#include "Scenes/SelectMapLayer.h"
#include "Game/Game.h"

Dropdown* Dropdown::create(InGameUI* inGameUI) {
    auto node = new Dropdown();
    if (node && node->init(inGameUI)) {
        node->autorelease();
        return node;
    }
    CC_SAFE_DELETE(node);
    return nullptr;
}

bool Dropdown::init(InGameUI* inGameUI) {
    if (!Node::init()) {
        return false;
    }
    this->inGameUI = inGameUI;
    bg = Sprite::create("res/dropdown_btn.png");
    bg->setOpacity(180);
    bg->setScale(4, 3);
    iconSprite = Sprite::create("res/left-arr-ui2.png");
    iconSprite->setScale(1.9);
    this->addChild(bg);
    this->addChild(iconSprite);
    isOpen = false;
    isScaling = false;  // Khởi tạo biến cờ
    bg->setAnchorPoint(Vec2(1.0, 0));
    updateIconSpritePosition();
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(Dropdown::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    return true;
}

bool Dropdown::onTouchBegan(Touch* touch, Event* event) {
    if (isVisible() && this->getParent()->isVisible()) {
        auto startPoint = this->convertToNodeSpace(touch->getLocation());
        if (iconSprite->getBoundingBox().containsPoint(startPoint)) {
            iconSprite->setVisible(false);
            iconSprite->setRotation(isOpen ? 180 : 0);
            if (isScaling) {
                return false; 
            }
            ScaleTo* scaleAction;
            if (isOpen) {
                scaleAction = ScaleTo::create(0.3, bg->getScaleX() / 2.6, bg->getScaleY());
                inGameUI->inventoryButton->setVisible(false);
                inGameUI->talentButton->setVisible(false);
            }
            else {
                scaleAction = ScaleTo::create(0.3, bg->getScaleX() * 2.6, bg->getScaleY());
                this->runAction(Sequence::create(
                    DelayTime::create(0.2), 
                    CallFunc::create([this]() {
                    inGameUI->inventoryButton->setVisible(true);
                    }), 
                    DelayTime::create(0.208), 
                    CallFunc::create([this]() {
                    inGameUI->talentButton->setVisible(true);
                    }), nullptr));
            }

            auto sequence = Sequence::create(
                scaleAction,
                CallFunc::create([this]() {
                    updateIconSpritePosition();
                    isScaling = false;  
                    iconSprite->setVisible(true);
                    }),
                nullptr
            );

            isScaling = true;  
            bg->runAction(sequence);

            isOpen = !isOpen;
        }
    }
    return false;
}

void Dropdown::updateIconSpritePosition() {
    float bgWidth = bg->getContentSize().width * bg->getScaleX();
    iconSprite->setPosition(Vec2(bg->getPositionX() - bgWidth + (iconSprite->getContentSize().width * iconSprite->getScale()) - 20, bg->getPositionY() + bg->getContentSize().height * bg->getScaleY() / 2));
}