#include "Inventory.h"
#include "../Player/Player.h"
#include "./UserInterface/InGame/EquipmentDetails.h"
#include "./UserInterface/InGame/ItemDetails.h"
#include "./UserInterface/InGame/Scrollview.h"
#include "SubInventory.h"
#include "TutorialManager/TutorialManager.h"
#include "Game/Game.h"
bool Inventory::isSorted = false;
bool Inventory::isSorting = false;
Inventory* Inventory::createInventory(Player* _player)
{
    auto inventory = new (std::nothrow) Inventory();
    if (inventory && inventory->init(_player))
    {
        inventory->autorelease();
        inventory->player = _player;
        return inventory;
    }
    else
    {
        delete inventory;
        return nullptr;
    }
}

bool Inventory::init(Player* _player)
{
    if (!Node::init())
    {
        return false;
    }
    currentTutorial = TutorialManager::getInstance()->currentTutorial;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    inventoryBorder = Node::create();
    this->addChild(inventoryBorder);
    auto layer = LayerColor::create(Color4B(34, 28, 25, 255));
    layer->setContentSize(visibleSize);
    layer->setPosition(Vec2(-visibleSize.width / 2, -visibleSize.height / 2));
    inventoryBorder->addChild(layer, 1);

    overlay = LayerColor::create(Color4B(0, 0, 0, 185));
    overlay->setContentSize(visibleSize);
    overlay->setPosition(Vec2(-visibleSize.width / 2, -visibleSize.height / 2));
    overlay->setVisible(false);
    inventoryBorder->addChild(overlay, 100);

    auto touchListener2 = EventListenerTouchOneByOne::create();
    touchListener2->setSwallowTouches(true);
    touchListener2->onTouchBegan = [&](Touch* touch, Event* event) {
        auto startPoint = this->convertToNodeSpace(touch->getLocation());
        if (overlay->isVisible() == false && sortButton->getBoundingBox().containsPoint(startPoint) && TutorialManager::getInstance()->currentTutorial > 3)
        {
            UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
            Inventory::sort();
        }
        if (overlay->isVisible() == false && prevInvenButton && prevInvenButton->getBoundingBox().containsPoint(startPoint))
        {
            UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
            Inventory::prevInventoryPage();
        }
        else if (overlay->isVisible() == false && nextInvenButton && nextInvenButton->getBoundingBox().containsPoint(startPoint))
        {
            UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
            Inventory::nextInventoryPage();
        }
        if (overlay->isVisible() == false && buttonClose->getBoundingBox().containsPoint(startPoint))
        {
            UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));

            Scene* currentScene = Director::getInstance()->getRunningScene();
            Game* game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
            game->resumeGame();
            //Director::getInstance()->resume();
            //Director::getInstance()->getRunningScene()->getPhysicsWorld()->setSpeed(1.0f);
            if (TutorialManager::getInstance()->currentTutorial == 3) {
                TutorialManager::getInstance()->prevTutorial();
            }
            hideInventory();
            return true;
        }
        return false;
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener2, layer);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [&](Touch* touch, Event* event)
        {
            if (overlay->isVisible() == false && isVisible()) {
                if (isDraggingItem == false && isSo)
                {
                    auto startPoint = this->convertToNodeSpace(touch->getLocation());
                    if (weaponNode->getBoundingNode().containsPoint(startPoint) && weaponNode->getStatus() == "busy") {
                        equipment = EquipmentFactory::createEquipment(weaponNode->getBaseEquipment()->getEquipmentName(), weaponNode->getBaseEquipment()->getLevel());
                        equipment->setScale(1.2 * Director::getInstance()->getContentScaleFactor());
                        equipment->setPosition(startPoint + Vec2(-15.6 * Director::getInstance()->getContentScaleFactor(), 15.6 * Director::getInstance()->getContentScaleFactor()));
                        player->removeEquipment("Weapon");
                        inventoryBorder->addChild(equipment, 2500);
                        weaponNode->removeBaseEquipment();
                        currentNode = weaponNode;
                        isDraggingItem = true;
                        isPlayerUsing = true;
                        weaponNode->getNodeSprite()->setTexture("res/inventory-node.png");
                    }
                    else if (ringNode->getBoundingNode().containsPoint(startPoint) && ringNode->getStatus() == "busy") {
                        equipment = EquipmentFactory::createEquipment(ringNode->getBaseEquipment()->getEquipmentName(), ringNode->getBaseEquipment()->getLevel());
                        equipment->setScale(1.2 * Director::getInstance()->getContentScaleFactor());
                        equipment->setPosition(startPoint + Vec2(-15.6 * Director::getInstance()->getContentScaleFactor(), 15.6 * Director::getInstance()->getContentScaleFactor()));
                        player->removeEquipment("Ring");
                        inventoryBorder->addChild(equipment, 2500);
                        ringNode->removeBaseEquipment();
                        currentNode = ringNode;
                        isDraggingItem = true;
                        isPlayerUsing = true;
                        ringNode->getNodeSprite()->setTexture("res/inventory-node.png");
                    }
                    else if (helmetNode->getBoundingNode().containsPoint(startPoint) && helmetNode->getStatus() == "busy") {
                        equipment = EquipmentFactory::createEquipment(helmetNode->getBaseEquipment()->getEquipmentName(), helmetNode->getBaseEquipment()->getLevel());
                        equipment->setScale(1.2 * Director::getInstance()->getContentScaleFactor());
                        equipment->setPosition(startPoint + Vec2(-15.6 * Director::getInstance()->getContentScaleFactor(), 15.6 * Director::getInstance()->getContentScaleFactor()));
                        player->removeEquipment("Helmet");
                        inventoryBorder->addChild(equipment, 2500);
                        helmetNode->removeBaseEquipment();
                        currentNode = helmetNode;
                        isDraggingItem = true;
                        isPlayerUsing = true;
                        helmetNode->getNodeSprite()->setTexture("res/inventory-node.png");
                    }
                    else if (shieldNode->getBoundingNode().containsPoint(startPoint) && shieldNode->getStatus() == "busy") {
                        equipment = EquipmentFactory::createEquipment(shieldNode->getBaseEquipment()->getEquipmentName(), shieldNode->getBaseEquipment()->getLevel());
                        equipment->setScale(1.2 * Director::getInstance()->getContentScaleFactor());
                        equipment->setPosition(startPoint + Vec2(-15.6 * Director::getInstance()->getContentScaleFactor(), 15.6 * Director::getInstance()->getContentScaleFactor()));
                        player->removeEquipment("Shield");
                        inventoryBorder->addChild(equipment, 2500);
                        shieldNode->removeBaseEquipment();
                        currentNode = shieldNode;
                        isDraggingItem = true;
                        isPlayerUsing = true;
                        shieldNode->getNodeSprite()->setTexture("res/inventory-node.png");
                    }
                    else if (shoeNode->getBoundingNode().containsPoint(startPoint) && shoeNode->getStatus() == "busy") {
                        equipment = EquipmentFactory::createEquipment(shoeNode->getBaseEquipment()->getEquipmentName(), shoeNode->getBaseEquipment()->getLevel());
                        equipment->setScale(1.2 * Director::getInstance()->getContentScaleFactor());
                        equipment->setPosition(startPoint + Vec2(-15.6 * Director::getInstance()->getContentScaleFactor(), 15.6 * Director::getInstance()->getContentScaleFactor()));
                        player->removeEquipment("Shoe");
                        inventoryBorder->addChild(equipment, 2500);
                        shoeNode->removeBaseEquipment();
                        currentNode = shoeNode;
                        isDraggingItem = true;
                        isPlayerUsing = true;
                        shoeNode->getNodeSprite()->setTexture("res/inventory-node.png");
                    }
                    else if (armorNode->getBoundingNode().containsPoint(startPoint) && armorNode->getStatus() == "busy") {
                        equipment = EquipmentFactory::createEquipment(armorNode->getBaseEquipment()->getEquipmentName(), armorNode->getBaseEquipment()->getLevel());
                        equipment->setScale(1.2 * Director::getInstance()->getContentScaleFactor());
                        equipment->setPosition(startPoint + Vec2(-15.6 * Director::getInstance()->getContentScaleFactor(), 15.6 * Director::getInstance()->getContentScaleFactor()));
                        player->removeEquipment("Armor");
                        inventoryBorder->addChild(equipment, 2500);
                        armorNode->removeBaseEquipment();
                        currentNode = armorNode;
                        isDraggingItem = true;
                        isPlayerUsing = true;
                        armorNode->getNodeSprite()->setTexture("res/inventory-node.png");
                    }
                    else if (itemNode1->getBoundingNode(0.6).containsPoint(startPoint) && itemNode1->getStatus() == "busy" && itemNode1->getBaseItem()->getItemType() == BaseItem::POTION_ITEM) {
                        item = ItemFactory::createItem(itemNode1->getBaseItem()->getItemName());
                        item->setScale(1.2 * Director::getInstance()->getContentScaleFactor());
                        item->setPosition(startPoint + Vec2(-15.6 * Director::getInstance()->getContentScaleFactor(), 15.6 * Director::getInstance()->getContentScaleFactor()));
                        inventoryBorder->addChild(item, 2500);
                        movingItemQuantity = itemNode1->getQuantity();
                        itemNode1->removeBaseItem();
                        currentNode = itemNode1;
                        isDraggingItem = true;
                        InGameUI::getInstance(player)->subInventory->removeItem(item->getItemName());
                        UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                    }
                    else if (itemNode2->getBoundingNode(0.6).containsPoint(startPoint) && itemNode2->getStatus() == "busy" && itemNode2->getBaseItem()->getItemType() == BaseItem::POTION_ITEM) {
                        item = ItemFactory::createItem(itemNode2->getBaseItem()->getItemName());
                        item->setScale(1.2 * Director::getInstance()->getContentScaleFactor());
                        item->setPosition(startPoint + Vec2(-15.6 * Director::getInstance()->getContentScaleFactor(), 15.6 * Director::getInstance()->getContentScaleFactor()));
                        inventoryBorder->addChild(item, 2500);
                        movingItemQuantity = itemNode2->getQuantity();
                        itemNode2->removeBaseItem();
                        currentNode = itemNode2;
                        isDraggingItem = true;
                        InGameUI::getInstance(player)->subInventory->removeItem(item->getItemName());
                        UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                    }
                    else if (itemNode3->getBoundingNode(0.6).containsPoint(startPoint) && itemNode3->getStatus() == "busy" && itemNode3->getBaseItem()->getItemType() == BaseItem::POTION_ITEM) {
                        item = ItemFactory::createItem(itemNode3->getBaseItem()->getItemName());
                        item->setScale(1.2 * Director::getInstance()->getContentScaleFactor());
                        item->setPosition(startPoint + Vec2(-15.6 * Director::getInstance()->getContentScaleFactor(), 15.6 * Director::getInstance()->getContentScaleFactor()));
                        inventoryBorder->addChild(item, 2500);
                        movingItemQuantity = itemNode3->getQuantity();
                        itemNode3->removeBaseItem();
                        currentNode = itemNode3;
                        isDraggingItem = true;
                        InGameUI::getInstance(player)->subInventory->removeItem(item->getItemName());
                        UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                    }
                    else if (itemNode4->getBoundingNode(0.6).containsPoint(startPoint) && itemNode4->getStatus() == "busy" && itemNode4->getBaseItem()->getItemType() == BaseItem::POTION_ITEM) {
                        item = ItemFactory::createItem(itemNode4->getBaseItem()->getItemName());
                        item->setScale(1.2 * Director::getInstance()->getContentScaleFactor());
                        item->setPosition(startPoint + Vec2(-15.6 * Director::getInstance()->getContentScaleFactor(), 15.6 * Director::getInstance()->getContentScaleFactor()));
                        inventoryBorder->addChild(item, 2500);
                        movingItemQuantity = itemNode4->getQuantity();
                        itemNode4->removeBaseItem();
                        currentNode = itemNode4;
                        isDraggingItem = true;
                        InGameUI::getInstance(player)->subInventory->removeItem(item->getItemName());
                        UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                    }
                    else {
                        for (auto& i : inventoryNodes)
                        {
                            if (i->isVisible() && i->getBoundingNode().containsPoint(startPoint))
                            {
                                if (TutorialManager::getInstance()->currentTutorial == 3 && i != inventoryNodes[0]) break;
                                if (i->getBaseEquipment())
                                {
                                    equipment = EquipmentFactory::createEquipment(i->getBaseEquipment()->getEquipmentName(), i->getBaseEquipment()->getLevel());
                                    equipment->setPosition(startPoint);
                                    if (equipment->getTypeName() == "Weapon") {
                                        if (weaponNode->getBaseEquipment() && weaponNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::STANDARD) {
                                            weaponNode->getNodeSprite()->setTexture("res/inventory-node-black-press.png");
                                        }
                                        else if (weaponNode->getBaseEquipment() && weaponNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::ENHANCED) {
                                            weaponNode->getNodeSprite()->setTexture("res/inventory-node-green-press.png");
                                        }
                                        else if (weaponNode->getBaseEquipment() && weaponNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::RARE) {
                                            weaponNode->getNodeSprite()->setTexture("res/inventory-node-purple-press.png");
                                        }
                                        else if (weaponNode->getBaseEquipment() && weaponNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::LEGENDARY) {
                                            weaponNode->getNodeSprite()->setTexture("res/inventory-node-orange-press.png");
                                        }
                                        else {
                                            weaponNode->getNodeSprite()->setTexture("res/inventory-node-press.png");
                                        }
                                    }
                                    else if (equipment->getTypeName() == "Armor") {
                                        if (armorNode->getBaseEquipment() && armorNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::STANDARD) {
                                            armorNode->getNodeSprite()->setTexture("res/inventory-node-black-press.png");
                                        }
                                        else if (armorNode->getBaseEquipment() && armorNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::ENHANCED) {
                                            armorNode->getNodeSprite()->setTexture("res/inventory-node-green-press.png");
                                        }
                                        else if (armorNode->getBaseEquipment() && armorNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::RARE) {
                                            armorNode->getNodeSprite()->setTexture("res/inventory-node-purple-press.png");
                                        }
                                        else if (armorNode->getBaseEquipment() && armorNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::LEGENDARY) {
                                            armorNode->getNodeSprite()->setTexture("res/inventory-node-orange-press.png");
                                        }
                                        else {
                                            armorNode->getNodeSprite()->setTexture("res/inventory-node-press.png");
                                        }
                                    }
                                    else if (equipment->getTypeName() == "Shield") {
                                        if (shieldNode->getBaseEquipment() && shieldNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::STANDARD) {
                                            shieldNode->getNodeSprite()->setTexture("res/inventory-node-black-press.png");
                                        }
                                        else if (shieldNode->getBaseEquipment() && shieldNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::ENHANCED) {
                                            shieldNode->getNodeSprite()->setTexture("res/inventory-node-green-press.png");
                                        }
                                        else if (shieldNode->getBaseEquipment() && shieldNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::RARE) {
                                            shieldNode->getNodeSprite()->setTexture("res/inventory-node-purple-press.png");
                                        }
                                        else if (shieldNode->getBaseEquipment() && shieldNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::LEGENDARY) {
                                            shieldNode->getNodeSprite()->setTexture("res/inventory-node-orange-press.png");
                                        }
                                        else {
                                            shieldNode->getNodeSprite()->setTexture("res/inventory-node-press.png");
                                        }
                                    }
                                    else if (equipment->getTypeName() == "Ring") {
                                        if (ringNode->getBaseEquipment() && ringNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::STANDARD) {
                                            ringNode->getNodeSprite()->setTexture("res/inventory-node-black-press.png");
                                        }
                                        else if (ringNode->getBaseEquipment() && ringNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::ENHANCED) {
                                            ringNode->getNodeSprite()->setTexture("res/inventory-node-green-press.png");
                                        }
                                        else if (ringNode->getBaseEquipment() && ringNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::RARE) {
                                            ringNode->getNodeSprite()->setTexture("res/inventory-node-purple-press.png");
                                        }
                                        else if (ringNode->getBaseEquipment() && ringNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::LEGENDARY) {
                                            ringNode->getNodeSprite()->setTexture("res/inventory-node-orange-press.png");
                                        }
                                        else {
                                            ringNode->getNodeSprite()->setTexture("res/inventory-node-press.png");
                                        }
                                    }
                                    else if (equipment->getTypeName() == "Shoe") {
                                        if (shoeNode->getBaseEquipment() && shoeNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::STANDARD) {
                                            shoeNode->getNodeSprite()->setTexture("res/inventory-node-black-press.png");
                                        }
                                        else if (shoeNode->getBaseEquipment() && shoeNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::ENHANCED) {
                                            shoeNode->getNodeSprite()->setTexture("res/inventory-node-green-press.png");
                                        }
                                        else if (shoeNode->getBaseEquipment() && shoeNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::RARE) {
                                            shoeNode->getNodeSprite()->setTexture("res/inventory-node-purple-press.png");
                                        }
                                        else if (shoeNode->getBaseEquipment() && shoeNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::LEGENDARY) {
                                            shoeNode->getNodeSprite()->setTexture("res/inventory-node-orange-press.png");
                                        }
                                        else {
                                            shoeNode->getNodeSprite()->setTexture("res/inventory-node-press.png");
                                        }
                                    }
                                    else if (equipment->getTypeName() == "Helmet") {
                                        if (helmetNode->getBaseEquipment() && helmetNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::STANDARD) {
                                            helmetNode->getNodeSprite()->setTexture("res/inventory-node-black-press.png");
                                        }
                                        else if (helmetNode->getBaseEquipment() && helmetNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::ENHANCED) {
                                            helmetNode->getNodeSprite()->setTexture("res/inventory-node-green-press.png");
                                        }
                                        else if (helmetNode->getBaseEquipment() && helmetNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::RARE) {
                                            helmetNode->getNodeSprite()->setTexture("res/inventory-node-purple-press.png");
                                        }
                                        else if (helmetNode->getBaseEquipment() && helmetNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::LEGENDARY) {
                                            helmetNode->getNodeSprite()->setTexture("res/inventory-node-orange-press.png");
                                        }
                                        else {
                                            helmetNode->getNodeSprite()->setTexture("res/inventory-node-press.png");
                                        }
                                    }
                                    currentNode = i;

                                    if (TutorialManager::getInstance()->currentTutorial != 3) {
                                        equipment->setScale(1.2 * Director::getInstance()->getContentScaleFactor());
                                        equipment->setPosition(startPoint + Vec2(-15.6 * Director::getInstance()->getContentScaleFactor(), 15.6 * Director::getInstance()->getContentScaleFactor()));
                                        inventoryBorder->addChild(equipment, 2500);
                                        i->removeBaseEquipment();
                                        isDraggingItem = true;
                                    }
                                    return true;
                                }
                                else if (i->getBaseItem() && TutorialManager::getInstance()->currentTutorial != 3) {
                                    item = ItemFactory::createItem(i->getBaseItem()->getItemName());
                                    
                                    item->setScale(1.2 * Director::getInstance()->getContentScaleFactor());
                                    
                                    if (i->getBaseItem()->getItemType() == BaseItem::POTION_ITEM) 
                                    {
                                        //bool flag1 = 
                                        for (auto i : listItemNodes) {
                                            i->getNodeSprite()->setTexture("res/inventory-node-press.png");
                                        }
                                    }
                                    item->setPosition(startPoint + Vec2(-15.6 * Director::getInstance()->getContentScaleFactor(), 15.6 * Director::getInstance()->getContentScaleFactor()));
                                    inventoryBorder->addChild(item, 2500);
                                    movingItemQuantity = i->getQuantity();
                                    i->removeBaseItem();
                                    currentNode = i;
                                    isDraggingItem = true;
                                    UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                                    return true;
                                }
                                return false;
                            }
                        }
                    }
                    return true;
                }
            }
            return false;
        };
    touchListener->onTouchMoved = [&](Touch* touch, Event* event)
        {
            auto movingPoint = this->convertToNodeSpace(touch->getLocation());
            if (overlay->isVisible() == false && isVisible()) {
                if (isDraggingItem && isSo && TutorialManager::getInstance()->currentTutorial != 3)
                {
                    if (equipment) {
                        // 
                        //

                        equipment->setPosition(movingPoint + Vec2(-15.6 * Director::getInstance()->getContentScaleFactor(), 15.6 * Director::getInstance()->getContentScaleFactor()));
                    }
                    else if (item) {
                        item->setPosition(movingPoint + Vec2(-15.6 * Director::getInstance()->getContentScaleFactor(), 15.6 * Director::getInstance()->getContentScaleFactor()));

                    }
                }
            }
        };
    touchListener->onTouchEnded = [&](Touch* touch, Event* event)
        {
            if (overlay->isVisible() == false && isVisible()) {

                auto endPoint = this->convertToNodeSpace(touch->getLocation());
                isSorting = false;
                bool isShowDetail = false;
                if ((equipment && currentNode->getBoundingNode().containsPoint(endPoint)) || (equipment && TutorialManager::getInstance()->currentTutorial == 3 && currentNode->getBoundingNode().containsPoint(endPoint))) {
                    equipment->setGlobalZOrder(50);
                 /*   for (auto i : this->listItemNodes) {
                        if (i->getBaseItem() && i->getBaseItem()->getItemName() == "Mana Potion") {
                            i->decreaseBaseItem(1);
                        }
                    }*/
                    showEquipmentDetails(equipment, isPlayerUsing);
                    isShowDetail = true;
                }
                else if (item && currentNode->getBoundingNode().containsPoint(endPoint) && TutorialManager::getInstance()->currentTutorial != 3) {
                    if (currentNode == itemNode1 || currentNode == itemNode2 || currentNode == itemNode3 || currentNode == itemNode4) {
                        showItemDetails(item, true);
                    }
                    else {
                        showItemDetails(item);
                    }
                    isShowDetail = true;
                }
                isPlayerUsing = false;
                if (isDraggingItem && isSo)
                {
                    if (equipment && weaponNode->getBoundingNode().containsPoint(equipment->getPosition()) && equipment->getTypeName() == "Weapon") {
                        if (weaponNode->getStatus() == "busy") {
                            auto tempName = player->getEquipment("Weapon")->getEquipmentName();
                            player->removeEquipment("Weapon");
                            currentNode->removeBaseEquipment();
                            currentNode->setBaseEquipment(tempName, player->getEquipment("Weapon")->getLevel());
                            currentNode->setQuantity(1);
                            weaponNode->removeBaseEquipment();
                        }
                        weaponNode->setBaseEquipment(equipment->getEquipmentName(), equipment->getLevel());
                        if (weaponNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::STANDARD)
                        {
                            weaponNode->getNodeSprite()->setTexture("res/inventory-node-black.png");
                        }
                        else if (weaponNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::ENHANCED)
                        {
                            weaponNode->getNodeSprite()->setTexture("res/inventory-node-green.png");
                        }
                        else if (weaponNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::RARE)
                        {
                            weaponNode->getNodeSprite()->setTexture("res/inventory-node-purple.png");
                        }
                        else if (weaponNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::LEGENDARY)
                        {
                            weaponNode->getNodeSprite()->setTexture("res/inventory-node-orange.png");
                        }
                        player->setEquipment(equipment->getEquipmentName(), equipment->getLevel(), "Weapon");
                        weaponNode->setQuantity(1);
                    }
                    else if (equipment && ringNode->getBoundingNode().containsPoint(equipment->getPosition()) && equipment->getTypeName() == "Ring") {
                        if (ringNode->getStatus() == "busy") {
                            auto tempName = player->getEquipment("Ring")->getEquipmentName();
                            player->removeEquipment("Ring");
                            currentNode->removeBaseEquipment();
                            currentNode->setBaseEquipment(tempName, player->getEquipment("Ring")->getLevel());
                            currentNode->setQuantity(1);
                            ringNode->removeBaseEquipment();
                            ringNode->setQuantity(0);
                        }
                        ringNode->setBaseEquipment(equipment->getEquipmentName(), equipment->getLevel());
                        if (ringNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::STANDARD)
                        {
                            ringNode->getNodeSprite()->setTexture("res/inventory-node-black.png");
                        }
                        else if (ringNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::ENHANCED)
                        {
                            ringNode->getNodeSprite()->setTexture("res/inventory-node-green.png");
                        }
                        else if (ringNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::RARE)
                        {
                            ringNode->getNodeSprite()->setTexture("res/inventory-node-purple.png");
                        }
                        else if (ringNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::LEGENDARY)
                        {
                            ringNode->getNodeSprite()->setTexture("res/inventory-node-orange.png");
                        }
                        player->setEquipment(equipment->getEquipmentName(), equipment->getLevel(), "Ring");
                        ringNode->setQuantity(1);
                    }
                    else if (equipment && helmetNode->getBoundingNode().containsPoint(equipment->getPosition()) && equipment->getTypeName() == "Helmet") {
                        if (helmetNode->getStatus() == "busy") {
                            auto tempName = player->getEquipment("Helmet")->getEquipmentName();
                            player->removeEquipment("Helmet");
                            currentNode->removeBaseEquipment();
                            currentNode->setBaseEquipment(tempName, player->getEquipment("Helmet")->getLevel());
                            currentNode->setQuantity(1);
                            helmetNode->removeBaseEquipment();
                            helmetNode->setQuantity(0);
                        }
                        helmetNode->setBaseEquipment(equipment->getEquipmentName(), equipment->getLevel());
                        if (helmetNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::STANDARD)
                        {
                            helmetNode->getNodeSprite()->setTexture("res/inventory-node-black.png");
                        }
                        else if (helmetNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::ENHANCED)
                        {
                            helmetNode->getNodeSprite()->setTexture("res/inventory-node-green.png");
                        }
                        else if (helmetNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::RARE)
                        {
                            helmetNode->getNodeSprite()->setTexture("res/inventory-node-purple.png");
                        }
                        else if (helmetNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::LEGENDARY)
                        {
                            helmetNode->getNodeSprite()->setTexture("res/inventory-node-orange.png");
                        }
                        player->setEquipment(equipment->getEquipmentName(), equipment->getLevel(), "Helmet");
                        helmetNode->setQuantity(1);
                    }
                    else if (equipment && shieldNode->getBoundingNode().containsPoint(equipment->getPosition()) && equipment->getTypeName() == "Shield") {
                        if (shieldNode->getStatus() == "busy") {
                            auto tempName = player->getEquipment("Shield")->getEquipmentName();
                            player->removeEquipment("Shield");
                            currentNode->removeBaseEquipment();
                            currentNode->setBaseEquipment(tempName, player->getEquipment("Shield")->getLevel());
                            currentNode->setQuantity(1);
                            shieldNode->removeBaseEquipment();
                            shieldNode->setQuantity(0);
                        }
                        shieldNode->setBaseEquipment(equipment->getEquipmentName(), equipment->getLevel());
                        if (shieldNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::STANDARD)
                        {
                            shieldNode->getNodeSprite()->setTexture("res/inventory-node-black.png");
                        }
                        else if (shieldNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::ENHANCED)
                        {
                            shieldNode->getNodeSprite()->setTexture("res/inventory-node-green.png");
                        }
                        else if (shieldNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::RARE)
                        {
                            shieldNode->getNodeSprite()->setTexture("res/inventory-node-purple.png");
                        }
                        else if (shieldNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::LEGENDARY)
                        {
                            shieldNode->getNodeSprite()->setTexture("res/inventory-node-orange.png");
                        }
                        player->setEquipment(equipment->getEquipmentName(), equipment->getLevel(), "Shield");
                        shieldNode->setQuantity(1);
                    }
                    else if (equipment && shoeNode->getBoundingNode().containsPoint(equipment->getPosition()) && equipment->getTypeName() == "Shoe") {
                        if (shoeNode->getStatus() == "busy") {
                            auto tempName = player->getEquipment("Shoe")->getEquipmentName();
                            player->removeEquipment("Shoe");
                            currentNode->removeBaseEquipment();
                            currentNode->setBaseEquipment(tempName, player->getEquipment("Shoe")->getLevel());
                            currentNode->setQuantity(1);
                            shoeNode->removeBaseEquipment();
                            shoeNode->setQuantity(0);
                        }
                        shoeNode->setBaseEquipment(equipment->getEquipmentName(), equipment->getLevel());
                        if (shoeNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::STANDARD)
                        {
                            shoeNode->getNodeSprite()->setTexture("res/inventory-node-black.png");
                        }
                        else if (shoeNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::ENHANCED)
                        {
                            shoeNode->getNodeSprite()->setTexture("res/inventory-node-green.png");
                        }
                        else if (shoeNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::RARE)
                        {
                            shoeNode->getNodeSprite()->setTexture("res/inventory-node-purple.png");
                        }
                        else if (shoeNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::LEGENDARY)
                        {
                            shoeNode->getNodeSprite()->setTexture("res/inventory-node-orange.png");
                        }
                        player->setEquipment(equipment->getEquipmentName(), equipment->getLevel(), "Shoe");
                        shoeNode->setQuantity(1);
                    }
                    else if (equipment && armorNode->getBoundingNode().containsPoint(equipment->getPosition()) && equipment->getTypeName() == "Armor") {
                        if (armorNode->getStatus() == "busy") {
                            auto tempName = player->getEquipment("Armor")->getEquipmentName();
                            player->removeEquipment("Armor");
                            currentNode->removeBaseEquipment();
                            currentNode->setBaseEquipment(tempName, player->getEquipment("Armor")->getLevel());
                            currentNode->setQuantity(1);
                            armorNode->removeBaseEquipment();
                            armorNode->setQuantity(0);
                        }
                        armorNode->setBaseEquipment(equipment->getEquipmentName(), equipment->getLevel());
                        if (armorNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::STANDARD)
                        {
                            armorNode->getNodeSprite()->setTexture("res/inventory-node-black.png");
                        }
                        else if (armorNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::ENHANCED)
                        {
                            armorNode->getNodeSprite()->setTexture("res/inventory-node-green.png");
                        }
                        else if (armorNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::RARE)
                        {
                            armorNode->getNodeSprite()->setTexture("res/inventory-node-purple.png");
                        }
                        else if (armorNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::LEGENDARY)
                        {
                            armorNode->getNodeSprite()->setTexture("res/inventory-node-orange.png");
                        }
                        player->setEquipment(equipment->getEquipmentName(), equipment->getLevel(), "Armor");
                        armorNode->setQuantity(1);
                    }
                    else if (item && itemNode1->getBoundingNode(0.6).containsPoint(item->getPosition()) && item->getItemType() == BaseItem::POTION_ITEM) {
                        if (itemNode1->getStatus() == "busy") {
                            int itemNodeQuantity = itemNode1->getQuantity();
                            currentNode->removeBaseItem();
                            currentNode->setBaseItem(itemNode1->getBaseItem()->getItemName());
                            currentNode->setQuantity(itemNodeQuantity);
                            /*int nodeID = currentNode == itemNode3 ? 3 : currentNode == itemNode2 ? 2 : currentNode == itemNode4 ? 4 : 1;
                            if (nodeID >= 1 && nodeID <= 4) InGameUI::getInstance(player)->subInventory->addItem(itemNode1->getBaseItem()->getItemName(), itemNodeQuantity, nodeID);*/

                            InGameUI::getInstance(player)->subInventory->removeItem(itemNode1->getBaseItem()->getItemName());
                            itemNode1->removeBaseItem();
                            itemNode1->setQuantity(0);
                            int id = currentNode == itemNode1 ? 1 : (currentNode == itemNode2) ? 2 : (currentNode == itemNode3) ? 3 : (currentNode == itemNode4) ? 4 : 0;
                            if (id >= 1 && id <= 4) InGameUI::getInstance(player)->subInventory->addItem(currentNode->getBaseItem()->getItemName(), currentNode->getQuantity(), id);
                        }
                        itemNode1->setBaseItem(item->getItemName());
                        itemNode1->setQuantity(movingItemQuantity);
                        InGameUI::getInstance(player)->subInventory->addItem(item->getItemName(), movingItemQuantity, 1);
                    }
                    else if (item && itemNode2->getBoundingNode(0.6).containsPoint(item->getPosition()) && item->getItemType() == BaseItem::POTION_ITEM) {
                        if (itemNode2->getStatus() == "busy") {
                            int itemNodeQuantity = itemNode2->getQuantity();
                            currentNode->removeBaseItem();
                            currentNode->setBaseItem(itemNode2->getBaseItem()->getItemName());
                            currentNode->setQuantity(itemNodeQuantity);
                            /*int nodeID = currentNode == itemNode1 ? 1 : currentNode == itemNode3 ? 3 : currentNode == itemNode4 ? 4 : 1;
                            if (nodeID >= 1 && nodeID <= 4) InGameUI::getInstance(player)->subInventory->addItem(itemNode2->getBaseItem()->getItemName(), itemNodeQuantity, nodeID);*/
                            //InGameUI::getInstance(player)->subInventory->addItem(itemNode2->getBaseItem()->getItemName(), itemNodeQuantity, 1);
                            InGameUI::getInstance(player)->subInventory->removeItem(itemNode2->getBaseItem()->getItemName());
                            itemNode2->removeBaseItem();
                            itemNode2->setQuantity(0);
                            int id = currentNode == itemNode1 ? 1 : (currentNode == itemNode2) ? 2 : (currentNode == itemNode3) ? 3 : (currentNode == itemNode4) ? 4 : 0;
                            if (id >= 1 && id <= 4) InGameUI::getInstance(player)->subInventory->addItem(currentNode->getBaseItem()->getItemName(), currentNode->getQuantity(), id);
                        }
                        itemNode2->setBaseItem(item->getItemName());
                        itemNode2->setQuantity(movingItemQuantity);
                        InGameUI::getInstance(player)->subInventory->addItem(item->getItemName(), movingItemQuantity, 2);
                    }
                    else if (item && itemNode3->getBoundingNode(0.6).containsPoint(item->getPosition()) && item->getItemType() == BaseItem::POTION_ITEM) {
                        if (itemNode3->getStatus() == "busy") {
                            int itemNodeQuantity = itemNode3->getQuantity();
                            currentNode->removeBaseItem();
                            currentNode->setBaseItem(itemNode3->getBaseItem()->getItemName());
                            currentNode->setQuantity(itemNodeQuantity);
                            /*int nodeID = currentNode == itemNode1 ? 1 : currentNode == itemNode2 ? 2 : currentNode == itemNode4 ? 4 : 1;
                            if(nodeID >= 1 && nodeID <= 4) InGameUI::getInstance(player)->subInventory->addItem(itemNode3->getBaseItem()->getItemName(), itemNodeQuantity, nodeID);*/
                            //InGameUI::getInstance(player)->subInventory->addItem(itemNode3->getBaseItem()->getItemName(), itemNodeQuantity, 2);
                            InGameUI::getInstance(player)->subInventory->removeItem(itemNode3->getBaseItem()->getItemName());
                            itemNode3->removeBaseItem();
                            itemNode3->setQuantity(0);
                            int id = currentNode == itemNode1 ? 1 : (currentNode == itemNode2) ? 2 : (currentNode == itemNode3) ? 3 : (currentNode == itemNode4) ? 4 : 0;
                            if (id >= 1 && id <= 4) InGameUI::getInstance(player)->subInventory->addItem(currentNode->getBaseItem()->getItemName(), currentNode->getQuantity(), id);
                        }
                        itemNode3->setBaseItem(item->getItemName());
                        itemNode3->setQuantity(movingItemQuantity);
                        InGameUI::getInstance(player)->subInventory->addItem(item->getItemName(), movingItemQuantity, 3);
                    }
                    else if (item && itemNode4->getBoundingNode(0.6).containsPoint(item->getPosition()) && item->getItemType() == BaseItem::POTION_ITEM) {
                        if (itemNode4->getStatus() == "busy") {
                            int itemNodeQuantity = itemNode4->getQuantity();
                            currentNode->removeBaseItem();
                            currentNode->setBaseItem(itemNode4->getBaseItem()->getItemName());
                            currentNode->setQuantity(itemNodeQuantity);
                           /* int nodeID = currentNode == itemNode1 ? 1 : currentNode == itemNode2 ? 2 : currentNode == itemNode3 ? 3 : 1;
                            InGameUI::getInstance(player)->subInventory->addItem(itemNode4->getBaseItem()->getItemName(), itemNodeQuantity, nodeID);*/
                            //InGameUI::getInstance(player)->subInventory->addItem(itemNode4->getBaseItem()->getItemName(), itemNodeQuantity, 3);
                            InGameUI::getInstance(player)->subInventory->removeItem(itemNode4->getBaseItem()->getItemName());
                            itemNode4->removeBaseItem();
                            itemNode4->setQuantity(0);
                            int id = currentNode == itemNode1 ? 1 : (currentNode == itemNode2) ? 2 : (currentNode == itemNode3) ? 3 : (currentNode == itemNode4) ? 4 : 0;
                            if (id >= 1 && id <= 4) InGameUI::getInstance(player)->subInventory->addItem(currentNode->getBaseItem()->getItemName(), currentNode->getQuantity(), id);
                        }
                        itemNode4->setBaseItem(item->getItemName());
                        itemNode4->setQuantity(movingItemQuantity);
                        InGameUI::getInstance(player)->subInventory->addItem(item->getItemName(), movingItemQuantity, 4);
                    }
                    else {
                        int count = 0;
                        for (auto& i : inventoryNodes)
                        {
                            if (i->isVisible()) {
                                if (isShowDetail == false && equipment && i->getBoundingNode().containsPoint(equipment->getPosition()) && i->getStatus() == "free")
                                {
                                    i->setBaseEquipment(equipment->getEquipmentName(), equipment->getLevel());
                                    i->setQuantity(1);
                                    count++;
                                    break;
                                }
                                else if (isShowDetail == false && item && i->getBoundingNode().containsPoint(item->getPosition()) && i->getStatus() == "free") {
                                    i->setBaseItem(item->getItemName());
                                    i->setQuantity(movingItemQuantity);
                                    movingItemQuantity = 0;
                                    count++;
                                    break;
                                }
                            }
                        }
                        if (count == 0)
                        {
                            if (currentNode == weaponNode) {
                                currentNode->setBaseEquipment(equipment->getEquipmentName(), equipment->getLevel());
                                player->setEquipment(equipment->getEquipmentName(), equipment->getLevel(), equipment->getTypeName());
                                currentNode->setQuantity(1);
                                count++;
                            }
                            else if (currentNode == helmetNode) {
                                currentNode->setBaseEquipment(equipment->getEquipmentName(), equipment->getLevel());
                                player->setEquipment(equipment->getEquipmentName(), equipment->getLevel(), equipment->getTypeName());
                                currentNode->setQuantity(1);
                                count++;
                            }
                            else if (currentNode == ringNode) {
                                currentNode->setBaseEquipment(equipment->getEquipmentName(), equipment->getLevel());
                                player->setEquipment(equipment->getEquipmentName(), equipment->getLevel(), equipment->getTypeName());
                                currentNode->setQuantity(1);
                                count++;
                            }
                            else if (currentNode == shieldNode) {
                                currentNode->setBaseEquipment(equipment->getEquipmentName(), equipment->getLevel());
                                player->setEquipment(equipment->getEquipmentName(), equipment->getLevel(), equipment->getTypeName());
                                currentNode->setQuantity(1);
                                count++;
                            }
                            else if (currentNode == armorNode) {
                                currentNode->setBaseEquipment(equipment->getEquipmentName(), equipment->getLevel());
                                player->setEquipment(equipment->getEquipmentName(), equipment->getLevel(), equipment->getTypeName());
                                currentNode->setQuantity(1);
                                count++;
                            }
                            else if (currentNode == shoeNode) {
                                currentNode->setBaseEquipment(equipment->getEquipmentName(), equipment->getLevel());
                                player->setEquipment(equipment->getEquipmentName(), equipment->getLevel(), equipment->getTypeName());
                                currentNode->setQuantity(1);
                                count++;
                            }
                            else {
                                if (equipment) {
                                    currentNode->setBaseEquipment(equipment->getEquipmentName(), equipment->getLevel());
                                    currentNode->setQuantity(1);
                                }
                                else if (item) {
                                    currentNode->setBaseItem(item->getItemName());
                                    currentNode->setQuantity(movingItemQuantity);
                                    if (currentNode == itemNode1 || currentNode == itemNode2 || currentNode == itemNode3 || currentNode == itemNode4) {
                                        InGameUI::getInstance(player)->subInventory->addItem(item->getItemName(), movingItemQuantity);
                                    }
                                }
                                count++;
                            }

                        }
                        
                    }
                    if (equipment) {
                        dmgStat->setString(" DMG: " + std::to_string(static_cast<int>(player->getDamage() + player->getEquipmentDamage())));
                        mpStat->setString(" MP: " + std::to_string(static_cast<int>(player->getMP() + player->getEquipmentMP())));
                        hpStat->setString(" HP: " + std::to_string(static_cast<int>(player->getHP() + player->getEquipmentHP())));
                        msStat->setString(" Movement speed: " + std::to_string(static_cast<int>(player->getMovementSpeed() + player->getEquipmentMS())));
                        cdrStat->setString(" Skill duration: " + std::to_string(static_cast<int>(player->getCDR() + player->getEquipmentCDR())));
                        armStat->setString(" Armor: " + std::to_string(static_cast<int>(player->getArmor() + player->getEquipmentArmor())));
                        equipment->removeFromParentAndCleanup(true);
                        equipment = nullptr;
                        if(weaponNode->getBaseEquipment() == nullptr) weaponNode->getNodeSprite()->setTexture("res/inventory-node.png");
                        else {
                            if (weaponNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::STANDARD)
                            {
                                weaponNode->getNodeSprite()->setTexture("res/inventory-node-black.png");
                                UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/equip_item_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                            }
                            else if (weaponNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::ENHANCED)
                            {
                                weaponNode->getNodeSprite()->setTexture("res/inventory-node-green.png");
                                UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/equip_item_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                            }
                            else if (weaponNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::RARE)
                            {
                                weaponNode->getNodeSprite()->setTexture("res/inventory-node-purple.png");
                                UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/equip_item_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                            }
                            else if (weaponNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::LEGENDARY)
                            {
                                weaponNode->getNodeSprite()->setTexture("res/inventory-node-orange.png");
                                UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/equip_item_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                            }
                        }

                        if(armorNode->getBaseEquipment() == nullptr) armorNode->getNodeSprite()->setTexture("res/inventory-node.png");
                        else {
                            if (armorNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::STANDARD)
                            {
                                armorNode->getNodeSprite()->setTexture("res/inventory-node-black.png");
                                UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/equip_item_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                            }
                            else if (armorNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::ENHANCED)
                            {
                                armorNode->getNodeSprite()->setTexture("res/inventory-node-green.png");
                                UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/equip_item_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                            }
                            else if (armorNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::RARE)
                            {
                                armorNode->getNodeSprite()->setTexture("res/inventory-node-purple.png");
                                UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/equip_item_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                            }
                            else if (armorNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::LEGENDARY)
                            {
                                armorNode->getNodeSprite()->setTexture("res/inventory-node-orange.png");
                                UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/equip_item_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                            }
                        }

                        if(shieldNode->getBaseEquipment() == nullptr) shieldNode->getNodeSprite()->setTexture("res/inventory-node.png");
                        else {
                            if (shieldNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::STANDARD)
                            {
                                shieldNode->getNodeSprite()->setTexture("res/inventory-node-black.png");
                                UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/equip_item_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                            }
                            else if (shieldNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::ENHANCED)
                            {
                                shieldNode->getNodeSprite()->setTexture("res/inventory-node-green.png");
                                UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/equip_item_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                            }
                            else if (shieldNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::RARE)
                            {
                                shieldNode->getNodeSprite()->setTexture("res/inventory-node-purple.png");
                                UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/equip_item_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                            }
                            else if (shieldNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::LEGENDARY)
                            {
                                shieldNode->getNodeSprite()->setTexture("res/inventory-node-orange.png");
                                UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/equip_item_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                            }
                        }

                        if(ringNode->getBaseEquipment() == nullptr) ringNode->getNodeSprite()->setTexture("res/inventory-node.png");
                        else {
                            if (ringNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::STANDARD)
                            {
                                ringNode->getNodeSprite()->setTexture("res/inventory-node-black.png");
                                UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/equip_item_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                            }
                            else if (ringNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::ENHANCED)
                            {
                                ringNode->getNodeSprite()->setTexture("res/inventory-node-green.png");
                                UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/equip_item_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                            }
                            else if (ringNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::RARE)
                            {
                                ringNode->getNodeSprite()->setTexture("res/inventory-node-purple.png");
                                UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/equip_item_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                            }
                            else if (ringNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::LEGENDARY)
                            {
                                ringNode->getNodeSprite()->setTexture("res/inventory-node-orange.png");
                                UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/equip_item_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                            }
                        }

                        if(shoeNode->getBaseEquipment() == nullptr) shoeNode->getNodeSprite()->setTexture("res/inventory-node.png");
                        else {
                            if (shoeNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::STANDARD)
                            {
                                shoeNode->getNodeSprite()->setTexture("res/inventory-node-black.png");
                                UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/equip_item_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                            }
                            else if (shoeNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::ENHANCED)
                            {
                                shoeNode->getNodeSprite()->setTexture("res/inventory-node-green.png");
                                UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/equip_item_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                            }
                            else if (shoeNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::RARE)
                            {
                                shoeNode->getNodeSprite()->setTexture("res/inventory-node-purple.png");
                                UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/equip_item_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                            }
                            else if (shoeNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::LEGENDARY)
                            {
                                shoeNode->getNodeSprite()->setTexture("res/inventory-node-orange.png");
                                UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/equip_item_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                            }
                        }

                        if(helmetNode->getBaseEquipment() == nullptr) helmetNode->getNodeSprite()->setTexture("res/inventory-node.png");
                        else {

                            if (helmetNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::STANDARD)
                            {
                                helmetNode->getNodeSprite()->setTexture("res/inventory-node-black.png");
                                UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/equip_item_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                            }
                            else if (helmetNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::ENHANCED)
                            {
                                helmetNode->getNodeSprite()->setTexture("res/inventory-node-green.png");
                                UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/equip_item_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                            }
                            else if (helmetNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::RARE)
                            {
                                helmetNode->getNodeSprite()->setTexture("res/inventory-node-purple.png");
                                UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/equip_item_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                            }
                            else if (helmetNode->getBaseEquipment()->getEquipmentType() == BaseEquipment::Type::LEGENDARY)
                            {
                                helmetNode->getNodeSprite()->setTexture("res/inventory-node-orange.png");
                                UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/equip_item_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                            }
                        }

                    }
                    else if (item) {
                        item->removeFromParentAndCleanup(true);
                        item = nullptr;
                        for (auto i : listItemNodes) {
                            i->getNodeSprite()->setTexture("res/inventory-node.png");
                        }
                    }
                }
                isDraggingItem = false;
            }

        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);


    mainInventory = Sprite::create("res/inventory-border.png");


    subInventory = Sprite::create("res/inventory-border.png");
    inventoryBorder->addChild(subInventory, 10);

    auto statsBorder = Sprite::create("res/stats-border.png");
    statsBorder->setPosition(Vec2(0, 0));
    statsBorder->setAnchorPoint(Vec2(0, 0));
    subInventory->addChild(statsBorder, 12);

    inventoryBorder->addChild(mainInventory, 12);
    float subInventoryScaleX = (visibleSize.width / 2 / (visibleSize.width / visibleSize.height)) / subInventory->getContentSize().width;
    float subInventoryScaleY = visibleSize.height / (visibleSize.width / visibleSize.height) / subInventory->getContentSize().height;
    subInventory->setScale(subInventoryScaleX, subInventoryScaleY);
    mainInventory->setScale(subInventoryScaleX, subInventoryScaleY);

    subInventory->setPosition(Vec2(-mainInventory->getContentSize().width * subInventoryScaleX / 2, 0));
    mainInventory->setPosition(Vec2(subInventory->getContentSize().width * subInventoryScaleX / 2, 0));

    buttonClose = Sprite::create("res/c_header_close.png");
    buttonClose->setScale(0.6 * Director::getInstance()->getContentScaleFactor());
    buttonClose->retain();
    buttonClose->setPosition(Vec2(mainInventory->getPositionX() + mainInventory->getContentSize().width * subInventoryScaleX / 2 - buttonClose->getContentSize().width * 0.4, mainInventory->getPositionY() + mainInventory->getContentSize().height * subInventoryScaleY / 2 - buttonClose->getContentSize().height * 0.4));
    inventoryBorder->addChild(buttonClose, 50);

    sortButton = Sprite::create("res/sort_button.png");
    sortButton->setScale(0.5 * Director::getInstance()->getContentScaleFactor());
    sortButton->retain();
    sortButton->setPosition(Vec2(mainInventory->getPositionX() + mainInventory->getContentSize().width * subInventoryScaleX / 2 - sortButton->getContentSize().width * (0.5 * Director::getInstance()->getContentScaleFactor()) * 0.5, 0));
    inventoryBorder->addChild(sortButton, 50);

    auto gaper = Director::getInstance()->getContentScaleFactor();
    auto characterSprite = Sprite::create("res/charactor-inventory.png");
    characterSprite->setPosition(subInventory->getPositionX() - 15 * gaper, subInventory->getPositionY() + 15 * gaper);
    characterSprite->setFlippedX(true);
    characterSprite->setScale(0.76 * gaper);
    inventoryBorder->addChild(characterSprite, 30);


    float squareSize = (subInventory->getContentSize().width * subInventoryScaleX) / 6.5;
    const float gapX = squareSize / 30;
    const float gapY = squareSize / 30;

    level = Label::createWithTTF("Knight - Level " + std::to_string(_player->getLevel()), "fonts/Diablo Light.ttf", 16);
    level->setPosition(characterSprite->getPosition() + Vec2(0, characterSprite->getContentSize().height * characterSprite->getScale() / 2 + squareSize));
    inventoryBorder->addChild(level, 30);

    #pragma region stats
    playerStats = Node::create();
    dmgStat = Label::createWithTTF(" DMG: " + std::to_string(static_cast<int>(_player->getDamage() + _player->getEquipmentDamage())), "fonts/Diablo Light.ttf", 18);
    dmgStat->setAnchorPoint(Vec2(0, 0));
    dmgStat->setPosition(0, 0);
    playerStats->addChild(dmgStat);
    mpStat = Label::createWithTTF(" MP: " + std::to_string(static_cast<int>(_player->getMP() + _player->getEquipmentMP())), "fonts/Diablo Light.ttf", 18);
    mpStat->setAnchorPoint(Vec2(0, 0));
    mpStat->setPosition(0, 30);
    playerStats->addChild(mpStat);
    hpStat = Label::createWithTTF(" HP: " + std::to_string(static_cast<int>(_player->getHP() + _player->getEquipmentHP())), "fonts/Diablo Light.ttf", 18);
    hpStat->setAnchorPoint(Vec2(0, 0));
    hpStat->setPosition(0, 60);
    playerStats->addChild(hpStat);
    msStat = Label::createWithTTF(" Movement speed: " + std::to_string(static_cast<int>(_player->getMS() + _player->getEquipmentMS())), "fonts/Diablo Light.ttf", 18);
    msStat->setAnchorPoint(Vec2(0, 0));
    msStat->setPosition((subInventory->getContentSize().width * subInventoryScaleX) / 1.8, 0);
    playerStats->addChild(msStat);
    cdrStat = Label::createWithTTF(" Skill duration: " + std::to_string(static_cast<int>(_player->getCDR() + _player->getEquipmentCDR())), "fonts/Diablo Light.ttf", 18);
    cdrStat->setAnchorPoint(Vec2(0, 0));
    cdrStat->setPosition((subInventory->getContentSize().width * subInventoryScaleX) / 1.8, 30);
    playerStats->addChild(cdrStat);
    armStat = Label::createWithTTF(" Armor: " + std::to_string(static_cast<int>(_player->getArmor() + _player->getEquipmentArmor())), "fonts/Diablo Light.ttf", 18);
    armStat->setAnchorPoint(Vec2(0, 0));
    armStat->setPosition((subInventory->getContentSize().width * subInventoryScaleX) / 1.8, 60);

    playerStats->addChild(armStat);
    playerStats->setAnchorPoint(Vec2(0, 0));
    playerStats->setScale(0.5);
    playerStats->setPosition(subInventory->getPositionX() - (subInventory->getContentSize().width * subInventoryScaleX) / 2.2, subInventory->getPositionY() - (subInventory->getContentSize().height * subInventoryScaleY) / 2.2);
    inventoryBorder->addChild(playerStats, 10);

    itemNode1 = InventoryNode::createInventoryNode(squareSize);
    itemNode1->getNodeSprite()->setScale(0.9);
    itemNode1->setIsInventory(true);
    itemNode1->setPosition(subInventory->getPositionX() + (subInventory->getContentSize().width / 2) + 13 * gaper - squareSize * 0.9 * 4 + squareSize * 0.9, subInventory->getPositionY() + gapY * 25 - squareSize * 0.9 * 2);
    inventoryBorder->addChild(itemNode1, 21);
    listItemNodes.push_back(itemNode1);

    itemNode2 = InventoryNode::createInventoryNode(squareSize);
    itemNode2->getNodeSprite()->setScale(0.9);
    itemNode2->setIsInventory(true);
    itemNode2->setPosition(subInventory->getPositionX() + (subInventory->getContentSize().width / 2) + 20.5 * gaper - squareSize * 0.9 * 4 + squareSize * 0.9 * 0.25 + squareSize * 0.9, subInventory->getPositionY() + gapY * 25 - squareSize * 0.9 * 2);
    inventoryBorder->addChild(itemNode2, 21);
    listItemNodes.push_back(itemNode2);

    itemNode3 = InventoryNode::createInventoryNode(squareSize);
    itemNode3->getNodeSprite()->setScale(0.9);
    itemNode3->setIsInventory(true);
    itemNode3->setPosition(subInventory->getPositionX() + (subInventory->getContentSize().width / 2) + 28 * gaper - squareSize * 0.9 * 4 + squareSize * 0.9 * 0.5 + squareSize * 0.9, subInventory->getPositionY() + gapY * 25 - squareSize * 0.9 * 2);
    inventoryBorder->addChild(itemNode3, 21);
    listItemNodes.push_back(itemNode3);

    itemNode4 = InventoryNode::createInventoryNode(squareSize);
    itemNode4->getNodeSprite()->setScale(0.9);
    itemNode4->setIsInventory(true);
    itemNode4->setPosition(subInventory->getPositionX() + (subInventory->getContentSize().width / 2) + 35.5 * gaper - squareSize * 0.9 * 4 + squareSize * 0.9 * 0.75 + squareSize * 0.9, subInventory->getPositionY() + gapY * 25 - squareSize * 0.9 * 2);
    inventoryBorder->addChild(itemNode4, 21);
    listItemNodes.push_back(itemNode4);


    weaponNode = InventoryNode::createInventoryNode(squareSize);
    weaponNode->setPosition(subInventory->getPositionX() + (subInventory->getContentSize().width / 2) + 13 * gaper, subInventory->getPositionY() + gapY * 25);
    inventoryBorder->addChild(weaponNode, 21);

    armorNode = InventoryNode::createInventoryNode(squareSize);
    armorNode->setPosition(subInventory->getPositionX() - (subInventory->getContentSize().width / 2) - squareSize - 43 * gaper, subInventory->getPositionY() + gapY * 25);
    inventoryBorder->addChild(armorNode, 21);

    ringNode = InventoryNode::createInventoryNode(squareSize);
    ringNode->setPosition(subInventory->getPositionX() + (subInventory->getContentSize().width / 2) + 13 * gaper, subInventory->getPositionY() - (squareSize + gapY) + gapY * 25);
    inventoryBorder->addChild(ringNode, 21);

    shieldNode = InventoryNode::createInventoryNode(squareSize);
    shieldNode->setPosition(subInventory->getPositionX() - (subInventory->getContentSize().width / 2) - squareSize - 43 * gaper, subInventory->getPositionY() - (squareSize + gapY) + gapY * 25);
    inventoryBorder->addChild(shieldNode, 21);

    shoeNode = InventoryNode::createInventoryNode(squareSize);
    shoeNode->setPosition(subInventory->getPositionX() + (subInventory->getContentSize().width / 2) + 13 * gaper, subInventory->getPositionY() - (squareSize + gapY) * 2 + gapY * 25);
    inventoryBorder->addChild(shoeNode, 21);

    helmetNode = InventoryNode::createInventoryNode(squareSize);
    helmetNode->setPosition(subInventory->getPositionX() - (subInventory->getContentSize().width / 2) - squareSize - 43 * gaper, subInventory->getPositionY() - (squareSize + gapY) * 2 + gapY * 25);
    inventoryBorder->addChild(helmetNode, 21);
    #pragma endregion


    pageLabel = Label::createWithTTF("1/3", "fonts/Diablo Light.ttf", 18);
    pageLabel->setTextColor(Color4B::WHITE);
    pageLabel->setPosition(mainInventory->getPosition() - Vec2(pageLabel->getContentSize().width, mainInventory->getContentSize().height * mainInventory->getScaleY() * 0.5 + pageLabel->getContentSize().height / 6));
    inventoryBorder->addChild(pageLabel, 50);

    prevInvenButton = Sprite::create("res/left-arr-ui.png");
    prevInvenButton->setScale(1.65);
    prevInvenButton->setPosition(pageLabel->getPosition() + Vec2(-(prevInvenButton->getContentSize().width * 5 + pageLabel->getContentSize().width / 2), -prevInvenButton->getContentSize().height * prevInvenButton->getScaleY() / 8 + 0.2 * Director::getInstance()->getContentScaleFactor()));
    inventoryBorder->addChild(prevInvenButton, 25);

    nextInvenButton = Sprite::create("res/right-arr-ui.png");
    nextInvenButton->setScale(1.65);
    nextInvenButton->setPosition(pageLabel->getPosition() + Vec2(nextInvenButton->getContentSize().width * 5 + pageLabel->getContentSize().width / 2, -nextInvenButton->getContentSize().height * prevInvenButton->getScaleY() / 8 + 0.2 * Director::getInstance()->getContentScaleFactor()));
    inventoryBorder->addChild(nextInvenButton, 25);

    int count = 0;
    auto t = Vec2(subInventory->getContentSize().width * subInventoryScaleX * 0.08, subInventory->getContentSize().height * subInventoryScaleY * 0.26);
    while (count < totalInventoryPage) {
        for (int row = 0; row < numRows; ++row)
        {
            for (int col = 0; col < numCols; ++col)
            {
                auto node = InventoryNode::createInventoryNode(squareSize);
                node->setPosition(t + Vec2(col * (squareSize + gapX), -row * (squareSize + gapY)) - Vec2(squareSize + gapX, 0));
                inventoryBorder->addChild(node, 20);
                inventoryNodes.push_back(node); 
            }
        }
        count++;
    }
    Inventory::onPageChange();
    //float totalHeight = 0;
    //for (auto node : nodesToAdd) {
    //    totalHeight += squareSize / 6;
    //}
    //scrollview->getScrollView()->setInnerContainerSize(Size(scrollview->getScrollView()->getContentSize().width, totalHeight));

    //float posY = totalHeight;
    //for (auto node : nodesToAdd) {
    //    node->setPosition(Vec2(node->getContentSize().width / 2, posY - node->getContentSize().height / 2));
    //    scrollview->addToScrollView(node);
    //    posY -= node->getContentSize().height;
    //}
    //inventoryBorder->addChild(scrollview, 20);
    hideInventory();

    return true;
}

