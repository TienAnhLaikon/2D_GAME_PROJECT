#include "SubInventory.h"
#include "../Player/Player.h"
#include "Game/Game.h"
#include <unordered_set>
#include "./UserInterface/InGame/EquipmentDetails.h"
#include "./UserInterface/InGame/ItemDetails.h"
#include "./Inventory.h"
#include "Item/ItemFactory.h"
#include "Item/PotionManager.h"
SubInventory* SubInventory::createSubInventory()
{
    auto inventory = new (std::nothrow) SubInventory();
    if (inventory && inventory->init())
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

bool SubInventory::init()
{
    if (!Node::init())
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();

    auto bottomNavigation = Sprite::create("res/ab2_bonus_frame.png");
    this->addChild(bottomNavigation, 1);
    bottomInventory = Sprite::create("res/ab2_main_frame.png");
    bottomInventory->setVisible(false);
    bottomInventory->setPosition(Vec2(0, -bottomNavigation->getContentSize().height + (bottomNavigation->getContentSize().height / 8)));
    bottomInventory->setScale(1.35, 1.3);
    this->addChild(bottomInventory, 1);
    this->setContentSize(bottomInventory->getContentSize() * 1.35);

    float squareSize = (bottomInventory->getContentSize().width * 1.3) / 8;

    auto bottomNavigation2 = Sprite::create("res/ab2_main_frame.png");
    bottomNavigation2->setPositionY(-(squareSize * 1.8));
    this->addChild(bottomNavigation2, 1);

    Scene* game = Director::getInstance()->getRunningScene();
    currentScene = dynamic_cast<Game*>(game->getChildByName("GameInstance"));

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [&](Touch* touch, Event* event) {
        auto startPoint = this->convertToNodeSpace(touch->getLocation());
        if (bottomInventory->getBoundingBox().containsPoint(startPoint)) {
            std::string itemName = "";
            bool checkItemValue = true;
            for (auto i : inventoryNodes) {
                if (i->getBoundingNode().containsPoint(startPoint) && i->getBaseItem()) {
                    currentNode = i;
                    itemName = i->getBaseItem()->getItemName();
                    if (itemName == "Mana Potion") {
                        if (currentScene->inGameUI->potionManager->addPotion("sprites/potion/mana-potion.png", 8)) {
                            ItemFactory::createItem(itemName)->use();
                            if (_inventory && _inventory->itemNode1 && _inventory->itemNode1->getBaseItem()) {
                                if (_inventory->itemNode1->getBaseItem() && _inventory->itemNode1->getBaseItem()->getItemName() == "Mana Potion") {
                                    if (_inventory->itemNode1->getQuantity() > 0)
                                        _inventory->itemNode1->decreaseBaseItem(1);
                                    else _inventory->itemNode1->removeBaseItem();
                                }
                            }

                            if (_inventory && _inventory->itemNode2 && _inventory->itemNode2->getBaseItem()) {
                                if (_inventory->itemNode2->getBaseItem() && _inventory->itemNode2->getBaseItem()->getItemName() == "Mana Potion") {
                                    if (_inventory->itemNode2->getQuantity() > 0)
                                        _inventory->itemNode2->decreaseBaseItem(1);
                                    else _inventory->itemNode2->removeBaseItem();
                                }
                            }

                            if (_inventory && _inventory->itemNode3 && _inventory->itemNode3->getBaseItem()) {
                                if (_inventory->itemNode3->getBaseItem() && _inventory->itemNode3->getBaseItem()->getItemName() == "Mana Potion") {
                                    if (_inventory->itemNode3->getQuantity() > 0)
                                        _inventory->itemNode3->decreaseBaseItem(1);
                                    else _inventory->itemNode3->removeBaseItem();
                                }
                            }

                            if (_inventory && _inventory->itemNode4 && _inventory->itemNode4->getBaseItem()) {
                                if (_inventory->itemNode4->getBaseItem() && _inventory->itemNode4->getBaseItem()->getItemName() == "Mana Potion") {
                                    if (_inventory->itemNode4->getQuantity() > 0)
                                        _inventory->itemNode4->decreaseBaseItem(1);
                                    else _inventory->itemNode4->removeBaseItem();
                                }
                            }
                        }
                        else {
                            NotificationManager::getInstance()->showMessageNotification("Now, you can't using this potion", Vec2::ZERO, Color3B::RED, 20);
                            checkItemValue = false;
                        }
                    }
                    else if (itemName == "Health Potion") {
                        if (_inventory && currentScene->inGameUI->potionManager->addPotion("sprites/potion/health-potion.png", 8)) {
                            ItemFactory::createItem(itemName)->use();
                            if (_inventory->itemNode1 && _inventory->itemNode1->getBaseItem()) {
                                if (_inventory->itemNode1->getBaseItem() && _inventory->itemNode1->getBaseItem()->getItemName() == "Health Potion") {
                                    if (_inventory->itemNode1->getQuantity() > 0)
                                        _inventory->itemNode1->decreaseBaseItem(1);
                                    else _inventory->itemNode1->removeBaseItem();
                                }
                            }

                            if (_inventory->itemNode2 && _inventory->itemNode2->getBaseItem()) {
                                if (_inventory->itemNode2->getBaseItem() && _inventory->itemNode2->getBaseItem()->getItemName() == "Health Potion") {
                                    if (_inventory->itemNode2->getQuantity() > 0)
                                        _inventory->itemNode2->decreaseBaseItem(1);
                                    else _inventory->itemNode2->removeBaseItem();
                                }
                            }

                            if (_inventory->itemNode3 && _inventory->itemNode3->getBaseItem()) {
                                if (_inventory->itemNode3->getBaseItem() && _inventory->itemNode3->getBaseItem()->getItemName() == "Health Potion") {
                                    if (_inventory->itemNode3->getQuantity() > 0)
                                        _inventory->itemNode3->decreaseBaseItem(1);
                                    else _inventory->itemNode3->removeBaseItem();
                                }
                            }

                            if (_inventory->itemNode4 && _inventory->itemNode4->getBaseItem()) {
                                if (_inventory->itemNode4->getBaseItem() && _inventory->itemNode4->getBaseItem()->getItemName() == "Health Potion") {
                                    if (_inventory->itemNode4->getQuantity() > 0)
                                        _inventory->itemNode4->decreaseBaseItem(1);
                                    else _inventory->itemNode4->removeBaseItem();
                                }
                            }
                        }
                        else {
                            NotificationManager::getInstance()->showMessageNotification("Now, you can't using this potion", Vec2::ZERO, Color3B::RED, 20);
                            checkItemValue = false;
                        }
                    }
                    else if (itemName == "Speed Potion") {
                        if (currentScene->inGameUI->potionManager->addPotion("sprites/potion/speed-potion.png", 8)) {
                            ItemFactory::createItem(itemName)->use();
                            if (_inventory->itemNode1 && _inventory->itemNode1->getBaseItem()) {
                                if (_inventory->itemNode1->getBaseItem() && _inventory->itemNode1->getBaseItem()->getItemName() == "Speed Potion") {
                                    if (_inventory->itemNode1->getQuantity() > 0)
                                        _inventory->itemNode1->decreaseBaseItem(1);
                                    else _inventory->itemNode1->removeBaseItem();
                                }
                            }

                            if (_inventory->itemNode2 && _inventory->itemNode2->getBaseItem()) {
                                if (_inventory->itemNode2->getBaseItem() && _inventory->itemNode2->getBaseItem()->getItemName() == "Speed Potion") {
                                    if (_inventory->itemNode2->getQuantity() > 0)
                                        _inventory->itemNode2->decreaseBaseItem(1);
                                    else _inventory->itemNode2->removeBaseItem();
                                }
                            }

                            if (_inventory->itemNode3 && _inventory->itemNode3->getBaseItem()) {
                                if (_inventory->itemNode3->getBaseItem() && _inventory->itemNode3->getBaseItem()->getItemName() == "Speed Potion") {
                                    if (_inventory->itemNode3->getQuantity() > 0)
                                        _inventory->itemNode3->decreaseBaseItem(1);
                                    else _inventory->itemNode3->removeBaseItem();
                                }
                            }

                            if (_inventory->itemNode4 && _inventory->itemNode4->getBaseItem()) {
                                if (_inventory->itemNode4->getBaseItem() && _inventory->itemNode4->getBaseItem()->getItemName() == "Speed Potion") {
                                    if (_inventory->itemNode4->getQuantity() > 0)
                                        _inventory->itemNode4->decreaseBaseItem(1);
                                    else _inventory->itemNode4->removeBaseItem();
                                }
                            }
                        }
                    }
                    else if (itemName == "Scroll Paper") {
                        if (UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0) > 10) {
                            if (currentScene->inGameUI->potionManager->addPotion("sprites/potion/scroll.png", 8)) {
                                ItemFactory::createItem(itemName)->use();
                                if (_inventory->itemNode1 && _inventory->itemNode1->getBaseItem()) {
                                    if (_inventory->itemNode1->getBaseItem() && _inventory->itemNode1->getBaseItem()->getItemName() == "Scroll Paper") {
                                        if (_inventory->itemNode1->getQuantity() > 0)
                                            _inventory->itemNode1->decreaseBaseItem(1);
                                        else _inventory->itemNode1->removeBaseItem();
                                    }
                                }

                                if (_inventory->itemNode2 && _inventory->itemNode2->getBaseItem()) {
                                    if (_inventory->itemNode2->getBaseItem() && _inventory->itemNode2->getBaseItem()->getItemName() == "Scroll Paper") {
                                        if (_inventory->itemNode2->getQuantity() > 0)
                                            _inventory->itemNode2->decreaseBaseItem(1);
                                        else _inventory->itemNode2->removeBaseItem();
                                    }
                                }

                                if (_inventory->itemNode3 && _inventory->itemNode3->getBaseItem()) {
                                    if (_inventory->itemNode3->getBaseItem() && _inventory->itemNode3->getBaseItem()->getItemName() == "Scroll Paper") {
                                        if (_inventory->itemNode3->getQuantity() > 0)
                                            _inventory->itemNode3->decreaseBaseItem(1);
                                        else _inventory->itemNode3->removeBaseItem();
                                    }
                                }

                                if (_inventory->itemNode4 && _inventory->itemNode4->getBaseItem()) {
                                    if (_inventory->itemNode4->getBaseItem() && _inventory->itemNode4->getBaseItem()->getItemName() == "Scroll Paper") {
                                        if (_inventory->itemNode4->getQuantity() > 0)
                                            _inventory->itemNode4->decreaseBaseItem(1);
                                        else _inventory->itemNode4->removeBaseItem();
                                    }
                                }
                            }
                        }
                        else {
                            NotificationManager::getInstance()->showMessageNotification("You Need Complete Tutorial Before Use This!", Vec2::ZERO, Color3B::RED, 16);
                            return true;
                        }
                    }
                }
                }
                if (checkItemValue && _inventory && itemName != "") {
                    for (auto i : _inventory->inventoryNodes) {
                        if (i->getBaseItem() && i->getBaseItem()->getItemName() == itemName) {
                            i->decreaseBaseItem(1);
                            break;
                        }
                    }
                }
                if (checkItemValue && !this->inventoryNodes.empty() && itemName != "") {
                    for (auto i : this->inventoryNodes) {
                        if (i->getBaseItem() && i->getBaseItem()->getItemName() == itemName) {
                            i->decreaseBaseItem(1);
                            break;
                        }
                    }
                    return true;
                }
            }
        return false;
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    const float gapX = squareSize / 30;
    const float gapY = squareSize / 30;

    auto t = -Vec2(bottomInventory->getContentSize().width * 1.3 / 4, 0);
    for (int row = 0; row < 4; ++row)
    {
        auto node = InventoryNode::createInventoryNode(squareSize);
        node->setSubInven(true);
        node->setPosition(t + Vec2(row * (squareSize + gapX), -bottomNavigation->getContentSize().height + (bottomNavigation->getContentSize().height / 8)) - Vec2(gapX * 5, squareSize / 2 + gapY * 2));
        this->addChild(node, 20);
        inventoryNodes.push_back(node);
    }

    return true;
}


std::vector<std::pair<std::string, int>> SubInventory::getSubInventoryItems() {
    std::vector<std::pair<std::string, int>> result;
    for (auto node : inventoryNodes) {
        if (node->getStatus() == "busy" && node->getBaseItem()) {
            result.push_back(std::make_pair(node->getBaseItem()->getItemName(), node->getQuantity()));
        }
    }
    return result;
}


void SubInventory::removeItem(std::string itemName) {
    for (auto node : inventoryNodes) {
        if (node->getBaseItem() && node->getBaseItem()->getItemName() == itemName) {
            node->removeBaseItem();
        }
    }
}

void SubInventory::addItem(std::string itemName, int quantity) {
        int count = 0;
        for (auto node : inventoryNodes) {
            if (node->getStatus() == "busy" && node->getBaseItem()) {
                if (node->getBaseItem()->getItemName() == itemName) {
                    node->setQuantity(node->getQuantity() + quantity);
                    return;
                }
                else if(count == 7) {
                    node->removeBaseItem();
                    node->setBaseItem(itemName);
                    node->setQuantity(quantity);
                }
            }
            count++;
        }
        for (auto node : inventoryNodes) {
            if (node->getStatus() == "free") {
                node->setBaseItem(itemName);
                node->setQuantity(quantity);
                return;
            }
        }
}

void SubInventory::addItem(std::string itemName, int quantity, int id) {
    int count = 0;
    int i = id - 1;
    if (inventoryNodes[i]->getBaseItem()) {
        inventoryNodes[i]->setQuantity(inventoryNodes[i]->getQuantity() + quantity);
        return;
    }
    else {
        inventoryNodes[i]->removeBaseItem();
        inventoryNodes[i]->setBaseItem(itemName);
        inventoryNodes[i]->setQuantity(quantity);
        return;
    }

   /* for (int i = 0; i < inventoryNodes.size(); i++) {
        if (inventoryNodes[i]->getBaseItem() && i == (id - 1)) {
            if (inventoryNodes[i]->getBaseItem()->getItemName() == itemName) {
                inventoryNodes[i]->setQuantity(inventoryNodes[i]->getQuantity() + quantity);

                return;
            }
            else if(count == 4 && i == (id - 1)) {
                inventoryNodes[i]->removeBaseItem();
                inventoryNodes[i]->setBaseItem(itemName);
                inventoryNodes[i]->setQuantity(quantity);
            }
        }
        count++;
    }*/
    for (auto node : inventoryNodes) {
        if (node->getStatus() == "free") {
            node->setBaseItem(itemName);
            node->setQuantity(quantity);
            return;
        }
    }
}