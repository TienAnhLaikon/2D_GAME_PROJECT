#ifndef __DROPDOWN__
#define __DROPDOWN__

#include "cocos2d.h"
USING_NS_CC;
class InGameUI;
class InventoryButton;
class TalentButton;
class Dropdown : public cocos2d::Node
{
public:
    static Dropdown* create(InGameUI* inGameUI);
    virtual bool init(InGameUI* inGameUI);
    bool onTouchBegan(Touch* touch, Event* event);
    Sprite* bg = nullptr;
    Sprite* iconSprite;
    void updateIconSpritePosition();
    bool didOpen() { return isOpen == true && isScaling == false; }
    InGameUI* inGameUI = nullptr;
    InventoryButton* inventoryButton = nullptr;
    TalentButton* talentButton = nullptr;
    bool isOpen = false;
private:
    Sprite* toggleButton = nullptr;
    bool isScaling = false;
};

#endif // __JOYSTICK_H__