void Inventory::nextInventoryPage() {
    if (prevInvenButton->isVisible() == false) prevInvenButton->setVisible(true);
    if (currentInventoryPage < totalInventoryPage - 1) {
        currentInventoryPage++;
        onPageChange();
    }
}
void Inventory::prevInventoryPage() {
    if (nextInvenButton->isVisible() == false) nextInvenButton->setVisible(true);
    if (currentInventoryPage > 0) {
        currentInventoryPage--;
        onPageChange();
    }
}
void Inventory::onPageChange() {
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
bool Inventory::compareByEquipmentType(const InventoryNode* a, const InventoryNode* b) {
    if (a->getBaseEquipment() && b->getBaseEquipment()) {
        //return a->getBaseItem()->getItemType() < b->getBaseItem()->getItemType();
        if (a->getBaseEquipment()->getEquipmentType() == BaseEquipment::STANDARD) {
            if (b->getBaseEquipment()->getEquipmentType() == BaseEquipment::STANDARD) {
                return false;
            }
            else return true;
        } 
        if (a->getBaseEquipment()->getEquipmentType() == BaseEquipment::ENHANCED) {
            if (b->getBaseEquipment()->getEquipmentType() == BaseEquipment::STANDARD) {
                return false;
            }
            else if (b->getBaseEquipment()->getEquipmentType() == BaseEquipment::ENHANCED) {
                return false;
            }
            else return true;
        } 
        if (a->getBaseEquipment()->getEquipmentType() == BaseEquipment::RARE) {
            if (b->getBaseEquipment()->getEquipmentType() == BaseEquipment::STANDARD) {
                return false;
            }
            else if (b->getBaseEquipment()->getEquipmentType() == BaseEquipment::ENHANCED) {
                return false;
            }
            else if (b->getBaseEquipment()->getEquipmentType() == BaseEquipment::RARE) {
                return false;
            }
            else return true;
        } 
        if (a->getBaseEquipment()->getEquipmentType() == BaseEquipment::LEGENDARY) {
            return true;
        } 

    }
    return false;
}
void Inventory::sort() {
    if (Inventory::isShow() && isSorted == false && isSorting == false) {
        isSorting = true;
       // std::sort(this->inventoryNodes.begin(), this->inventoryNodes.end(), compareByEquipmentType);

        for (int i = 0; i < this->inventoryNodes.size(); i++) {
            for (int j = i + 1; j < this->inventoryNodes.size(); j++) {
                if (this->inventoryNodes[i]->getBaseItem() && this->inventoryNodes[j]->getBaseItem() && this->inventoryNodes[i]->getBaseItem()->getItemName() == this->inventoryNodes[j]->getBaseItem()->getItemName()) {
                    this->inventoryNodes[i]->setQuantity(this->inventoryNodes[i]->getQuantity() + this->inventoryNodes[j]->getQuantity());
                    this->inventoryNodes[j]->removeBaseItem();
                }
            }
        }
        bool isCompleted = false;
        for (int i = 0; i < this->inventoryNodes.size(); i++) {
            for (int j = i + 1; j < this->inventoryNodes.size(); j++) {
                if (i > 0) {
                    if (this->inventoryNodes[(i - 1)]->getBaseItem()) break;
                }
                if (this->inventoryNodes[i]->getBaseItem() && this->inventoryNodes[j]->getStatus() == "free") {
                    isCompleted = false;
                    if (i > 0) {
                        if (this->inventoryNodes[(i - 1)]->getBaseItem()) isCompleted = true;
                    }
                    if (isCompleted)
                        break;
                    std::string nameI = this->inventoryNodes[i]->getBaseItem()->getItemName();
                    int quantityI = this->inventoryNodes[i]->getQuantity();
                    this->inventoryNodes[j]->setBaseItem(nameI);
                    this->inventoryNodes[j]->setQuantity(quantityI);
                    this->inventoryNodes[i]->removeBaseItem();
                    
                }
            }
        }
        for (int i = 0; i < this->inventoryNodes.size(); i++) {
            for (int j = i + 1; j < this->inventoryNodes.size(); j++) {
                if (this->inventoryNodes[i]->getStatus() == "free" && (this->inventoryNodes[j]->getBaseEquipment() || this->inventoryNodes[j]->getBaseItem())) {
                    if (this->inventoryNodes[j]->getBaseEquipment()) {
                        this->inventoryNodes[i]->removeBaseEquipment();
                        this->inventoryNodes[i]->setBaseEquipment(this->inventoryNodes[j]->getBaseEquipment()->getEquipmentName(), this->inventoryNodes[j]->getBaseEquipment()->getLevel());
                        this->inventoryNodes[i]->setQuantity(this->inventoryNodes[j]->getQuantity());
                        this->inventoryNodes[j]->removeBaseEquipment();
                    }
                    else if (this->inventoryNodes[j]->getBaseItem()) {
                        this->inventoryNodes[i]->removeBaseItem();
                        this->inventoryNodes[i]->setBaseItem(this->inventoryNodes[j]->getBaseItem()->getItemName());
                        this->inventoryNodes[i]->setQuantity(this->inventoryNodes[j]->getQuantity());
                        this->inventoryNodes[j]->removeBaseItem();
                    }
                }
                
                
            }
        }
        for (int i = 0; i < this->inventoryNodes.size(); i++) {
            for (int j = i + 1; j < this->inventoryNodes.size(); j++) {
                if (this->inventoryNodes[i]->getBaseEquipment() && this->inventoryNodes[j]->getBaseEquipment()) {
                    auto typeI = this->inventoryNodes[i]->getBaseEquipment()->getEquipmentType();
                    auto typeJ = this->inventoryNodes[j]->getBaseEquipment()->getEquipmentType();

                    if (typeI < typeJ) {
                        std::string nameI = this->inventoryNodes[i]->getBaseEquipment()->getEquipmentName();
                        int levelI = this->inventoryNodes[i]->getBaseEquipment()->getLevel();
                        int quantityI = this->inventoryNodes[i]->getQuantity();

                        std::string nameJ = this->inventoryNodes[j]->getBaseEquipment()->getEquipmentName();
                        int levelJ = this->inventoryNodes[j]->getBaseEquipment()->getLevel();
                        int quantityJ = this->inventoryNodes[j]->getQuantity();


                        this->inventoryNodes[i]->removeBaseEquipment();
                        if (this->inventoryNodes[i]->getStatus() == "free") {
                            this->inventoryNodes[i]->removeBaseEquipment();
                            this->inventoryNodes[i]->setBaseEquipment(nameJ, levelJ);
                            this->inventoryNodes[i]->setQuantity(quantityJ);
                        }

                        this->inventoryNodes[j]->removeBaseEquipment();
                        if (this->inventoryNodes[j]->getStatus() == "free") {
                            this->inventoryNodes[j]->removeBaseEquipment();
                            this->inventoryNodes[j]->setBaseEquipment(nameI, levelI);
                            this->inventoryNodes[j]->setQuantity(quantityI);
                        }
                    }
                }
                else if (this->inventoryNodes[i]->getBaseItem() && this->inventoryNodes[j]->getBaseItem()) {
                    auto typeI = this->inventoryNodes[i]->getBaseItem()->getItemName();
                    auto typeJ = this->inventoryNodes[j]->getBaseItem()->getItemName();

                    if (typeJ.compare(typeI) > 0) {
                        std::string nameI = this->inventoryNodes[i]->getBaseItem()->getItemName();
                        int quantityI = this->inventoryNodes[i]->getQuantity();

                        std::string nameJ = this->inventoryNodes[j]->getBaseItem()->getItemName();
                        int quantityJ = this->inventoryNodes[j]->getQuantity();


                        this->inventoryNodes[i]->removeBaseItem();
                        if (this->inventoryNodes[i]->getStatus() == "free") {
                            this->inventoryNodes[i]->setBaseItem(nameJ);
                            this->inventoryNodes[i]->setQuantity(quantityJ);
                        }

                        this->inventoryNodes[j]->removeBaseItem();
                        if (this->inventoryNodes[j]->getStatus() == "free") {
                            this->inventoryNodes[j]->setBaseItem(nameI);
                            this->inventoryNodes[j]->setQuantity(quantityI);
                        }
                    }
                }
            }
        }
        isSorting = false;
        isSorted = true;
    }
}

void Inventory::addItemToItemNodes(std::string itemName, int quantity) {
    isSorted = false;
    for (int i = 0; i < listItemNodes.size(); i++) {
        if (listItemNodes[i]->getStatus() == "free") {
            listItemNodes[i]->setBaseItem(itemName);
            listItemNodes[i]->setQuantity(quantity);
            break;
        }
    }
    
}

void Inventory::removeItemFromItemNodes(std::string itemName) {
    for (int i = 0; i < listItemNodes.size(); i++) {
        auto bi = listItemNodes[i]->getBaseItem();
        if (bi && bi->getItemName() == itemName) {
            listItemNodes[i]->removeBaseItem();
            listItemNodes[i]->setQuantity(0);
            break;
        }
    }
}

void Inventory::decreaseItemQuantity(std::string itemName, int quantity) {
    for (auto i : listItemNodes) {
        if (i->getBaseItem()->getItemName() == itemName) {
            i->decreaseBaseItem(quantity);
        }
    }
}

void Inventory::equipWeapon(std::string name, int level) {
    isSorted = false;
    weaponNode->setBaseEquipment(name, level);
    weaponNode->setQuantity(1);
    for (auto i : inventoryNodes) {
        if (i->getBaseEquipment() && i->getBaseEquipment()->getEquipmentName() == name) {
            i->removeBaseEquipment();
            return;
        }
    }
}
void Inventory::equipShoe(std::string name, int level) {
    isSorted = false;
    shoeNode->setBaseEquipment(name, level);
    shoeNode->setQuantity(1);
    for (auto i : inventoryNodes) {
        if (i->getBaseEquipment() && i->getBaseEquipment()->getEquipmentName() == name) {
            i->removeBaseEquipment();
            return;
        }
    }
}
void Inventory::equipArmor(std::string name, int level) {
    isSorted = false;
    armorNode->setBaseEquipment(name, level);
    armorNode->setQuantity(1);
    for (auto i : inventoryNodes) {
        if (i->getBaseEquipment() && i->getBaseEquipment()->getEquipmentName() == name) {
            i->removeBaseEquipment();
            return;
        }
    }
}
void Inventory::equipRing(std::string name, int level) {
    isSorted = false;
    ringNode->setBaseEquipment(name, level);
    ringNode->setQuantity(1);
    for (auto i : inventoryNodes) {
        if (i->getBaseEquipment() && i->getBaseEquipment()->getEquipmentName() == name) {
            i->removeBaseEquipment();
            return;
        }
    }
}
void Inventory::equipShield(std::string name, int level) {
    isSorted = false;
    shieldNode->setBaseEquipment(name, level);
    shieldNode->setQuantity(1);
    for (auto i : inventoryNodes) {
        if (i->getBaseEquipment() && i->getBaseEquipment()->getEquipmentName() == name) {
            i->removeBaseEquipment();
            return;
        }
    }
}
void Inventory::equipHelmet(std::string name, int level) {
    isSorted = false;
    helmetNode->setBaseEquipment(name, level);
    helmetNode->setQuantity(1);
    for (auto i : inventoryNodes) {
        if (i->getBaseEquipment() && i->getBaseEquipment()->getEquipmentName() == name) {
            i->removeBaseEquipment();
            return;
        }
    }
}

void Inventory::showEquipmentDetails(BaseEquipment* equipment, bool isPlayerUsing) {
    isSorted = false;
    overlay->setVisible(true);
    if (UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0) == 3) {
        inventoryNodes[0]->getNodeSprite()->setTexture("res/inventory-node.png");
        TutorialManager::getInstance()->nextTutorial();
    }

    std::string path = equipment->getTexture()->getPath();
    std::string name = equipment->getEquipmentName();
    if (equipment->equipmentDetails == nullptr) {
        std::string equipmentStats = "";
        if (equipment->getDamage() > 0) { 
            if (equipmentStats != "") {
                equipmentStats += "\ndmg: " + std::to_string(static_cast<int>(equipment->getDamage()));
            }
            else {
                equipmentStats += "dmg: " + std::to_string(static_cast<int>(equipment->getDamage()));
            }
        }
        if (equipment->getSkillDamage() > 0) {
            if (equipmentStats != "") {
                equipmentStats += "\nskill dmg: " + std::to_string(static_cast<int>(equipment->getSkillDamage()));
            }
            else {
                equipmentStats += "skill dmg: " + std::to_string(static_cast<int>(equipment->getSkillDamage()));
            }
        }
        if (equipment->getArmor() > 0) {
            if (equipmentStats != "") {
                equipmentStats += "\narmor: " + std::to_string(static_cast<int>(equipment->getArmor()));
            }
            else {
                equipmentStats += "armor: " + std::to_string(static_cast<int>(equipment->getArmor()));
            }
        }
        if (equipment->getMovementSpeed() > 0) {
            if (equipmentStats != "") {
                equipmentStats += "\nmovement speed: " + std::to_string(static_cast<int>(equipment->getMovementSpeed()));
            }
            else {
                equipmentStats += "movement speed: " + std::to_string(static_cast<int>(equipment->getMovementSpeed()));
            }
        }
        if (equipment->getHP() > 0) {
            if (equipmentStats != "") {
                equipmentStats += "\nHP: " + std::to_string(static_cast<int>(equipment->getHP()));
            }
            else {
                equipmentStats += "HP: " + std::to_string(static_cast<int>(equipment->getHP()));
            }
        }
        if (equipment->getMP() > 0) {
            if (equipmentStats != "") {
                equipmentStats += "\nMP: " + std::to_string(static_cast<int>(equipment->getMP()));
            }
            else {
                equipmentStats += "MP: " + std::to_string(static_cast<int>(equipment->getMP()));
            }
        }
        if (equipment->getCDR() > 0) {
            if (equipmentStats != "") {
                equipmentStats += "\nskill cooldown: " + std::to_string(static_cast<int>(equipment->getCDR()));
            }
            else {
                equipmentStats += "skill cooldown: " + std::to_string(static_cast<int>(equipment->getCDR()));
            }
        }
        equipment->equipmentDetails = EquipmentDetails::create(path, name, equipment->getEquipmentDesc(), std::to_string(equipment->getLevel()), equipmentStats, equipment->getTypeName(), equipment->getLevel(), isPlayerUsing, false, equipment->getEquipmentType(), equipment->getElement() == BaseEquipment::Element::FIRE ? "Fire" : equipment->getElement() == BaseEquipment::Element::THUNDER ? "Thunder" : "None");
        inventoryBorder->addChild(equipment->equipmentDetails, 2000);
        equipment->equipmentDetails->setScale(0.5 * Director::getInstance()->getContentScaleFactor());
    }
    auto ePos = Vec2(currentNode->getBoundingNode().getMidX(), currentNode->getBoundingNode().getMidY());
    auto eSize = currentNode->getBoundingNode().size / 2;
    Vec2 equipmentPos = ePos + Vec2(equipment->equipmentDetails->background->getContentSize().width / 2 - eSize.width, -equipment->equipmentDetails->background->getContentSize().height  / 2 + eSize.height);
    if (equipmentPos.x + equipment->equipmentDetails->background->getContentSize().width  / 2 > buttonClose->getPosition().x) {
        equipmentPos.x = ePos.x - Vec2(equipment->equipmentDetails->background->getContentSize().width  / 2 + eSize.width, -equipment->equipmentDetails->background->getContentSize().height  / 2).x;
    }
    if (equipmentPos.y - equipment->equipmentDetails->background->getContentSize().height  / 2 > buttonClose->getPosition().y) {
        equipmentPos.y = ePos.y - Vec2(equipment->equipmentDetails->background->getContentSize().width  / 2, -equipment->equipmentDetails->background->getContentSize().height / 2).y;
    }
    if (equipmentPos.y + equipment->equipmentDetails->background->getContentSize().height / 2 < buttonClose->getPosition().y - equipment->equipmentDetails->background->getContentSize().height / 2) {
        equipmentPos.y = ePos.y + Vec2(-equipment->equipmentDetails->background->getContentSize().width  / 2, equipment->equipmentDetails->background->getContentSize().height * 0.6 / 2).y;
    }
    equipment->equipmentDetails->setInventory(this);
    equipment->equipmentDetails->setPosition(equipmentPos);
    equipment->equipmentDetails->show();
}

