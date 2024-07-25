#ifndef __SUB_INVENTORY__
#define __SUB_INVENTORY__

#include "cocos2d.h"
#include "../Equipment/EquipmentFactory.h"
#include "../Equipment/BaseEquipment.h"
#include "../Item/ItemFactory.h"
#include "./InventoryNode.h"
#include "../UserInterface/InGame/UpgradeProgressBar.h"

USING_NS_CC;
class Player;
class Inventory;
class Game;
class SubInventory : public Node
{
public:
    static SubInventory* createSubInventory();
    virtual bool init();
    void setInventory(Inventory* inventory) { _inventory = inventory; };
    void addItem(std::string itemName, int quantity);
    void addItem(std::string itemName, int quantity, int id);
    void removeItem(std::string itemName);
    float widthSize;
    InventoryNode* currentNode;
    InventoryNode* weaponNode;
    InventoryNode* materialItem1;
    InventoryNode* materialItem2;
    InventoryNode* materialItem3;
    BaseEquipment* equipment;
    BaseItem* item;
    Node* playerStats;
    Label* dmgStat;
    Label* mpStat;
    Label* hpStat;
    Label* msStat;
    Label* cdrStat;
    Label* armStat;
    std::vector<InventoryNode*> inventoryNodes;
    UpgradeProgressBar* upgradeProgressBar;
    std::vector<InventoryNode*> materialNodes;
    std::vector<std::pair<std::string, int>> getSubInventoryItems();
    Game* currentScene;
private:
    Sprite* bottomInventory;
    Inventory* _inventory;
    Sprite* buttonClose;
    Sprite* mainSubInventory;
    Sprite* subSubInventory;
    EventListenerTouchOneByOne* touchListener;
    int movingItemQuantity;
    Label* updateButton;
    Node* inventoryBorder;
    Player* player;
    bool isDraggingItem = false;
    bool isSo = false;
    bool isInUpgrade = false;
};

#endif // __SUB_INVENTORY__