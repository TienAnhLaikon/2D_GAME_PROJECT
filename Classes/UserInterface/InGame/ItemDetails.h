#ifndef __ITEM_DETAILS__
#define __ITEM_DETAILS__

#include "cocos2d.h"
#include "Item/BaseItem.h"
USING_NS_CC;
class SubInventory;
class Inventory;
class UpgradeInventory;
class ItemDetails : public cocos2d::Node
{
public:
    ItemDetails(const std::string& iconPath, const std::string& itemName, const std::string& description, const BaseItem::Type itemType, const std::string stats, const bool isInUpgrade, const int itemQuantity, bool isInMaterialNode, int itemTypeId, bool isInInventory);
    ~ItemDetails();
    static ItemDetails* create(const std::string& iconPath, const std::string& itemName, const std::string& description, const BaseItem::Type itemType, const std::string stats, const bool isInUpgrade, const int itemQuantity, bool isInMaterialNode, int itemTypeId, bool isInInventory = false);
    virtual bool init();
    void destroy();
    void setSubInventory(SubInventory* inventory) { _subInventory = inventory; };
    void setInventory(Inventory* inventory) { _inventory = inventory; };
    void setUpgradeInventory(UpgradeInventory* inventory) { _upgradeInventory = inventory; };
    Sprite* background = nullptr;
    Sprite* useButton = nullptr;
    Sprite* addButton = nullptr;
    Sprite* dropButton = nullptr;
    void hide();
    void show();
private:
    SubInventory* _subInventory;
    Inventory* _inventory;
    UpgradeInventory* _upgradeInventory;
    int _itemQuantity;
    int _itemTypeId = -1;
    std::string _iconPath;
    std::string _itemName;
    std::string _description;
    std::string _stats;
    bool _isInUpgrade;
    bool _isInMaterialNode;
    bool _isInInventory = false;
    BaseItem::Type _itemType;

};

#endif // __JOYSTICK_H__