void Inventory::showItemDetails(BaseItem* item) {
    isSorted = false;
    overlay->setVisible(true);
    std::string path = item->getItemSprite()->getTexture()->getPath();
    std::string name = item->getItemName();
    if (item->itemDetails == nullptr) {
        item->itemDetails = ItemDetails::create(path, name, item->getItemDesc(), item->getItemType(), "", false, movingItemQuantity, false, item->getItemLevel());
        item->itemDetails->setSubInventory(_subInventory);
        item->itemDetails->setInventory(this);
        item->itemDetails->setScale(0.5 * Director::getInstance()->getContentScaleFactor());
        inventoryBorder->addChild(item->itemDetails, 2000);
    }
    auto iPos = Vec2(currentNode->getBoundingNode().getMidX(), currentNode->getBoundingNode().getMidY());
    auto iSize = currentNode->getBoundingNode().size / 2;
    Vec2 itemPos = iPos + Vec2(item->itemDetails->background->getContentSize().width  / 2 - iSize.width, -item->itemDetails->background->getContentSize().height  / 2 + iSize.height);
    if (itemPos.x + item->itemDetails->background->getContentSize().width  / 2 > buttonClose->getPosition().x) {
        itemPos.x = iPos.x - Vec2(item->itemDetails->background->getContentSize().width  / 2 + iSize.width, -item->itemDetails->background->getContentSize().height  / 2).x;
    }
    if (itemPos.y - item->itemDetails->background->getContentSize().height  / 2 > buttonClose->getPosition().y) {
        itemPos.y = iPos.y - Vec2(item->itemDetails->background->getContentSize().width  / 2, -item->itemDetails->background->getContentSize().height  / 2).y;
    }
    if (itemPos.y + item->itemDetails->background->getContentSize().height / 2 < buttonClose->getPosition().y - item->itemDetails->background->getContentSize().height / 2) {
        itemPos.y = iPos.y + Vec2(-item->itemDetails->background->getContentSize().width / 2, item->itemDetails->background->getContentSize().height * 0.6 / 2).y;
    }
    item->itemDetails->setPosition(itemPos);
    item->itemDetails->show();
}

