#ifndef __JOYSTICK_SKILL__
#define __JOYSTICK_SKILL__

#include "cocos2d.h"
USING_NS_CC;
class SkillBase;
class JoystickSkill : public cocos2d::Node
{
public:
    JoystickSkill();
    virtual bool init();
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    Vec2 getDirection();
    bool getIsPress() const { return isPressed; };
    Size getSize() const { return joystickBorder->getContentSize(); };
    Sprite* joystickBorder;
    CREATE_FUNC(JoystickSkill);
private:
    bool isPressed;
    Sprite* joystickBtn;
    Vec2 prevPosBeforeRelease = Vec2::ZERO;
    Vec2 centerPos;
    Vec2 originalPosition;
    Vec2 currentDirection;
};

#endif // __JOYSTICK_SKILL__