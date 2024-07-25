#include "UpgradeDetails.h"
#include "Inventory/InventoryNode.h"
#include "Game/Game.h"
UpgradeDetails* UpgradeDetails::create()
{
    auto inventoryNode = new (std::nothrow) UpgradeDetails();
    if (inventoryNode && inventoryNode->init())
    {
        inventoryNode->autorelease();
        return inventoryNode;
    }
    else
    {
        delete inventoryNode;
        return nullptr;
    }
}

bool UpgradeDetails::init()
{
    if (!Node::init())
    {
        return false;
    }

    eNameAndLvLabel = Label::createWithTTF("", "fonts/Diablo Light.ttf", 20);
    eNameAndLvLabel->setScale(0.3);
    this->addChild(eNameAndLvLabel);

    //eNode = InventoryNode::createInventoryNode(12 * Director::getInstance()->getContentScaleFactor());
    //eNode->setPosition(Vec2(-eNameAndLvLabel->getContentSize().width / 2 - eNode->getContentSize().width / 2, 0));
    //this->addChild(eNode);

    eStatsLabel = Label::createWithTTF("", "fonts/Diablo Light.ttf", 20);
    eStatsLabel->setScale(0.3);
    this->addChild(eStatsLabel);

    //upgradeButton = Sprite::create("res/buttonUpgradeE.png"); // , "res/buttonUpgradeE_push.png"

    //auto text = Label::createWithTTF("Upgrade", "fonts/Diablo Light.ttf", 18);
    //text->setPosition(upgradeButton->getContentSize() / 2);
    //text->setScale(0.29);
    //upgradeButton->addChild(text);
    //upgradeButton->setPosition(Vec2(eNameAndLvLabel->getPositionX(), 0));
    //this->addChild(upgradeButton);

   /* Scene* currentScene = Director::getInstance()->getRunningScene();
    Game* game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));*/
   /* std::vector<InventoryNode*> materialNodes;
    for (int i = 0; i < 3; i++) {
        InventoryNode* node = InventoryNode::createInventoryNode(10 * Director::getInstance()->getContentScaleFactor());
        this->addChild(node);
    }*/

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(UpgradeDetails::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(UpgradeDetails::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    /*Scene* currentScene = Director::getInstance()->getRunningScene();
    Game* game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));*/
    return true;
}

void UpgradeDetails::setUpgradeDetails(UpgradeDetailsStruct temp) {
    //if(eNameAndLvLabel && temp.name.length() > 0 && temp.level > 0)
    //eNameAndLvLabel->setString(temp.name + " || Lv. " + std::to_string(temp.level));

    ////if (eNode->getBaseEquipment()) eNode->removeBaseEquipment();
    ////eNode->setBaseEquipment(temp.name, temp.level);
    ////for (auto node : materialNodes) {
    ////    node->removeBaseItem();
    ////    node->removeMaterialCondition();
    ////}
    ////for (auto node : materialNodes) {
    ////    auto matToUpgrade = eNode->getBaseEquipment()->getMaterialsToUpgrade(temp.level);
    ////    node->setMaterialsToUpgrade();
    ////}
    //if (eStatsLabel)
    //eStatsLabel->setString(temp.statsStr);
    
    
}
bool UpgradeDetails::onTouchBegan(Touch* touch, Event* event) {
    if (isShow()) {
        Vec2 touchLocation = this->convertToNodeSpace(touch->getLocation());
        if (upgradeButton->getBoundingBox().containsPoint(touchLocation))
        {
            upgradeButton->setTexture("res/buttonUpgradeE_push.png");
            isUpgradeTouchBegan = true;
        } 
    }
}

void UpgradeDetails::onTouchEnded(Touch* touch, Event* event) {
    Vec2 touchLocation = this->convertToNodeSpace(touch->getLocation());
    if (isUpgradeTouchBegan)
    {
        upgradeButton->setTexture("res/buttonUpgradeE.png");
        isUpgradeTouchBegan = false;
    }

}

void UpgradeDetails::hide() {
    checkIsShow = false;
    this->setVisible(false);
}

void UpgradeDetails::show() {
    checkIsShow = true;
    this->setVisible(true);
}

UpgradeDetails::~UpgradeDetails() {
    eNameAndLvLabel->removeFromParentAndCleanup(true);
    eNode->removeFromParentAndCleanup(true);
    eStatsLabel->removeFromParentAndCleanup(true);
    upgradeButton->removeFromParentAndCleanup(true);
}