void Inventory::showItemDetails(BaseItem* item, bool isInQuickInven) {
    isSorted = false;
    overlay->setVisible(true);
    std::string path = item->getItemSprite()->getTexture()->getPath();
    std::string name = item->getItemName();
    if (item->itemDetails == nullptr) {
        item->itemDetails = ItemDetails::create(path, name, "", item->getItemType(), "", false, movingItemQuantity, false, item->getItemLevel(), isInQuickInven);
        item->itemDetails->setSubInventory(_subInventory);
        item->itemDetails->setInventory(this);
        item->itemDetails->setScale(0.5 * Director::getInstance()->getContentScaleFactor());
        inventoryBorder->addChild(item->itemDetails, 2000);
    }
    auto iPos = Vec2(currentNode->getBoundingNode().getMidX(), currentNode->getBoundingNode().getMidY());
    auto iSize = currentNode->getBoundingNode().size / 2;
    Vec2 itemPos = iPos + Vec2(item->itemDetails->background->getContentSize().width  / 2 - iSize.width, -item->itemDetails->background->getContentSize().height  / 2 + iSize.height);
    if (itemPos.x + item->itemDetails->background->getContentSize().width  / 2 > buttonClose->getPosition().x) {
        itemPos.x = iPos.x - Vec2(item->itemDetails->background->getContentSize().width  / 2 + iSize.width, -item->itemDetails->background->getContentSize().height  / 2).x;
    }
    if (itemPos.y - item->itemDetails->background->getContentSize().height  / 2 > buttonClose->getPosition().y) {
        itemPos.y = iPos.y - Vec2(item->itemDetails->background->getContentSize().width  / 2, -item->itemDetails->background->getContentSize().height  / 2).y;
    }
    if (itemPos.y + item->itemDetails->background->getContentSize().height / 2 < buttonClose->getPosition().y - item->itemDetails->background->getContentSize().height / 2) {
        itemPos.y = iPos.y + Vec2(-item->itemDetails->background->getContentSize().width / 2, item->itemDetails->background->getContentSize().height * 0.6 / 2).y;
    }
    item->itemDetails->setPosition(itemPos);
    item->itemDetails->show();
}

