#include "Joystick.h"
#include "TutorialManager/TutorialManager.h"
USING_NS_CC;

Joystick::Joystick() : isPressed(false), currentDirection(Vec2::ZERO)
{
}

bool Joystick::init()
{
    if (!Node::init()) // Thay i ng dn hnh nh joystick ca bn
    {
        return false;
    }

    joystickBorder = Sprite::create("res/joystick-border.png");
    joystickBorder->setPosition(Vec2(0, 0));
    joystickBorder->setOpacity(0);
    this->addChild(joystickBorder, 300);

    joystickBtn = Sprite::create("res/joystick-btn.png");
    joystickBtn->setPosition(Vec2(0, 0));
    joystickBtn->setOpacity(0);
    this->addChild(joystickBtn, 500);
    centerPos = joystickBtn->getPosition();

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(Joystick::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(Joystick::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(Joystick::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

bool Joystick::onTouchBegan(Touch *touch, Event *event)
{
    Vec2 touchLocationInNode = this->convertToNodeSpace(touch->getLocation());

    if (joystickBtn->getBoundingBox().containsPoint(touchLocationInNode) && isCanPress)
    {
        isPressed = true;
        joystickBtn->setOpacity(0);
        int currentTutorial = UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0);
        if (currentTutorial == 0) {
            TutorialManager::getInstance()->nextTutorial();
        }
        return true;
    }
    return false;
}

void Joystick::onTouchMoved(Touch *touch, Event *event)
{
    if (isPressed)
    {
        Vec2 touchLocationInNode = this->convertToNodeSpace(touch->getLocation());
        Vec2 direction = touchLocationInNode - centerPos;
        float distance = direction.length();

        float radius = joystickBorder->getContentSize().width / 1.5;

        if (distance > radius)
        {
            direction.normalize();
            direction *= radius;
            joystickBtn->setPosition(centerPos + direction);
        }
        else
        {
            joystickBtn->setPosition(touchLocationInNode);
        }
    }
}

void Joystick::onTouchEnded(Touch *touch, Event *event)
{
    if (isPressed) {
        prevPosBeforeRelease = joystickBtn->getPosition();
        joystickBtn->setPosition(centerPos);
        isPressed = false;
        joystickBtn->setOpacity(0);
    }
}

Vec2 Joystick::getDirection()
{
    if (isPressed) {
        auto result = joystickBtn->getPosition() - centerPos;
        result.normalize();
        prevDirection = result;
        prevPosBeforeRelease = result;
        return result;
    }
    else
    {
        return prevPosBeforeRelease;
    }
}

Vec2 Joystick::previousDirection() {
    return prevPosBeforeRelease;
}