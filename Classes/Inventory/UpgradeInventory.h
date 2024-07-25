#ifndef __UPGRADE_INVENTORY_H__
#define __UPGRADE_INVENTORY_H__

#include "cocos2d.h"
#include "../Equipment/EquipmentFactory.h"
#include "../Equipment/BaseEquipment.h"
#include "../Item/ItemFactory.h"
#include "./InventoryNode.h"
#include "../UserInterface/InGame/UpgradeProgressBar.h"
#include "UserInterface/InGame/ItemDetails.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
class Player;
class Scrollview;
class UpgradeDetails;

class UpgradeInventory : public Node
{
public:
    static UpgradeInventory* createUpgradeInventory(Player* _player);
    virtual bool init(Player* _player);
    void addEquipment(std::string equipmentName, int equipmentLv, int quantity);
    void onAddEquipment();
    Scrollview* scrollview;
    void addItem(std::string itemName, int quantity);
    void dropItem(std::string itemName);
    void dropEquipment(std::string itemName);
    void removeEquipment(std::string equipmentName);
    void removeItem(std::string itemName);
    void showUpgradeInventory();
    void hideUpgradeInventory();
    bool isShow() const { return isSo; };
    Size visibleSize;
    Size bgSize;
    Sprite* bg = nullptr;
    bool onTouchBegan(Touch* touch, Event* event);
    bool onTouchBegan2(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    bool isUpgrade = false;
    float widthSize;
    Sprite* bg2;
    InventoryNode* weaponNode;
    InventoryNode* materialItem1;
    InventoryNode* materialItem2;
    InventoryNode* materialItem3;
    BaseEquipment* equipment;
    BaseItem* item;
    InventoryNode* currentNode;
    Node* playerStats;
    Label* dmgStat;
    Label* mpStat;
    Label* hpStat;
    Label* msStat;
    Label* cdrStat;
    Label* armStat;
    UpgradeProgressBar* upgradeProgressBar;
    std::vector<InventoryNode*> inventoryNodes;
    void ShowEquipmentDetails(std::string eName, int eCurrentLevel);
    UpgradeDetails* upgradeDetails = nullptr;
    InventoryNode* upgradeEIcon = nullptr;
    Label* eNameLabel = nullptr;
    Label* eStatsLabel = nullptr;
    Label* eCurrentLevelLabel = nullptr;
    InventoryNode* itemSlot1 = nullptr;
    InventoryNode* itemSlot2 = nullptr;
    InventoryNode* itemSlot3 = nullptr;
    Sprite* upgradeButton = nullptr;
    Label* durinStoreLabel = nullptr;
    std::vector<InventoryNode*> materialNodes;
    std::vector<std::tuple<std::string, int>> itemsAndQuantity;
    Sprite* popupBack = nullptr;
    bool isUpgradeSuccess = true;
    Label* popupFront = nullptr;
    int currentEquipmentId = 0;
    Game* game = nullptr;
    std::string currentEquipmentName = "";
    int currentEquipmentLv = 1;
    std::string currentIconPath = "";
    BaseEquipment* equipmentTemp = nullptr;
    Label* currentLabel = nullptr;
    void resetUI();
    void onPageChange();
    void nextInventoryPage();
    void prevInventoryPage();
    void resetMaterialNodes();
    void sort();
    BaseEquipment* onUpgradeEquipment = nullptr;
   // void upgradeEquipment();
private:
    std::vector<BaseEquipment::EquipmentData*> equipmentsData;
    const int totalInventoryPage = 3;
    const int numRows = 5;
    const int numCols = 5;
    int currentInventoryPage = 0;
    Sprite* prevInvenButton = nullptr;
    Sprite* nextInvenButton = nullptr;
    Label* pageLabel = nullptr;
    void showItemDetails(BaseItem* item, bool isInMaterialsNode);
    Sprite* buttonClose;
    Sprite* mainUpgradeInventory;
    Sprite* subUpgradeInventory;
    Sprite* sortButton;
    EventListenerTouchOneByOne* touchListener;
    int movingItemQuantity;
    bool compareUpgradeItem(const std::vector<std::tuple<std::string, int>>& tuples1, const std::vector<std::tuple<std::string, int>>& tuples2);
    Label* updateButton;
    Node* inventoryBorder;
    Player* _player = nullptr;
    bool isDraggingItem = false;
    bool isSo = false;
    bool isInUpgrade = false;
    Vec2 touchStartPoint;
    cocos2d::Node* currentSwipeNode = nullptr;
    std::vector<cocos2d::Node*> nodesToAdd;
    void adjustNodesPosition();
    float minY = 0, maxY = minY;
    Node* crENode = nullptr;
};

#endif // __UPGRADE_INVENTORY_H__
