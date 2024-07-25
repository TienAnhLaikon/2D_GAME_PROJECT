#include "ItemDetails.h"
#include "Inventory/Inventory.h"
#include "Player/Player.h"
#include "Item/ItemFactory.h"
#include "Inventory/Inventory.h"
#include "Inventory/UpgradeInventory.h"
#include "Inventory/SubInventory.h"
#include "Game/Game.h"
#include "NotificationManager/NotificationManager.h"
#include "Map/Map.h"
//#include "Item/BaseItem.h"

// Constructor implementation
ItemDetails::ItemDetails(const std::string& iconPath, const std::string& itemName, const std::string& description, const BaseItem::Type itemType, const std::string stats, const bool isInUpgrade, const int itemQuantity, bool isInMaterialNode, int itemTypeId, bool isInInventory) : _iconPath(iconPath), _itemName(itemName), _description(description), _itemType(itemType), _stats(stats), _isInUpgrade(isInUpgrade), _itemQuantity(itemQuantity), _isInMaterialNode(isInMaterialNode), _itemTypeId(itemTypeId), _isInInventory(isInInventory) {

}

// Destructor implementation
ItemDetails::~ItemDetails() {

}

// Static create method implementation
ItemDetails* ItemDetails::create(const std::string& iconPath, const std::string& itemName, const std::string& description, const BaseItem::Type itemType, const std::string stats, const bool isInUpgrade, const int itemQuantity, bool isInMaterialNode, int itemTypeId, bool isInInventory) {
    ItemDetails* customClass = new (std::nothrow) ItemDetails(iconPath, itemName, description, itemType, stats, isInUpgrade, itemQuantity, isInMaterialNode, itemTypeId, isInInventory);
    if (customClass && customClass->init()) {
        customClass->autorelease();
        return customClass;
    }
    else {
        delete customClass;
        return nullptr;
    }
}