void Inventory::addEquipment(std::string equipmentName, int equipmentLv, int quantity) {
    isSorted = false;
    for (auto node : inventoryNodes) {
        if (node->getStatus() == "free") {
            node->setBaseEquipment(equipmentName, equipmentLv);
            node->setQuantity(quantity);
            return;
        }
    }
}

void Inventory::addItem(std::string itemName, int quantity) {
    isSorted = false;

    for (auto node : inventoryNodes) {
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
    }
}

void Inventory::dropItem(std::string itemName) {
    isSorted = false;

    for (auto node : inventoryNodes) {
        if (node->getStatus() == "busy" && node->getBaseItem()) {
            if (node->getBaseItem()->getItemName() == itemName) {
                node->removeBaseItem();
                return;
            }
        }
    }
}

void Inventory::dropEquipment(std::string itemName) {
    isSorted = false;

    for (auto node : inventoryNodes) {
        if (node->getStatus() == "busy" && node->getBaseEquipment()) {
            if (node->getBaseEquipment()->getEquipmentName() == itemName) {
                node->removeBaseEquipment();
                return;
            }
        }
    }
}

void Inventory::upgradeEquipment(std::string equipmentName) {
    if (weaponNode->getBaseEquipment() && weaponNode->getBaseEquipment()->getEquipmentName() == equipmentName) {
        weaponNode->getBaseEquipment()->levelUp();
        dmgStat->setString(" DMG: " + std::to_string(static_cast<int>(player->getDamage() + weaponNode->getBaseEquipment()->getDamage())));
        mpStat->setString(" MP: " + std::to_string(static_cast<int>(player->getMP() + weaponNode->getBaseEquipment()->getMP())));
        hpStat->setString(" HP: " + std::to_string(static_cast<int>(player->getHP() + weaponNode->getBaseEquipment()->getHP())));
        msStat->setString(" Movement speed: " + std::to_string(static_cast<int>(player->getMovementSpeed() + weaponNode->getBaseEquipment()->getMovementSpeed())));
        cdrStat->setString(" Skill duration: " + std::to_string(static_cast<int>(player->getCDR() + weaponNode->getBaseEquipment()->getCDR())));
        armStat->setString(" Armor: " + std::to_string(static_cast<int>(player->getArmor() + weaponNode->getBaseEquipment()->getArmor())));
        if (weaponNode->getBaseEquipment() && weaponNode->getBaseEquipment()->getEquipmentName() == equipmentName) {
            auto materialsToUpgrade = weaponNode->getBaseEquipment()->materialsToUpgrade[weaponNode->getBaseEquipment()->getLevel() - 2];
            for (auto j : materialsToUpgrade.materialsItem) {
                std::string name = std::get<0>(j);
                int quantity = std::get<1>(j);
                for (auto k : inventoryNodes) {
                    if (k->getBaseItem() && k->getBaseItem()->getItemName() == name) {
                        k->decreaseBaseItem(quantity);
                    }
                }
            }
        }
        return;
    }
    else if (armorNode->getBaseEquipment() && armorNode->getBaseEquipment()->getEquipmentName() == equipmentName) {
        armorNode->getBaseEquipment()->levelUp();
        dmgStat->setString(" DMG: " + std::to_string(static_cast<int>(player->getDamage() + armorNode->getBaseEquipment()->getDamage())));
        mpStat->setString(" MP: " + std::to_string(static_cast<int>(player->getMP() + armorNode->getBaseEquipment()->getMP())));
        hpStat->setString(" HP: " + std::to_string(static_cast<int>(player->getHP() + armorNode->getBaseEquipment()->getHP())));
        msStat->setString(" Movement speed: " + std::to_string(static_cast<int>(player->getMovementSpeed() + weaponNode->getBaseEquipment()->getMovementSpeed())));
        cdrStat->setString(" Skill duration: " + std::to_string(static_cast<int>(player->getCDR() + armorNode->getBaseEquipment()->getCDR())));
        armStat->setString(" Armor: " + std::to_string(static_cast<int>(player->getArmor() + armorNode->getBaseEquipment()->getArmor())));
        if (armorNode->getBaseEquipment() && armorNode->getBaseEquipment()->getEquipmentName() == equipmentName) {
            auto materialsToUpgrade = armorNode->getBaseEquipment()->materialsToUpgrade[armorNode->getBaseEquipment()->getLevel() - 2];
            for (auto j : materialsToUpgrade.materialsItem) {
                std::string name = std::get<0>(j);
                int quantity = std::get<1>(j);
                for (auto k : inventoryNodes) {
                    if (k->getBaseItem() && k->getBaseItem()->getItemName() == name) {
                        k->decreaseBaseItem(quantity);
                    }
                }
            }
        }
        return;
    }
    else if (shoeNode->getBaseEquipment() && shoeNode->getBaseEquipment()->getEquipmentName() == equipmentName) {
        shoeNode->getBaseEquipment()->levelUp();
        dmgStat->setString(" DMG: " + std::to_string(static_cast<int>(player->getDamage() + shoeNode->getBaseEquipment()->getDamage())));
        mpStat->setString(" MP: " + std::to_string(static_cast<int>(player->getMP() + shoeNode->getBaseEquipment()->getMP())));
        hpStat->setString(" HP: " + std::to_string(static_cast<int>(player->getHP() + shoeNode->getBaseEquipment()->getHP())));
        msStat->setString(" Movement speed: " + std::to_string(static_cast<int>(player->getMovementSpeed() + shoeNode->getBaseEquipment()->getMovementSpeed())));
        cdrStat->setString(" Skill duration: " + std::to_string(static_cast<int>(player->getCDR() + shoeNode->getBaseEquipment()->getCDR())));
        armStat->setString(" Armor: " + std::to_string(static_cast<int>(player->getArmor() + shoeNode->getBaseEquipment()->getArmor())));
        if (shoeNode->getBaseEquipment() && shoeNode->getBaseEquipment()->getEquipmentName() == equipmentName) {
            auto materialsToUpgrade = shoeNode->getBaseEquipment()->materialsToUpgrade[shoeNode->getBaseEquipment()->getLevel() - 2];
            for (auto j : materialsToUpgrade.materialsItem) {
                std::string name = std::get<0>(j);
                int quantity = std::get<1>(j);
                for (auto k : inventoryNodes) {
                    if (k->getBaseItem() && k->getBaseItem()->getItemName() == name) {
                        k->decreaseBaseItem(quantity);
                    }
                }
            }
        }
        return;
    }
    else if (helmetNode->getBaseEquipment() && helmetNode->getBaseEquipment()->getEquipmentName() == equipmentName) {
        helmetNode->getBaseEquipment()->levelUp();
        dmgStat->setString(" DMG: " + std::to_string(static_cast<int>(player->getDamage() + helmetNode->getBaseEquipment()->getDamage())));
        mpStat->setString(" MP: " + std::to_string(static_cast<int>(player->getMP() + helmetNode->getBaseEquipment()->getMP())));
        hpStat->setString(" HP: " + std::to_string(static_cast<int>(player->getHP() + helmetNode->getBaseEquipment()->getHP())));
        msStat->setString(" Movement speed: " + std::to_string(static_cast<int>(player->getMovementSpeed() + helmetNode->getBaseEquipment()->getMovementSpeed())));
        cdrStat->setString(" Skill duration: " + std::to_string(static_cast<int>(player->getCDR() + helmetNode->getBaseEquipment()->getCDR())));
        armStat->setString(" Armor: " + std::to_string(static_cast<int>(player->getArmor() + helmetNode->getBaseEquipment()->getArmor())));
        if (helmetNode->getBaseEquipment() && helmetNode->getBaseEquipment()->getEquipmentName() == equipmentName) {
            auto materialsToUpgrade = helmetNode->getBaseEquipment()->materialsToUpgrade[helmetNode->getBaseEquipment()->getLevel() - 2];
            for (auto j : materialsToUpgrade.materialsItem) {
                std::string name = std::get<0>(j);
                int quantity = std::get<1>(j);
                for (auto k : inventoryNodes) {
                    if (k->getBaseItem() && k->getBaseItem()->getItemName() == name) {
                        k->decreaseBaseItem(quantity);
                    }
                }
            }
        }
        return;
    }
    else if (ringNode->getBaseEquipment() && ringNode->getBaseEquipment()->getEquipmentName() == equipmentName) {
        ringNode->getBaseEquipment()->levelUp();
        dmgStat->setString(" DMG: " + std::to_string(static_cast<int>(player->getDamage() + ringNode->getBaseEquipment()->getDamage())));
        mpStat->setString(" MP: " + std::to_string(static_cast<int>(player->getMP() + ringNode->getBaseEquipment()->getMP())));
        hpStat->setString(" HP: " + std::to_string(static_cast<int>(player->getHP() + ringNode->getBaseEquipment()->getHP())));
        msStat->setString(" Movement speed: " + std::to_string(static_cast<int>(player->getMovementSpeed() + ringNode->getBaseEquipment()->getMovementSpeed())));
        cdrStat->setString(" Skill duration: " + std::to_string(static_cast<int>(player->getCDR() + ringNode->getBaseEquipment()->getCDR())));
        armStat->setString(" Armor: " + std::to_string(static_cast<int>(player->getArmor() + ringNode->getBaseEquipment()->getArmor())));
        if (ringNode->getBaseEquipment() && ringNode->getBaseEquipment()->getEquipmentName() == equipmentName) {
            auto materialsToUpgrade = ringNode->getBaseEquipment()->materialsToUpgrade[ringNode->getBaseEquipment()->getLevel() - 2];
            for (auto j : materialsToUpgrade.materialsItem) {
                std::string name = std::get<0>(j);
                int quantity = std::get<1>(j);
                for (auto k : inventoryNodes) {
                    if (k->getBaseItem() && k->getBaseItem()->getItemName() == name) {
                        k->decreaseBaseItem(quantity);
                    }
                }
            }
        }
        return;
    }
    else if (shieldNode->getBaseEquipment() && shieldNode->getBaseEquipment()->getEquipmentName() == equipmentName) {
        shieldNode->getBaseEquipment()->levelUp();
        dmgStat->setString(" DMG: " + std::to_string(static_cast<int>(player->getDamage() + shieldNode->getBaseEquipment()->getDamage())));
        mpStat->setString(" MP: " + std::to_string(static_cast<int>(player->getMP() + shieldNode->getBaseEquipment()->getMP())));
        hpStat->setString(" HP: " + std::to_string(static_cast<int>(player->getHP() + shieldNode->getBaseEquipment()->getHP())));
        msStat->setString(" Movement speed: " + std::to_string(static_cast<int>(player->getMovementSpeed() + shieldNode->getBaseEquipment()->getMovementSpeed())));
        cdrStat->setString(" Skill duration: " + std::to_string(static_cast<int>(player->getCDR() + shieldNode->getBaseEquipment()->getCDR())));
        armStat->setString(" Armor: " + std::to_string(static_cast<int>(player->getArmor() + shieldNode->getBaseEquipment()->getArmor())));
        if (shieldNode->getBaseEquipment() && shieldNode->getBaseEquipment()->getEquipmentName() == equipmentName) {
            auto materialsToUpgrade = shieldNode->getBaseEquipment()->materialsToUpgrade[shieldNode->getBaseEquipment()->getLevel() - 2];
            for (auto j : materialsToUpgrade.materialsItem) {
                std::string name = std::get<0>(j);
                int quantity = std::get<1>(j);
                for (auto k : inventoryNodes) {
                    if (k->getBaseItem() && k->getBaseItem()->getItemName() == name) {
                        k->decreaseBaseItem(quantity);
                    }
                }
            }
        }
        return;
    }
    for (auto node : inventoryNodes) {
        if (node) {
            auto baseEquipmentI = node->getBaseEquipment();
            if (baseEquipmentI && baseEquipmentI->getEquipmentName() == equipmentName) {
                auto& materialsToUpgrade = baseEquipmentI->materialsToUpgrade[baseEquipmentI->getLevel() - 1];
                for (auto j : materialsToUpgrade.materialsItem) {
                    std::string name = std::get<0>(j);
                    int quantity = std::get<1>(j);
                    for (auto k : inventoryNodes) {
                        if (k->getBaseItem() && k->getBaseItem()->getItemName() == name) {
                            k->decreaseBaseItem(quantity);
                        }
                    }
                }
            }
        }
    }
    for (auto node : inventoryNodes) {
        if (node->getBaseEquipment() && node->getBaseEquipment()->getEquipmentName() == equipmentName) {
            node->getBaseEquipment()->levelUp();
            break;
        }
    }
}

