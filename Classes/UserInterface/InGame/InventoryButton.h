#ifndef __INVENTORY_BUTTON__
#define __INVENTORY_BUTTON__

#include "cocos2d.h"
USING_NS_CC;
class Inventory;
class InventoryButton : public cocos2d::Node
{
public:
    static InventoryButton* create(Inventory* inventory);
    virtual bool init(Inventory* inventory);
    bool onTouchBegan(Touch* touch, Event* event);
    Size getSpriteSize() const { return btnSprite->getContentSize(); };
    Inventory* inven;
    Sprite* btnSprite;
private: 


};

#endif // __JOYSTICK_H__