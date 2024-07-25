#include "DiedLayer.h"
#include "Game/Game.h"

DiedLayer* createDiedLayer() {
    auto node = new DiedLayer();
    if (node && node->init()) {
        node->autorelease();
        return node;
    }
    CC_SAFE_DELETE(node);
    return nullptr;
}

bool DiedLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();

    auto origin = Director::getInstance()->getVisibleOrigin();

    auto background = LayerColor::create(Color4B(0, 0, 0, 150));
    background->setContentSize(Size(visibleSize.width, visibleSize.height));
    background->setPosition(Vec2(0,0));
    this->addChild(background);


    this->setContentSize(Size(background->getContentSize().width * (visibleSize.width / background->getContentSize().width), background->getContentSize().height * (visibleSize.height / background->getContentSize().height)));

    auto logo = Sprite::create("res/skull_icon.png");
    logo->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + logo->getContentSize().height * 2));
    logo->setScale(1.25 * Director::getInstance()->getContentScaleFactor());
    this->addChild(logo);

    auto diedLabel = Label::createWithTTF("You Died!", "fonts/Diablo Light.ttf", 25);
    diedLabel->setPosition(logo->getPosition() - Vec2(0, logo->getContentSize().height * 1.3 * Director::getInstance()->getContentScaleFactor()));
    diedLabel->setTextColor(Color4B::RED);
    this->addChild(diedLabel);

    continueLabel = Label::createWithTTF("Tap to continue..", "fonts/Diablo Light.ttf", 18);
    continueLabel->setPosition(diedLabel->getPosition() + Vec2(0, -continueLabel->getContentSize().height * 2));
    continueLabel->setTextColor(Color4B::WHITE);
    this->addChild(continueLabel);

    

    return true;
}
void DiedLayer::tabToContinue() {
    continueLabel->setString("Waiting...");
}