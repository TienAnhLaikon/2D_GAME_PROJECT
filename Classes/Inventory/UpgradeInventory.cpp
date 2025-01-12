#include "UpgradeInventory.h"
#include "../Player/Player.h"
#include <unordered_set>
#include "./UserInterface/InGame/EquipmentDetails.h"
#include "./UserInterface/InGame/ItemDetails.h"
#include "./UserInterface/InGame/Scrollview.h"
#include "Equipment/BaseEquipment.h"
#include "Inventory/Inventory.h"
#include "Inventory/InventoryNode.h"
#include "Game/Game.h"
UpgradeInventory* UpgradeInventory::createUpgradeInventory(Player* player)
{
    auto inventory = new (std::nothrow) UpgradeInventory();
    if (inventory && inventory->init(player))
    {
        inventory->autorelease();
        return inventory;
    }
    else
    {
        delete inventory;
        return nullptr;
    }
}

bool UpgradeInventory::compareUpgradeItem(const std::vector<std::tuple<std::string, int>>& tuples1, const std::vector<std::tuple<std::string, int>>& tuples2) {
    int correctCount = 0;
    for (const auto& tuple1 : tuples1) {
        std::string str1 = std::get<0>(tuple1);
        int int1 = std::get<1>(tuple1);
        for (const auto& tuple2 : tuples2) {
            std::string str2 = std::get<0>(tuple2);
            if (str2 == str1) {
                int int2 = std::get<1>(tuple2);
                if (int1 >= int2) {
                    for (auto materials : materialNodes)
                    {
                        if (materials->getBaseItem() && materials->getBaseItem()->getItemName() == str1) {
                            materials->decreaseBaseItem(int2);
                        }
                    }
                    correctCount++;
                }
            }
        }
    }
    if (correctCount != static_cast<int>(tuples2.size())) return false;
    else return true;
}


