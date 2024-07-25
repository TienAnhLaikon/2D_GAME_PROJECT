#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include "cocos2d.h"
USING_NS_CC;
class Joystick : public cocos2d::Node
{
public:
    Joystick();

    // Khi to joystick
    virtual bool init();

    // Phng thc x l s kin chm
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

    // Tr v hng ca joystick
    Vec2 getDirection();
    Vec2 previousDirection();
    bool getIsPress() const { return isPressed; };
    Size getSize() const { return joystickBorder->getContentSize(); };
    void setCanPress(bool isTrue) { isCanPress = isTrue; };
    bool getIsCanPress() { return isCanPress; };
    bool isCanPress = true;
    CREATE_FUNC(Joystick);

private:

    bool isPressed;
    Sprite* joystickBorder;
    Sprite* joystickBtn;
    Vec2 prevPosBeforeRelease = Vec2::ZERO;
    Vec2 centerPos;
    Vec2 originalPosition;
    Vec2 currentDirection;
    Vec2 prevDirection = Vec2::ZERO;
};

#endif // __JOYSTICK_H__