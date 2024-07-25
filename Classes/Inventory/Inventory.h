#ifndef __INVENTORY_H__
#define __INVENTORY_H__

#include "cocos2d.h"
#include "../Equipment/EquipmentFactory.h"
#include "../Equipment/BaseEquipment.h"
#include "./InventoryNode.h"

USING_NS_CC;
class Player;
class SubInventory;
class EquipmentDetails;
class Inventory : public Layer
{
public:
    static Inventory* createInventory(Player* _player);
    virtual bool init(Player* _player);
    void addEquipment(std::string equipmentName, int equipmentLv, int quantity);
    void addItem(std::string itemName, int quantity);
    void dropItem(std::string itemName);
    void sort();
    static bool compareByEquipmentType(const InventoryNode* a, const InventoryNode* b);
    static bool isSorting;
    static bool isSorted;
    void dropEquipment(std::string itemName);
    void removeEquipment(std::string equipmentName);
    void removeItem(std::string itemName);
    void showInventory();
    void hideInventory();
    void addItemToItemNodes(std::string itemName, int quantity);
    void removeItemFromItemNodes(std::string itemName);
    void hideItem();
    void showItem();
    std::pair<std::string, int> getItem(std::string itemName);
    std::vector<std::string> getAllItem();
    std::vector<BaseEquipment::EquipmentData> getAllEquipment();
    bool isShow() const { return isSo; };
    void upgradeEquipment(std::string equipmentName);
    float widthSize;
    Player* player;
    InventoryNode* getFirstNode() const { return inventoryNodes[0]; };
    int clickButtonSoundId;
    std::vector<InventoryNode*> inventoryNodes;
    InventoryNode* weaponNode;
    InventoryNode* ringNode;
    InventoryNode* armorNode;
    InventoryNode* shoeNode;
    InventoryNode* shieldNode;
    InventoryNode* helmetNode;
    BaseEquipment* equipment;
    InventoryNode* currentNode;
    InventoryNode* itemNode1 = nullptr;
    InventoryNode* itemNode2 = nullptr;
    InventoryNode* itemNode3 = nullptr;
    InventoryNode* itemNode4 = nullptr;
    LayerColor* overlay = nullptr;
    Vec2 prevPoint = Vec2::ZERO;
    std::vector<InventoryNode*> listItemNodes;
    Node* playerStats;
    Label* dmgStat;
    Label* mpStat;
    Label* hpStat;
    Label* msStat;
    Label* cdrStat;
    Label* armStat;
    Label* level;
    void setSubInventory(SubInventory* subInventory) { _subInventory = subInventory; };
    void equipWeapon(std::string name, int level);
    void equipShoe(std::string name, int level);
    void equipArmor(std::string name, int level);
    void equipRing(std::string name, int level);
    void equipShield(std::string name, int level);
    void equipHelmet(std::string name, int level);
    void decreaseItemQuantity(std::string itemName, int quantity);
    void nextInventoryPage();
    void prevInventoryPage();
    void onPageChange();
private:
    const int totalInventoryPage = 3;
    const int numRows = 5;
    const int numCols = 6;
    int currentInventoryPage = 0;
    Sprite* prevInvenButton = nullptr;
    Sprite* nextInvenButton = nullptr;
    Label* pageLabel = nullptr;
    std::vector<std::vector<InventoryNode*>> inventoryPages;
    int currentTutorial = 0;
    std::vector<EquipmentDetails*> listEquipmentDetails;
    SubInventory* _subInventory;
    Sprite* buttonClose;
    Sprite* mainInventory;
    Sprite* subInventory;
    Sprite* sortButton;
    EventListenerTouchOneByOne* touchListener;
    BaseItem* item;
    int movingItemQuantity;
    Node* inventoryBorder;
    void showEquipmentDetails(BaseEquipment* equipment, bool isPlayerUsing);
    void showItemDetails(BaseItem* item);
    void showItemDetails(BaseItem* item, bool isInQuickInven);
    bool isDraggingItem = false;
    bool isSo = false;
    bool isPlayerUsing = false;

};

#endif // __INVENTORY_H__