bool UpgradeInventory::init(Player* player)
{
    if (!Node::init())
    {
        return false;
    }

    visibleSize = Director::getInstance()->getVisibleSize();
    bg = Sprite::create("res/background-img-3.5.png");
    bg2 = Sprite::create("res/background-img-3-semi.png");
    bg2->setAnchorPoint(Vec2(0,0));
    bg2->setPosition(Vec2(-bg->getContentSize().width / 2, -bg->getContentSize().height / 2));
    this->addChild(bg);
    touchStartPoint = Vec2::ZERO;
    this->addChild(bg2, 10);
    buttonClose = Sprite::create("res/c_header_close.png");
    buttonClose->setPosition(Vec2(bg->getContentSize().width / 2, bg->getContentSize().height / 2) - buttonClose->getContentSize() * 1.5);
    this->addChild(buttonClose);
    bgSize = bg->getContentSize();
    durinStoreLabel = Label::createWithTTF("Durin's Store", "fonts/Diablo Light.ttf", 20);
    durinStoreLabel->setScale(0.6);
    durinStoreLabel->setPosition(Vec2(0, bgSize.height / 2 - durinStoreLabel->getContentSize().height));
    this->addChild(durinStoreLabel, 50);
    hideUpgradeInventory();
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(UpgradeInventory::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(UpgradeInventory::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(UpgradeInventory::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    Scene* currentScene = Director::getInstance()->getRunningScene();
    game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
    return true;
}
void UpgradeInventory::adjustNodesPosition() {
}
bool UpgradeInventory::onTouchBegan(Touch* touch, Event* event) {
    if (isShow()) {
        Vec2 touchLocation = this->convertToNodeSpace(touch->getLocation());
        if(bg2->getBoundingBox().containsPoint(touchLocation)) {
            touchStartPoint = touchLocation;
        } else {
            touchStartPoint = Vec2::ZERO;
        }

        if (buttonClose->getBoundingBox().containsPoint(touchLocation)) {
            hideUpgradeInventory();
        }
        if (upgradeButton != nullptr && upgradeButton->getBoundingBox().containsPoint(touchLocation)) {
            UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
            upgradeButton->setTexture("res/buttonUpgradeE_push.png");
        }

        //for (int i = 0; i < nodesToAdd.size(); i++) {
        //    if (nodesToAdd[i]->getBoundingBox().containsPoint(touchLocation)) {
        //        currentNode = equipmentsData[i];
        //    }
        //}

        if (crENode != nullptr && crENode->getChildByName("bg")) {
            Sprite* temp = dynamic_cast<Sprite*>(crENode->getChildByName("bg"));
            if (temp) temp->setTexture("res/item_in_upgrade.png");
            crENode = nullptr;
        }

        for (int i = 0; i < nodesToAdd.size(); i++) {
            Sprite* tempSpr = dynamic_cast<Sprite*>(nodesToAdd[i]->getChildByName("bg"));
            auto temp = Rect(nodesToAdd[i]->getPositionX() - nodesToAdd[i]->getBoundingBox().size.width * 0.5, nodesToAdd[i]->getPositionY() - ((tempSpr == nullptr) ? (nodesToAdd[i]->getBoundingBox().size.height * 1.5) : (tempSpr->getContentSize().height * tempSpr->getScaleY() / 2)), nodesToAdd[i]->getBoundingBox().size.width, nodesToAdd[i]->getBoundingBox().size.height);
            if (temp.containsPoint(touchLocation)) {
                crENode = nodesToAdd[i];
                currentEquipmentId = i;
                break;
            }
        }
        
        return true;
    }
    return false;
}
void UpgradeInventory::onTouchMoved(Touch* touch, Event* event) {
    if (isShow() && touchStartPoint != Vec2::ZERO) {
        Vec2 touchLocation = this->convertToNodeSpace(touch->getLocation());
        bool canMove = true;
        Vec2 delta = touchLocation - touchStartPoint;

        if (!nodesToAdd.empty()) {

            Vec2 firstNodePosition = nodesToAdd[0]->getPosition();
            Vec2 lastNodePosition = nodesToAdd.back()->getPosition();

            if (firstNodePosition.y + delta.y < minY) {
                delta.y = minY - firstNodePosition.y;
            }

            if (lastNodePosition.y + delta.y > maxY) {
                delta.y = maxY - lastNodePosition.y;
            }

            for (auto node : nodesToAdd) {
                Vec2 pos = node->getPosition();
                pos.y += delta.y;
                node->setPosition(pos);
            }
            touchStartPoint = touchLocation;
        }
    }
}
void UpgradeInventory::resetMaterialNodes() {
    if (!materialNodes.empty()) {
        std::vector<InventoryNode*> deleteNodes;
        for (auto i : materialNodes) {
            deleteNodes.push_back(i);
        }
        materialNodes.clear();
        for (auto i : deleteNodes) {
            if (i->getParent()) i->removeFromParentAndCleanup(true);
        }
    }
}
void UpgradeInventory::resetUI() {
    eNameLabel->setString(currentEquipmentName + " || Lv." + std::to_string(currentEquipmentLv));
   /* if (i == currentEquipmentId) {
        Label* temp = static_cast<Label*>(nodesToAdd[i]->getChildByName("lb"));
        if (temp) {
            temp->setString(currentEquipmentName + " || Lv." + std::to_string(currentEquipmentLv));
        }
    }*/
    for (int i = 0; i < nodesToAdd.size(); i++)
    {
        if (i == currentEquipmentId) {
            dynamic_cast<Label*>(nodesToAdd[i]->getChildByName("lb"))->setString(currentEquipmentName + " || Lv." + std::to_string(currentEquipmentLv));
        }
    }

    auto materialsToUpgrade = onUpgradeEquipment->getMaterialsToUpgrade(onUpgradeEquipment->getLevel());
    itemSlot1 = InventoryNode::createInventoryNode(10 * Director::getInstance()->getContentScaleFactor());
    itemSlot1->setPosition(Vec2(bgSize.width * 0.25 - itemSlot1->getBoundingNode().size.width * 1.25, upgradeButton->getPosition().y + upgradeButton->getContentSize().height / 2));
    materialNodes.push_back(itemSlot1);
    this->addChild(itemSlot1);

    itemSlot2 = InventoryNode::createInventoryNode(10 * Director::getInstance()->getContentScaleFactor());
    itemSlot2->setPosition(itemSlot1->getPosition() + Vec2(itemSlot1->getBoundingNode().size.width + 1 * Director::getInstance()->getContentScaleFactor(), 0));
    materialNodes.push_back(itemSlot2);
    this->addChild(itemSlot2);

    itemSlot3 = InventoryNode::createInventoryNode(10 * Director::getInstance()->getContentScaleFactor());
    itemSlot3->setPosition(itemSlot2->getPosition() + Vec2(itemSlot2->getBoundingNode().size.width + 1 * Director::getInstance()->getContentScaleFactor(), 0));
    materialNodes.push_back(itemSlot3);
    this->addChild(itemSlot3);
    for (int i = 0; i < materialNodes.size(); i++) {
        materialNodes[i]->setMaterialsToUpgrade(std::get<0>(materialsToUpgrade.materialsItem[i]), std::get<1>(materialsToUpgrade.materialsItem[i]));
        materialNodes[i]->setBaseItem(std::get<0>(materialsToUpgrade.materialsItem[i]));

    }
    for (auto item1 : game->getPlayer()->getInventory()->getAllItem()) {
        auto item = game->getPlayer()->getInventory()->getItem(item1);
        if (std::get<0>(item).length() > 0) {
            for (auto it : materialNodes) {
                if (it->conditionItem && it->conditionItem->getItemName() == std::get<0>(item)) {
                    it->setQuantity(std::get<1>(item));
                }
            }
        }
    }
    //if (game && game->getPlayer() && game->getPlayer()->getInventory()) {
    //    for (auto node : materialNodes) {
    //        if (node->conditionItem->getItemName().length() > 0) {
    //            auto itemTemp = game->getPlayer()->getInventory()->getItem(node->conditionItem->getItemName());
    //            int quanTemp = node->getQuantity();
    //            if (std::get<1>(itemTemp) > quanTemp) {
    //                node->setQuantity();
    //            }
    //        }
    //    }

    //}

}

void UpgradeInventory::ShowEquipmentDetails(std::string eName, int eCurrentLevel) {
    if (onUpgradeEquipment) onUpgradeEquipment->removeFromParentAndCleanup(true);
    onUpgradeEquipment = EquipmentFactory::createEquipment(eName, eCurrentLevel);
    if (onUpgradeEquipment) {
    this->addChild(onUpgradeEquipment);
    onUpgradeEquipment->setVisible(false);
   
    currentEquipmentName = eName;
    currentEquipmentLv = eCurrentLevel;
    int count = 0;
    bool isNewLine = (count == 2 || count == 5);
    std::string str = "";
    if (onUpgradeEquipment) {
        std::string equipmentAD = onUpgradeEquipment->getDamage() > 0 ? "AD: " + std::to_string(static_cast<int>(onUpgradeEquipment->getDamage())) + "  " : "";
        count++;

        isNewLine = (count == 2 || count == 5);
        std::string equipmentAP = onUpgradeEquipment->getSkillDamage() > 0 ? "AP: " + std::to_string(static_cast<int>(onUpgradeEquipment->getSkillDamage())) : isNewLine == true ? "\n" : "";
        count++;
        isNewLine = (count == 2 || count == 5);
        std::string equipmentArm = onUpgradeEquipment->getArmor() > 0 ? "Armor: " + std::to_string(static_cast<int>(onUpgradeEquipment->getArmor())) : isNewLine == true ? "\n" : "  ";
        count++;
        isNewLine = (count == 2 || count == 5);
        std::string equipmentCDR = onUpgradeEquipment->getCDR() > 0 ? "\nCDR (Cooldown): " + std::to_string(static_cast<int>(onUpgradeEquipment->getArmor())) + "  " : "";
        count++;
        isNewLine = (count == 2 || count == 5);
        std::string equipmentHP = onUpgradeEquipment->getHP() > 0 ? "HP: " + std::to_string(static_cast<int>(onUpgradeEquipment->getArmor())) : isNewLine == true ? "\n" : "  ";
        count++;
        isNewLine = (count == 2 || count == 5);
        std::string equipmentMP = onUpgradeEquipment->getMP() > 0 ? isNewLine == true ? " \nMP: " : "\n" + std::to_string(static_cast<int>(onUpgradeEquipment->getMP())) : "  ";
        count++;
        isNewLine = (count == 2 || count == 5);
        std::string equipmentMS = onUpgradeEquipment->getMovementSpeed() > 0 ? "MS: " + std::to_string(static_cast<int>(onUpgradeEquipment->getMovementSpeed())) : isNewLine == true ? "\n" : "  ";
        str = equipmentAD + equipmentAP + equipmentArm + equipmentCDR + equipmentHP + equipmentMP + equipmentMS;
    }
   
    UpgradeDetailsStruct temp = UpgradeDetailsStruct(onUpgradeEquipment->getEquipmentName(), onUpgradeEquipment->getLevel(), str);
    if (temp.name.length() > 0) {
        if (upgradeEIcon == nullptr) {
            upgradeEIcon = InventoryNode::createInventoryNode(15 * Director::getInstance()->getContentScaleFactor());
            upgradeEIcon->setPosition(0, 0);
            this->addChild(upgradeEIcon, 20);
        }
        upgradeEIcon->removeBaseEquipment();
        upgradeEIcon->setBaseEquipment(temp.name, temp.level);
        upgradeEIcon->setQuantity(1);

        if (eNameLabel == nullptr) {
            eNameLabel = Label::createWithTTF(temp.name + " || Lv." + std::to_string(temp.level), "fonts/Diablo Light.ttf", 20);
            eNameLabel->setScale(0.29);
            eNameLabel->setPosition(Vec2(upgradeEIcon->getPosition().x + eNameLabel->getContentSize().width * eNameLabel->getScaleX() / 2 + upgradeEIcon->getBoundingNode().size.width * 1.5, upgradeEIcon->getBoundingNode().size.height / 2));
            this->addChild(eNameLabel, 20);
        }
        eNameLabel->setString(temp.name + " || Lv." + std::to_string(temp.level));

        if (eStatsLabel == nullptr) {
            eStatsLabel = Label::createWithTTF("", "fonts/Diablo Light.ttf", 20);
            eStatsLabel->setScale(0.3);
            this->addChild(eStatsLabel, 20);
        }
        eStatsLabel->setString(temp.statsStr);

        if (upgradeButton) upgradeButton->removeFromParent();
        upgradeButton = Sprite::create("res/buttonUpgradeE.png"); // , "res/buttonUpgradeE_push.png"
        auto text = Label::createWithTTF("Upgrade", "fonts/Diablo Light.ttf", 18);
        text->setPosition(upgradeButton->getContentSize() / 2);
        text->setScale(0.29);
        upgradeButton->addChild(text);
        upgradeButton->setPosition(Vec2(eNameLabel->getPositionX(), -bgSize.height / 2 + upgradeButton->getContentSize().height + 2 * Director::getInstance()->getContentScaleFactor()));
        this->addChild(upgradeButton, 20);
        upgradeButton->retain();
        

        BaseEquipment::LevelUpMaterials materialsToUpgrade = onUpgradeEquipment->materialsToUpgrade[eCurrentLevel - 1];

        if (!materialNodes.empty()) {
            std::vector<InventoryNode*> deleteNodes;
            for (auto i : materialNodes) {
                deleteNodes.push_back(i);
            }
            for (auto i : deleteNodes) {
                if (i->getParent()) i->removeFromParentAndCleanup(true);
            }
            materialNodes.clear();
            deleteNodes.clear();
        }
        if (itemSlot1) {
            itemSlot1->removeFromParentAndCleanup(true);
            itemSlot1 = nullptr;
        }
        if (itemSlot2) {
            itemSlot2->removeFromParentAndCleanup(true);
            itemSlot2 = nullptr;
        }
        if (itemSlot3) {
            itemSlot3->removeFromParentAndCleanup(true);
            itemSlot3 = nullptr;
        }
        if (upgradeButton) {
            itemSlot1 = InventoryNode::createInventoryNode(10 * Director::getInstance()->getContentScaleFactor());
            itemSlot1->setPosition(Vec2(bgSize.width * 0.25 - itemSlot1->getBoundingNode().size.width * 1.25, upgradeButton->getPosition().y + upgradeButton->getContentSize().height / 2));
            materialNodes.push_back(itemSlot1);
            this->addChild(itemSlot1);

            itemSlot2 = InventoryNode::createInventoryNode(10 * Director::getInstance()->getContentScaleFactor());
            itemSlot2->setPosition(itemSlot1->getPosition() + Vec2(itemSlot1->getBoundingNode().size.width + 1 * Director::getInstance()->getContentScaleFactor(), 0));
            materialNodes.push_back(itemSlot2);
            this->addChild(itemSlot2);

            itemSlot3 = InventoryNode::createInventoryNode(10 * Director::getInstance()->getContentScaleFactor());
            itemSlot3->setPosition(itemSlot2->getPosition() + Vec2(itemSlot2->getBoundingNode().size.width + 1 * Director::getInstance()->getContentScaleFactor(), 0));
            materialNodes.push_back(itemSlot3);
            this->addChild(itemSlot3);
            for (int i = 0; i < materialsToUpgrade.materialsItem.size(); i++) {
                materialNodes[i]->setMaterialsToUpgrade(std::get<0>(materialsToUpgrade.materialsItem[i]), std::get<1>(materialsToUpgrade.materialsItem[i]));
                materialNodes[i]->setBaseItem(std::get<0>(materialsToUpgrade.materialsItem[i]));

            }
            if (game && game->inGameUI && game->inGameUI->inventory)
                for (auto item1 : game->inGameUI->inventory->getAllItem()) {
                    auto item = game->inGameUI->inventory->getItem(item1);
                    if (std::get<0>(item).length() > 1) {
                        for (auto it : materialNodes) {
                            if (it->conditionItem && it->conditionItem->getItemName() == std::get<0>(item)) {
                                it->setQuantity(std::get<1>(item));
                            }
                        }
                    }
                }
        }
    }
    }
    
       
}
  

void UpgradeInventory::onTouchEnded(Touch* touch, Event* event) {
    if (isShow()) 
    {
        Vec2 touchLocation = this->convertToNodeSpace(touch->getLocation());
        if (crENode && crENode->getChildByName("bg")) 
        {
            Sprite* temp = dynamic_cast<Sprite*>(crENode->getChildByName("bg"));
            currentLabel = dynamic_cast<Label*>(crENode->getChildByName("lb"));
            if (temp) 
            {
                temp->setTexture("res/item_in_upgrade_push.png");
                if(currentEquipmentId != -1)
                    this->ShowEquipmentDetails(equipmentsData[currentEquipmentId]->name, equipmentsData[currentEquipmentId]->level);
            }
        }
        if (upgradeButton && upgradeButton->getBoundingBox().containsPoint(touchLocation) && isUpgradeSuccess)
        {
            Scene* currentScene = Director::getInstance()->getRunningScene();
            game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
            upgradeButton->setTexture("res/buttonUpgradeE.png");
            for (auto node : materialNodes) {
                if (node && node->getStatus() == "busy" && node->checkQuantityRequire() == false) {
                    isUpgradeSuccess = false;
                    break;
                }
            }

            if (isUpgradeSuccess) {
                currentEquipmentLv += 1;
                if (currentEquipmentName != "" && currentEquipmentLv > 0) {
                    std::string tempStr1 = "Upgrade " + currentEquipmentName + " To Level " + std::to_string(currentEquipmentLv) + " Successfully!";
                    NotificationManager::getInstance()->showMessageNotification(tempStr1, Vec2::ZERO, Color3B::GREEN, 16);
                    if (game && game->getPlayer())
                        game->getPlayer()->upgradeEquipment(currentEquipmentName);
                    
                    if (currentLabel) {
                        
                        if (currentLabel) currentLabel->setString(currentEquipmentName + " || Lv." + std::to_string(currentEquipmentLv));
                    }

                        if(currentEquipmentId != -1 && equipmentsData[currentEquipmentId])
                            equipmentsData[currentEquipmentId]->level += 1;
                    this->ShowEquipmentDetails(currentEquipmentName, currentEquipmentLv);
                }
              //  resetUI();
            }
            else {
                std::string tempStr2 = "Upgrade false, not enough materials!";
                NotificationManager::getInstance()->showMessageNotification(tempStr2, Vec2::ZERO, Color3B::RED, 16);

            }
            isUpgradeSuccess = true;
            return;
        }
    }
}
void UpgradeInventory::onAddEquipment() {
    int equipmentsQuan = equipmentsData.size() - 1;
    auto i = equipmentsData[equipmentsQuan];
    Node* node = Node::create();
    auto bgt = Sprite::create("res/item_in_upgrade.png");
    bgt->setName("bg");
    node->addChild(bgt);
    Label* lb = Label::createWithTTF(i->name + " Lv." + std::to_string(i->level), "fonts/Diablo Light.ttf", 20);
    lb->setScale(0.23);
    lb->setName("lb");
    /*if (equipmentTemp->getEquipmentType() == BaseEquipment::Type::ENHANCED) {
        eNameLabel->setTextColor(Color4B::GREEN);
    }
    else if (equipmentTemp->getEquipmentType() == BaseEquipment::Type::RARE) {
        eNameLabel->setTextColor(Color4B::MAGENTA);
    }
    else if (equipmentTemp->getEquipmentType() == BaseEquipment::Type::LEGENDARY) {
        eNameLabel->setTextColor(Color4B::RED);
    }*/
    node->addChild(lb);
    Sprite* icon = Sprite::create(i->iconPath);
    node->addChild(icon);
    icon->setScale(0.69);
    bgt->setScale(bgt->getContentSize().width / bg2->getContentSize().width * 0.9 * Director::getInstance()->getContentScaleFactor(), bgt->getContentSize().height / (icon->getContentSize().height * icon->getScaleY() * 8));
    icon->setPosition(Vec2(bg2->getContentSize().width / 2 - icon->getContentSize().width * icon->getScaleX(), 0));
    node->setContentSize(bgt->getContentSize() * bgt->getScale());
    node->setAnchorPoint(Vec2(0,0));
    node->setPosition(Vec2(-bgSize.width / 2 + bgt->getContentSize().width * bgt->getScaleX() / 2, bgSize.height / 4 - (icon->getContentSize().height * (equipmentsQuan + 1))));
    this->addChild(node, 5);
    node->retain();
    nodesToAdd.push_back(node);
    if (nodesToAdd.size() == 1) {
        minY = bgSize.height / 4 - (icon->getContentSize().height * (equipmentsQuan + 1));
    }
    if (nodesToAdd.size() == 6) {
        maxY = nodesToAdd[nodesToAdd.size() - 1]->getPosition().y + nodesToAdd[nodesToAdd.size() - 1]->getContentSize().height / 2;
    }
}
/// <summary>
/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// </summary>
void UpgradeInventory::nextInventoryPage() {
    if (prevInvenButton->isVisible() == false) prevInvenButton->setVisible(true);
    if (currentInventoryPage < totalInventoryPage - 1) {
        currentInventoryPage++;
        onPageChange();
    }
}

void UpgradeInventory::prevInventoryPage() {
    if (nextInvenButton->isVisible() == false) nextInvenButton->setVisible(true);

    if (currentInventoryPage > 0) {
        currentInventoryPage--;
        onPageChange();
    }
}

void UpgradeInventory::onPageChange() {
    for (int i = 0; i < inventoryNodes.size(); i++) {
        if (i >= (currentInventoryPage == 0 ? 0 : currentInventoryPage * (numRows * numCols)) && i < (currentInventoryPage + 1) * (numRows * numCols)) {
            inventoryNodes[i]->setVisible(true);
            inventoryNodes[i]->show();
        }
        else {
            inventoryNodes[i]->setVisible(false);
            inventoryNodes[i]->hide();
        }
    }
    if (currentInventoryPage == 0) prevInvenButton->setVisible(false);
    if (currentInventoryPage == (totalInventoryPage - 1)) nextInvenButton->setVisible(false);
    pageLabel->setString(std::to_string(currentInventoryPage + 1) + "/" + std::to_string(totalInventoryPage));
}

void UpgradeInventory::sort() {
    if (UpgradeInventory::isShow()) {
        for (int i = 0; i < inventoryNodes.size(); i++) {
            for (int j = i + 1; j < inventoryNodes.size(); j++) {
                if (inventoryNodes[i]->getBaseItem() && inventoryNodes[j]->getBaseItem() && inventoryNodes[i]->getBaseItem()->getItemName() == inventoryNodes[j]->getBaseItem()->getItemName()) {
                    inventoryNodes[i]->setQuantity(inventoryNodes[i]->getQuantity() + inventoryNodes[j]->getQuantity());
                    inventoryNodes[j]->removeBaseItem();
                }
            }
        }
        for (int i = 0; i < inventoryNodes.size(); i++) {
            for (int j = i + 1; j < inventoryNodes.size(); j++) {
                if (inventoryNodes[i]->getStatus() == "free" && (inventoryNodes[j]->getBaseEquipment() || inventoryNodes[j]->getBaseItem())) {
                    if (inventoryNodes[j]->getBaseEquipment()) {
                        inventoryNodes[i]->setBaseEquipment(inventoryNodes[j]->getBaseEquipment()->getEquipmentName(), inventoryNodes[j]->getBaseEquipment()->getLevel());
                        inventoryNodes[i]->setQuantity(inventoryNodes[j]->getQuantity());
                        inventoryNodes[j]->removeBaseEquipment();
                    }
                    else if (inventoryNodes[j]->getBaseItem()) {
                        inventoryNodes[i]->setBaseItem(inventoryNodes[j]->getBaseItem()->getItemName());
                        inventoryNodes[i]->setQuantity(inventoryNodes[j]->getQuantity());
                        inventoryNodes[j]->removeBaseItem();
                    }
                }
            }
        }
    }
}


void UpgradeInventory::showItemDetails(BaseItem* item, bool isInMaterialsNode) {
    std::string path = item->getItemSprite()->getTexture()->getPath();

    std::string name = item->getItemName();
    if (item->itemDetails == nullptr) {
        item->itemDetails = ItemDetails::create(path, name, item->getItemDesc(), item->getItemType(), "", true, movingItemQuantity, isInMaterialsNode, item->getItemLevel());
        item->itemDetails->setScale(0.5 * Director::getInstance()->getContentScaleFactor());
        inventoryBorder->addChild(item->itemDetails, 2000);
    }
    auto iPos = Vec2(currentNode->getBoundingNode().getMidX(), currentNode->getBoundingNode().getMidY());
    auto iSize = currentNode->getBoundingNode().size / 2;
    Vec2 itemPos = iPos + Vec2(item->itemDetails->background->getContentSize().width / 2 - iSize.width, -item->itemDetails->background->getContentSize().height / 2 + iSize.height);
    if (itemPos.x + item->itemDetails->background->getContentSize().width / 2 > buttonClose->getPosition().x) {
        itemPos.x = iPos.x - Vec2(item->itemDetails->background->getContentSize().width / 2 + iSize.width, -item->itemDetails->background->getContentSize().height / 2).x;
    }
    if (itemPos.y - item->itemDetails->background->getContentSize().height / 2 > buttonClose->getPosition().y) {
        itemPos.y = iPos.y - Vec2(item->itemDetails->background->getContentSize().width / 2, -item->itemDetails->background->getContentSize().height / 2).y;
    }
    if (itemPos.y + item->itemDetails->background->getContentSize().height / 2 < buttonClose->getPosition().y - item->itemDetails->background->getContentSize().height / 2) {
        itemPos.y = iPos.y + Vec2(-item->itemDetails->background->getContentSize().width / 2, item->itemDetails->background->getContentSize().height * 0.6 / 2).y;
    }
    item->itemDetails->setUpgradeInventory(this);
    item->itemDetails->setPosition(itemPos);
    item->itemDetails->show();
}
void UpgradeInventory::addEquipment(std::string equipmentName, int equipmentLv, int quantity) {
    if (equipmentName == "") return;
    /*for (auto node : inventoryNodes) {
        if (node->getStatus() == "free") {
            node->setBaseEquipment(equipmentName, equipmentLv);
            node->setQuantity(quantity);
            return;
        }
    }*/
   BaseEquipment* equipment = EquipmentFactory::createEquipment(equipmentName, equipmentLv);

    BaseEquipment::EquipmentData *temp = new BaseEquipment::EquipmentData();
    temp->name = equipmentName;
    temp->level = equipmentLv;
    temp->iconPath = equipment->getTexture()->getPath();
    equipmentsData.push_back(temp);
    onAddEquipment();
}

void UpgradeInventory::addItem(std::string itemName, int quantity) {
  /*  for (auto node : inventoryNodes) {
        if (node->getStatus() == "busy" && node->getBaseItem()) {
            if (node->getBaseItem()->getItemName() == itemName) {
                node->setQuantity(node->getQuantity() + quantity);
                return;
            }
        }
    }
    for (auto node : inventoryNodes) {
        if (node->getStatus() == "free") {
            node->setBaseItem(itemName);
            node->setQuantity(quantity);
            return;
        }
    }*/
    itemsAndQuantity.push_back(std::make_tuple(itemName, quantity));
}

void UpgradeInventory::dropItem(std::string itemName) {
    for (auto node : inventoryNodes) {
        if (node->getStatus() == "busy" && node->getBaseItem()) {
            if (node->getBaseItem()->getItemName() == itemName) {
                node->removeBaseItem();
                return;
            }
        }
    }
}

void UpgradeInventory::dropEquipment(std::string itemName) {
    for (auto node : inventoryNodes) {
        if (node->getStatus() == "busy" && node->getBaseEquipment()) {
            if (node->getBaseEquipment()->getEquipmentName() == itemName) {
                node->removeBaseEquipment();
                return;
            }
        }
    }
}

void UpgradeInventory::removeEquipment(std::string equipmentName) {
    for (auto node : inventoryNodes) {
        if (node->getStatus() == "busy") {
            if (node->getBaseEquipment()->getEquipmentName() == equipmentName) {
                node->removeBaseEquipment();
                return;
            }
        }
    }
}

void UpgradeInventory::removeItem(std::string itemName) 
{
    for (auto node : inventoryNodes) {
        if (node->getStatus() == "busy") {
            if (node->getBaseItem()->getItemName() == itemName) 
            {
                node->removeBaseItem();
                return;
            }
        }
    }
}

void UpgradeInventory::showUpgradeInventory() {
    game->getPlayer()->getInventory()->sort();
    this->setVisible(true);
    isSo = true;
}

void UpgradeInventory::hideUpgradeInventory() {
    this->setVisible(false);
    isSo = false;
}