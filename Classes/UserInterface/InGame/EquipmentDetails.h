#ifndef __EQUIPMENT_DETAILS__
#define __EQUIPMENT_DETAILS__

#include "cocos2d.h"
USING_NS_CC;
class Inventory;
class UpgradeInventory;
class Element;
class EquipmentDetails : public cocos2d::Node {
public:
    EquipmentDetails(const std::string imagePath, const std::string equipmentName, const std::string& description, const std::string& level, const std::string props, const std::string equipmentType, const int equipmentLevel, const bool isPlayerUsing, const bool isInUpgrade, int typeId, std::string weaponElement);
    EquipmentDetails();
    ~EquipmentDetails();
    static EquipmentDetails* create(const std::string imagePath, const std::string equipmentName, const std::string& description, const std::string& level, const std::string props, const std::string equipmentType, const int equipmentLevel, const bool isPlayerUsing, const bool isInUpgrade, int typeId, std::string weaponElement);
    virtual bool init() override;
    bool isPlayerUse() const {}
    void setPlayerUsing(bool isUsing) {}
    void setInventory(Inventory* inventory) { _inventory = inventory; };
    std::string getEquipmentDetailsName() const { return _equipmentName; };
    bool onTouchBegan(Touch* touch, Event* event);
    void setUpgradeInventory(UpgradeInventory* inventory) { _upgradeInventory = inventory; };
    Sprite* background = nullptr;
    Sprite* equipButton = nullptr;
    Sprite* dropButton = nullptr;
    Inventory* _inventory = nullptr;
    UpgradeInventory* _upgradeInventory = nullptr;
    void hide();
    void show();
private:    
    std::string _weaponElement;
    std::string _imagePath = "";
    std::string _equipmentName = "";
    std::string _equipmentType = "";
    int _equipmentTypeId = -1;
    std::string _description = "";
    std::string _level = "";
    std::string _props = "";
    bool _isInUpgrade = false;
    bool _isPlayerUsing = false;
    int _equipmentLevel = 0;

};

#endif // __EQUIPMENT_DETAILS__