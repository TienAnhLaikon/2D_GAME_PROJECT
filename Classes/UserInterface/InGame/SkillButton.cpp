#include "SkillButton.h"
#include "Audio/AudioEngine.h"
#include "SettingsManager/SettingsData.h"
USING_NS_CC;

SkillButton::SkillButton() : isPressed(false), currentDirection(Vec2::ZERO)
{
}

bool SkillButton::init()
{
    if (!Node::init()) // Thay i ng dn hnh nh SkillButton ca bn
    {
        return false;
    }

    skillButtonBorder = Sprite::create("res/joystick-border.png");
    skillButtonBorder->setPosition(Vec2(0, 0));
    skillButtonBorder->setOpacity(0);
    this->addChild(skillButtonBorder, 300);

    skillButtonBtn = Sprite::create("res/joystick-btn.png");
    skillButtonBtn->setPosition(Vec2(0, 0));
    skillButtonBtn->setOpacity(0);
    skillButtonBtn->setContentSize(Size(skillButtonBorder->getContentSize().width *2 /3, skillButtonBorder->getContentSize().height * 2 /3));
    this->addChild(skillButtonBtn, 500);
    centerPos = skillButtonBtn->getPosition();

    cancelButton = Sprite::create("res/cancelBorder.png");
    cancelButton->setPosition(Vec2(0, 65));
    cancelButton->setOpacity(60);
    this->addChild(cancelButton, 500);
    cancelButton->setVisible(false);

    //auto touchListener = EventListenerTouchOneByOne::create();
    //touchListener->setSwallowTouches(true);
    //touchListener->onTouchBegan = CC_CALLBACK_2(SkillButton::onTouchBegan, this);
    //touchListener->onTouchMoved = CC_CALLBACK_2(SkillButton::onTouchMoved, this);
    //touchListener->onTouchEnded = CC_CALLBACK_2(SkillButton::onTouchEnded, this);
    //_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

//bool SkillButton::onTouchBegan(Touch *touch, Event *event)
//{
//    Vec2 touchLocationInNode = this->convertToNodeSpace(touch->getLocation());
//
//    if (skillButtonBtn->getBoundingBox().containsPoint(touchLocationInNode))
//    {
//        isPressed = true;
//        return true;
//    }
//    return false;
//}
//
//void SkillButton::onTouchMoved(Touch *touch, Event *event)
//{
//    if (isPressed)
//    {
//        Vec2 touchLocationInNode = this->convertToNodeSpace(touch->getLocation());
//        Vec2 direction = touchLocationInNode - centerPos;
//        float distance = direction.length();
//
//        float radius = 40;
//
//        if (distance > radius)
//        {
//            direction.normalize();
//            direction *= radius;
//            skillButtonBtn->setPosition(centerPos + direction);
//            currentPos = centerPos + direction;
//        }
//        else
//        {
//            skillButtonBtn->setPosition(touchLocationInNode);
//            currentPos = touchLocationInNode;
//        }
//
//    }
//}
//
//void SkillButton::onTouchEnded(Touch *touch, Event *event)
//{
//    prevPosBeforeRelease = skillButtonBtn->getPosition();
//    skillButtonBtn->setPosition(centerPos);
//    isPressed = false;
//}

Vec2 SkillButton::getDirection()
{
    if (isPressed)
        return skillButtonBtn->getPosition() - centerPos;
    else
    {
        return Vec2::ZERO;
    }
}
