#ifndef __UPGRADE_INVENTORY_BUTTON__
#define __UPGRADE_INVENTORY_BUTTON__

#include "cocos2d.h"
USING_NS_CC;
class UpgradeInventory;
class UpgradeInventoryButton : public cocos2d::Node
{
public:
    static UpgradeInventoryButton* create(UpgradeInventory* inventory);
    virtual bool init(UpgradeInventory* inventory);
    bool onTouchBegan(Touch* touch, Event* event);
    Size getSpriteSize() const { return btnSprite->getContentSize(); };
    UpgradeInventory* inven;
    Sprite* btnSprite;
private: 



};

#endif // __UPGRADE_INVENTORY_BUTTON__