std::vector<std::string> Inventory::getAllItem() {
    std::vector<std::string> result;
    for (auto i : inventoryNodes) {
        if (i->getStatus() == "busy" && i->getBaseItem() != nullptr) {
            int count = 0;
            while (count < i->getQuantity()) {
                result.push_back(i->getBaseItem()->getItemName());
                count++;
            }
        }
    }
    return result;
}
std::vector<BaseEquipment::EquipmentData> Inventory::getAllEquipment() {
    std::vector<BaseEquipment::EquipmentData> result;
    for (auto i : inventoryNodes) {
        if (i->getBaseEquipment() && i->getStatus() == "busy" && i->getBaseEquipment() != nullptr) {
            BaseEquipment::EquipmentData equipmentData;
            equipmentData.name = i->getBaseEquipment()->getEquipmentName();
            equipmentData.level = i->getBaseEquipment()->getLevel();
            equipmentData.iconPath = i->getBaseEquipment()->getTexture()->getPath();
            result.push_back(equipmentData);
        }
    }
    return result;
}

void Inventory::removeEquipment(std::string equipmentName) {
    isSorted = false;
    for (auto node : inventoryNodes) {
        if (node->getStatus() == "busy") {
            if (node->getBaseEquipment() && node->getBaseEquipment()->getEquipmentName() == equipmentName) {
                node->removeBaseEquipment();
                return;
            }
        }
    }
}

void Inventory::removeItem(std::string itemName) {
    isSorted = false;
    for (auto node : inventoryNodes) {
        if (node->getStatus() == "busy") {
            if (node->getBaseItem() && node->getBaseItem()->getItemName() == itemName) {
                node->removeBaseItem();
                return;
            }
        }
    }
}


std::pair<std::string, int> Inventory::getItem(std::string itemName) {
    for (auto i : inventoryNodes) {
        if (i->getBaseItem()->getItemName() == itemName) {
            return std::make_pair(itemName, i->getQuantity());
        }
    }
    return std::make_pair("", 0);
}

void Inventory::showInventory() {
    inventoryBorder->setVisible(true);
    isSo = true;
    level->setString("Knight - Level " + std::to_string(player->getLevel()));
    if (TutorialManager::getInstance()->currentTutorial == 2) {
        inventoryNodes[0]->getNodeSprite()->setTexture("res/inventory-node-press.png");
    }
}

void Inventory::hideInventory() {
    inventoryBorder->setVisible(false);
    isSo = false;
}