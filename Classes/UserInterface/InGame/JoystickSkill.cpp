#include "JoystickSkill.h"

USING_NS_CC;

JoystickSkill::JoystickSkill() : isPressed(false), currentDirection(Vec2::ZERO)
{
}

bool JoystickSkill::init()
{
    if (!Node::init()) // Thay i ng dn hnh nh joystick ca bn
    {
        return false;
    }

    joystickBorder = Sprite::create("res/joystick-border.png");
    joystickBorder->setPosition(Vec2(0, 0));
    this->addChild(joystickBorder, 300);

    joystickBtn = Sprite::create("res/joystick-btn.png");
    joystickBtn->setPosition(Vec2(0, 0));
    this->addChild(joystickBtn, 500);
    centerPos = joystickBtn->getPosition();

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(JoystickSkill::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(JoystickSkill::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(JoystickSkill::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

bool JoystickSkill::onTouchBegan(Touch *touch, Event *event)
{
    Vec2 touchLocationInNode = this->convertToNodeSpace(touch->getLocation());

    if (joystickBtn->getBoundingBox().containsPoint(touchLocationInNode))
    {
        isPressed = true;
        return true;
    }
    return false;
}

void JoystickSkill::onTouchMoved(Touch *touch, Event *event)
{
    if (isPressed)
    {
        Vec2 touchLocationInNode = this->convertToNodeSpace(touch->getLocation());
        Vec2 direction = touchLocationInNode - centerPos;
        float distance = direction.length();

        float radius = joystickBorder->getContentSize().width / 2;

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

void JoystickSkill::onTouchEnded(Touch *touch, Event *event)
{
    if (isPressed) {
        prevPosBeforeRelease = joystickBtn->getPosition();
        joystickBtn->setPosition(centerPos);
        isPressed = false;
    }
}

Vec2 JoystickSkill::getDirection()
{
    if (isPressed) {
        auto result = joystickBtn->getPosition() - centerPos;
        result.normalize();
        return result;
    }
    else
    {
        return Vec2::ZERO;
    }
}
