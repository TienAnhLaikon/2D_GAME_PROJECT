#ifndef __UPGRADE_DETAILS__
#define __UPGRADE_DETAILS__

#include "cocos2d.h"
USING_NS_CC;
class InventoryNode;
struct UpgradeDetailsStruct {
    std::string name;
    int level;
    std::string statsStr;
    UpgradeDetailsStruct(std::string name, int level, std::string statsStr) : name(name), level(level), statsStr(statsStr) {}
};
class UpgradeDetails : public Node
{
public:
    static UpgradeDetails* create();
    ~UpgradeDetails();
    virtual bool init();
    void hide();
    void show();
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    bool isShow() const { return checkIsShow; }
    void setUpgradeDetails(UpgradeDetailsStruct temp);
    Label* eNameAndLvLabel = nullptr;
    InventoryNode* eNode = nullptr;
    Label* eStatsLabel = nullptr;
    Sprite* upgradeButton = nullptr;
    std::vector<InventoryNode*> materialNodes;
private:
    bool checkIsShow = false;
    bool isUpgradeTouchBegan = false;
};

#endif // __UPGRADE_DETAILS__