bool ItemDetails::init() {
    if (!Node::init()) return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    background = Sprite::create("res/c_container.png");
    background->retain();
    this->addChild(background, 1);

    Sprite* iconBorder = Sprite::create("res/talent_slot.png");
    iconBorder->setPosition(-(background->getContentSize().width / 2 - iconBorder->getContentSize().width / 2), background->getContentSize().height / 2 - iconBorder->getContentSize().height / 2);
    iconBorder->setScale(0.5 * Director::getInstance()->getContentScaleFactor());
    this->addChild(iconBorder, 2);

    if (_itemTypeId == 1) {
        iconBorder->setTexture("res/talent_slot_enhanced.png");
    }
    else if (_itemTypeId == 2) {
        iconBorder->setTexture("res/talent_slot_rare.png");
    }
    else if (_itemTypeId == 3) {
        iconBorder->setTexture("res/talent_slot_legendary.png");
    }

    Sprite* icon = Sprite::create(_iconPath);
    icon->setPosition(iconBorder->getPosition() - icon->getContentSize() / 2);
    icon->setAnchorPoint(Vec2(0, 0));
    this->addChild(icon, 2);

    Label* itemName = Label::createWithTTF(_itemName, "fonts/Diablo Light.ttf", 10);
    itemName->setPosition(20, background->getContentSize().height / 2 - itemName->getContentSize().height * 2);
    this->addChild(itemName, 2);

    Label* stats = Label::createWithTTF(_stats, "fonts/Diablo Light.ttf", 8);
    stats->setPosition(iconBorder->getPosition().x + stats->getContentSize().width / 4, icon->getContentSize().height / 2);
    this->addChild(stats, 2);

    Label* description = Label::createWithTTF(_description, "fonts/Diablo Light.ttf", 7);
    description->setPosition(background->getPosition() + Vec2(0, -12 * Director::getInstance()->getContentScaleFactor()));
    this->addChild(description, 2);

    Label* equip = nullptr;
    if (_itemType == BaseItem::Type::POTION_ITEM) {
        addButton = Sprite::create("res/button_gray.png");
        addButton->setScale(0.28 * Director::getInstance()->getContentScaleFactor());
        equip = Label::createWithTTF("Equip", "fonts/Diablo Light.ttf", 10);
        addButton->setPosition(background->getPosition() + background->getContentSize() / 2 + Vec2(addButton->getContentSize().width * 0.28 * Director::getInstance()->getContentScaleFactor() / 2, -background->getContentSize().height / 2 - addButton->getContentSize().height * 0.28 * Director::getInstance()->getContentScaleFactor() + 30));
        this->addChild(addButton);
        equip->setPosition(addButton->getPosition());
        this->addChild(equip);
    }
    if (_isInUpgrade == false && _isInInventory == false) {
        dropButton = Sprite::create("res/button_gray.png");
        dropButton->setScale(0.28 * Director::getInstance()->getContentScaleFactor());
        Label* drop = Label::createWithTTF("Drop", "fonts/Diablo Light.ttf", 10);
        dropButton->setPosition(background->getPosition() + background->getContentSize() / 2 + Vec2(dropButton->getContentSize().width * 0.28 * Director::getInstance()->getContentScaleFactor() / 2, -background->getContentSize().height / 2 - dropButton->getContentSize().height * 0.28 * Director::getInstance()->getContentScaleFactor() * 2 + 40));
        this->addChild(dropButton);
        drop->setPosition(dropButton->getPosition());
        this->addChild(drop);
    }
    else {
        if (_isInInventory == false) {
            addButton = Sprite::create("res/button_gray.png");
            addButton->setScale(0.28 * Director::getInstance()->getContentScaleFactor());
            equip = Label::createWithTTF("Add", "fonts/Diablo Light.ttf", 10);
            addButton->setPosition(background->getPosition() + background->getContentSize() / 2 + Vec2(addButton->getContentSize().width * 0.28 * Director::getInstance()->getContentScaleFactor() / 2, -background->getContentSize().height / 2 - addButton->getContentSize().height * 0.28 * Director::getInstance()->getContentScaleFactor()));
            this->addChild(addButton);
            equip->setPosition(addButton->getPosition());
            this->addChild(equip);
            if (_isInMaterialNode) {
                equip->setString("Remove");
            }
        } 
    }
    if (_isInInventory == true) {
        if(dropButton) dropButton->setVisible(false);
        if (equip) equip->setString("Unequip");

    }

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [&](Touch* touch, Event* event) {
        if (isVisible()) {
            auto startPoint = this->convertToNodeSpace(touch->getLocation());
            if (dropButton && dropButton->getBoundingBox().containsPoint(startPoint)) {
              //  int itemQuantity = 0;
                if (_inventory) {
                   // itemQuantity = std::get<1>(_inventory->player->getItem(_itemName));
                    _inventory->player->dropItem(_itemName);
                    NotificationManager::getInstance()->showMessageNotification("Drop item successfully!", Vec2::ZERO, Color3B(161, 251, 142), 17);
                }
   /*             Scene* currentScene = Director::getInstance()->getRunningScene();
                Game* game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
                if (game) {
                    int ran = Engine::getInstance()->randomNumber(30, 60);
                    int ran2 = Engine::getInstance()->randomNumber(30, 60);
                    Vec2 dropPos = Vec2(ran, ran2);

                    for (int i = 0; i < itemQuantity; i++) {
                        if (game && game->getPlayer() && game->gameMap && game->gameMap->getTiledMap()) {
                            auto droppedItem = ItemFactory::createItem(_itemName);
                            droppedItem->showLootIndicator();
                            droppedItem->setPosition(game->getPlayer()->getPosition() + dropPos);
                            game->listOfItems.push_back(droppedItem);
                            game->gameMap->getTiledMap()->addChild(droppedItem);
                            if (_inventory->currentNode && _inventory->currentNode->getBaseItem())
                                _inventory->currentNode->removeBaseItem();
                        }
                    }

                }*/
                ItemDetails::hide();
                return true;
            }
            else if (_itemType == BaseItem::Type::POTION_ITEM && addButton != nullptr) {
                if (addButton->isVisible() && addButton->getBoundingBox().containsPoint(startPoint)) {
                    if (_isInInventory) {
                       // _inventory->player->equipItem(_itemName, _itemQuantity);
                        _inventory->removeItemFromItemNodes(_itemName);
                        _inventory->addItem(_itemName, _itemQuantity);
                        NotificationManager::getInstance()->showMessageNotification("Unequip item successfully!", Vec2::ZERO, Color3B(161, 251, 142), 17);
                    }
                    else {
                        _inventory->player->equipItem(_itemName, _itemQuantity);
                        _inventory->addItemToItemNodes(_itemName, _itemQuantity);
                        NotificationManager::getInstance()->showMessageNotification("Equip item successfully!", Vec2::ZERO, Color3B(161, 251, 142), 17);
                    }
                }
                ItemDetails::hide();
                return true;
            }
            else if (_itemType == BaseItem::Type::UPGRADE_ITEM && addButton != nullptr && _isInMaterialNode == false) {
                bool isSuccess = false;
                for (auto i : _upgradeInventory->materialNodes) {
                    if (i->getBaseItemToUpgrade() && i->getBaseItemToUpgrade()->isVisible() && i->getBaseItemToUpgrade()->getItemName() == _itemName) {
                        i->setBaseItem(_itemName);
                        i->setQuantity(_itemQuantity);
                        for (auto j : _upgradeInventory->inventoryNodes) {
                            if (j->getBaseItem() && j->getBaseItem()->getItemName() == _itemName) {
                                j->removeBaseItem();
                                NotificationManager::getInstance()->showMessageNotification("Equip item successfully!", Vec2::ZERO, Color3B(161, 251, 142), 17);
                                isSuccess = true;
                                ItemDetails::hide();
                                return true;
                            }
                        }
                    }
                }
                if (isSuccess == false) {
                    NotificationManager::getInstance()->showMessageNotification("This item isn't in materials to upgrade!", Vec2::ZERO, Color3B(251, 67, 53), 17);
                    ItemDetails::hide();
                    return true;
                }
                ItemDetails::hide();
                return true;
            }
            else if (_itemType == BaseItem::Type::UPGRADE_ITEM && _isInMaterialNode == true) {
                for (auto i : _upgradeInventory->materialNodes) {
                    if (i->getBaseItem()->getItemName() == _itemName) {
                        i->removeBaseItem();
                        NotificationManager::getInstance()->showMessageNotification("Remove Item Success!", Vec2::ZERO, Color3B(161, 251, 142), 17);
                        ItemDetails::hide();
                        _upgradeInventory->addItem(_itemName, _itemQuantity);
                        return true;
                    }
                }
                ItemDetails::hide();
                return true;
            }
            else {
                ItemDetails::hide();
                return true;
            }
        }
        return false;
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    return true;
}

void ItemDetails::hide() {
    _inventory->overlay->setVisible(false);
    this->setVisible(false);
}

void ItemDetails::show() {
    this->setVisible(true);
}

void ItemDetails::destroy() {
    this->removeFromParentAndCleanup(true);
}