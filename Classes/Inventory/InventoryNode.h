#ifndef __INVENTORY_NODE_H__
#define __INVENTORY_NODE_H__

#include "cocos2d.h"
#include "../Equipment/EquipmentFactory.h"
#include "../Item/ItemFactory.h"
#include "../Equipment/BaseEquipment.h"
#include <iostream>
USING_NS_CC;
class InventoryNode : public Node
{
public:
    static InventoryNode* createInventoryNode(float size);
    virtual bool init(float size);
    void setId(int id);
    int getId() const;
    Rect getBoundingNode() const;
    Rect getBoundingNode(float amt) const;
    bool setBaseEquipment(std::string,int);
    bool setBaseItem(std::string);
    BaseEquipment* getBaseEquipment() const;
    BaseItem* getBaseItem() const;
    BaseItem* getBaseItemToUpgrade() const;
    void setStatus(std::string stt);
    std::string getStatus() const;
    void setQuantity(int qtt);
    int getQuantity() const;
    bool removeBaseEquipment();
    bool removeBaseItem();
    bool decreaseBaseEquipment(int);
    bool decreaseBaseItem(int);
    bool decreaseBaseItem(std::string, int);
    void setMaterialsToUpgrade(std::string itemName, int itemQuantity);
    void removeMaterialCondition();
    void setSubInven(bool isTrue) { _subInven = isTrue; };
    void setIsInventory(bool isTrue) { _inven = isTrue; };
    void hide();
    void show();
    BaseItem* conditionItem;
    Sprite* getNodeSprite() {
        return nodeSpr;
    }
    bool checkQuantityRequire() const { return checkQuantityRequired; };
    ~InventoryNode();
private:
    bool checkQuantityRequired = false;
    bool _subInven;
    bool _inven;
    Sprite* nodeSpr;
    std::string status; // busy, free
    BaseEquipment* baseEquipment;
    BaseItem* baseItem;
    Label* quantityLabel;
    int quantityItem;
    int id;
    float nodeScaleX;
    float nodeScaleY;
    bool isDraggingItem = false;
    bool isMaterialNode = false;
    int requiredQuantity = 0;
    Node* node;
};

#endif // __INVENTORY_NODE_H__