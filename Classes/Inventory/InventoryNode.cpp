#include "InventoryNode.h"
#include "NotificationManager/NotificationManager.h"
InventoryNode* InventoryNode::createInventoryNode(float size)
{
    auto inventoryNode = new (std::nothrow) InventoryNode();
    if (inventoryNode && inventoryNode->init(size))
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

bool InventoryNode::init(float size)
{
    if (!Node::init())
    {
        return false;
    }
    this->setContentSize(Size(size, size));
    this->status = "free";
    nodeSpr = Sprite::create("res/inventory-node.png");
    nodeSpr->retain();
    nodeScaleX = size / nodeSpr->getContentSize().width;
    nodeScaleY = size / nodeSpr->getContentSize().height;
    nodeSpr->setScale(nodeScaleX, nodeScaleY);
    nodeSpr->setAnchorPoint(Vec2(0, 0));
    nodeSpr->setPosition(size / (nodeSpr->getContentSize().width / 2), size / (nodeSpr->getContentSize().height / 2));
    this->addChild(nodeSpr);
 
    quantityLabel = Label::createWithTTF("0", "fonts/Diablo Light.ttf", 10);
    quantityLabel->setPosition(nodeSpr->getContentSize().width * nodeScaleX, nodeSpr->getContentSize().height * nodeScaleY);
    this->addChild(quantityLabel);
    quantityLabel->setVisible(false);
    this->retain();
    return true;
}

void InventoryNode::hide() {
    this->setVisible(false);
    if (baseItem) baseItem->setVisible(false);
    if (baseEquipment) baseEquipment->setVisible(false);
    if (quantityLabel) quantityLabel->setVisible(false);
}

void InventoryNode::show() {
    this->setVisible(true);
    if (baseItem) baseItem->setVisible(true);
    if (baseEquipment) baseEquipment->setVisible(true);
    if (getQuantity() != 0 && !baseEquipment && quantityLabel) quantityLabel->setVisible(true);
}

void InventoryNode::setId(int id)
{
    this->id = id;
}

int InventoryNode::getId() const
{
    return this->id;
}

Rect InventoryNode::getBoundingNode() const
{
    return this->getBoundingBox();
}
Rect InventoryNode::getBoundingNode(float amt) const
{
    Rect rs = Rect(getPosition(), Size(this->getContentSize() * 0.6));
    return rs;
}

bool InventoryNode::setBaseEquipment(std::string be, int equipmentLv)
{
    if (this->getStatus() != "busy")
    {
        this->baseEquipment = EquipmentFactory::createEquipment(be, equipmentLv);
        if (this->baseEquipment)
        {
            if (this->baseEquipment->getEquipmentType() == BaseEquipment::Type::STANDARD)
            {
                nodeSpr->setTexture("res/inventory-node-black.png");
            }
            else if (this->baseEquipment->getEquipmentType() == BaseEquipment::Type::ENHANCED)
            {
                nodeSpr->setTexture("res/inventory-node-green.png");
            }
            else if (this->baseEquipment->getEquipmentType() == BaseEquipment::Type::RARE)
            {
                nodeSpr->setTexture("res/inventory-node-purple.png");
            }
            else if (this->baseEquipment->getEquipmentType() == BaseEquipment::Type::LEGENDARY)
            {
                nodeSpr->setTexture("res/inventory-node-orange.png");
            }
            this->setStatus("busy");
            float equipmentScaleX = nodeSpr->getContentSize().width * (nodeScaleX * 0.5) / this->baseEquipment->getContentSize().width;
            float equipmentScaleY = nodeSpr->getContentSize().height * (nodeScaleY * 0.5) / this->baseEquipment->getContentSize().height;
            this->baseEquipment->setAnchorPoint(Vec2(0, 0));
            this->baseEquipment->setScale(equipmentScaleX, equipmentScaleY);
            this->baseEquipment->setPosition(Vec2(nodeSpr->getContentSize().width * equipmentScaleX / 2, nodeSpr->getContentSize().height * equipmentScaleY / 2));
            this->setQuantity(0);
            this->addChild(this->baseEquipment);
        }
        return true;
    }
    return false;
}


bool InventoryNode::setBaseItem(std::string be)
{
    if (be == "") return false;
    if (this->getStatus() == "busy")
    {
        this->removeBaseItem();
    }
    this->baseItem = ItemFactory::createItem(be);
    if (this->baseItem)
    {
        this->setStatus("busy");
        if (_subInven == true) {
            float itemScaleX = nodeSpr->getContentSize().width * (nodeScaleX * 0.5) / this->baseItem->getItemSprite()->getContentSize().width;
            float itemScaleY = nodeSpr->getContentSize().height * (nodeScaleY * 0.5) / this->baseItem->getItemSprite()->getContentSize().height;
            this->baseItem->setScale(itemScaleX * 0.8, itemScaleY * 0.8);
            this->baseItem->setPosition(Vec2(nodeSpr->getContentSize().width / 3, nodeSpr->getContentSize().height / 3) + Vec2(nodeSpr->getContentSize().width, nodeSpr->getContentSize().height) + this->baseItem->getItemSprite()->getContentSize() / 4);
            this->quantityLabel->setScale(1 * Director::getInstance()->getContentScaleFactor());
        }
        else if (_inven == true) {
            float itemScaleX = nodeSpr->getContentSize().width * (nodeScaleX * 0.5) / this->baseItem->getItemSprite()->getContentSize().width * 0.25;
            float itemScaleY = nodeSpr->getContentSize().height * (nodeScaleY * 0.5) / this->baseItem->getItemSprite()->getContentSize().height * 0.25;
            
            this->baseItem->setScale(itemScaleX * 1.2, itemScaleY * 1.2);
            this->baseItem->setPosition(Vec2(nodeSpr->getContentSize().width * itemScaleX / 2, nodeSpr->getContentSize().height * itemScaleY / 2) + this->baseItem->getItemSprite()->getContentSize() * this->baseItem->getItemSprite()->getScale() / 2);

            this->quantityLabel->setScale(0.6 * Director::getInstance()->getContentScaleFactor());
            this->quantityLabel->setPosition((nodeSpr->getContentSize().width * nodeScaleX) / 2, nodeSpr->getContentSize().height * nodeScaleY * 0.55);
        }
        else {
            this->baseItem->setScale(0.6 * Director::getInstance()->getContentScaleFactor());
            this->baseItem->setPosition(Vec2(nodeSpr->getContentSize().width / 3, nodeSpr->getContentSize().height / 3) + Vec2(nodeSpr->getContentSize().width, nodeSpr->getContentSize().height) / 2 + this->baseItem->getItemSprite()->getContentSize() / 4);
            
            this->quantityLabel->setScale(0.5 * Director::getInstance()->getContentScaleFactor());
        }
        if (isMaterialNode) {
            this->baseItem->setScale(0.45 * Director::getInstance()->getContentScaleFactor());
            this->baseItem->setPosition(Vec2(this->getBoundingNode().size.width / 2, this->getBoundingNode().size.height / 2));
            this->quantityLabel->setScale(0.5 * Director::getInstance()->getContentScaleFactor());
            this->quantityLabel->setPosition(this->quantityLabel->getPosition() + Vec2(-this->quantityLabel->getContentSize().width / 6, this->quantityLabel->getContentSize().height / 2));
        }
        this->addChild(this->baseItem);
    }
    return true;
    
}

bool InventoryNode::removeBaseEquipment()
{
    if (this->getQuantity() != 0)
    {
        nodeSpr->setTexture("res/inventory-node.png");
        this->baseEquipment->removeFromParentAndCleanup(true);
        this->baseEquipment = nullptr;
        this->setQuantity(0);
        this->setStatus("free");
        //Color4B(20, 20, 23, 21);
        return true;
    }
    return false;
}

bool InventoryNode::removeBaseItem()
{
    if (!isMaterialNode) {
        if (this->getQuantity() != 0)
        {
            this->baseItem->removeFromParentAndCleanup(true);
            this->baseItem = nullptr;
            this->setQuantity(0);
            this->setStatus("free");
            quantityLabel->setVisible(false);
            return true;
        }
    }
    else {
        if (this->getQuantity() != 0)
        {
            if (this->baseItem) {
                this->baseItem->removeFromParentAndCleanup(true);
                this->baseItem = nullptr;
            }
            this->setStatus("free");
            this->setQuantity(0);
            quantityLabel->setString("0/" + std::to_string(requiredQuantity));
            quantityLabel->setTextColor(Color4B::RED);
        }
        return true;
    }
    return false;
}

void InventoryNode::removeMaterialCondition() {
    if (this->conditionItem && this->conditionItem->getParent()) {
        this->conditionItem->removeFromParentAndCleanup(true);
        this->conditionItem = nullptr;
        quantityLabel->setVisible(false);
        isMaterialNode = false;
    }
}

bool InventoryNode::decreaseBaseEquipment(int quantity)
{
    if (this->quantityItem >= quantity)
    {
        this->quantityItem -= quantity;
        quantityLabel->setString(std::to_string(quantityItem));
        if (this->getQuantity() <= 0)
        {
            nodeSpr->setTexture("res/inventory-node.png");
            this->baseEquipment->removeFromParentAndCleanup(true);
            this->baseEquipment = nullptr;
            quantityLabel->setVisible(false);
            this->setStatus("free");
        }
        return true;
    }
    return false;
}

bool InventoryNode::decreaseBaseItem(int quantity)
{
    if (!isMaterialNode) {
        if (this->quantityItem >= quantity)
        {
            this->quantityItem -= quantity;
            quantityLabel->setString(std::to_string(quantityItem));
            if (this->getQuantity() <= 0 && this->baseItem)
            {
                this->baseItem->removeFromParentAndCleanup(true);
                this->baseItem = nullptr;
                quantityLabel->setVisible(false);
                this->setStatus("free");
            }
            return true;
        }
    }
    else {
        if (this->quantityItem >= quantity)
        {
            this->quantityItem -= quantity;
            NotificationManager::getInstance()->showMessageNotification("\n\n\nDecrease Base Item Success!", Vec2::ZERO, Color3B::ORANGE, 30);
            quantityLabel->setString(std::to_string(quantityItem) + "/" + std::to_string(requiredQuantity));
            if (this->getQuantity() <= 0 && this->baseItem)
            {
                this->baseItem->removeFromParentAndCleanup(true);
                this->baseItem = nullptr;
                quantityLabel->setVisible(false);
                this->setStatus("free");
            }
            return true;
        }
    }
    return false;
}

bool InventoryNode::decreaseBaseItem(std::string itemName, int quantity)
{

    return false;
}



void InventoryNode::setMaterialsToUpgrade(std::string itemName, int itemQuantity) {
    if (itemName == "" || itemQuantity == 0) return;
    if (!isMaterialNode)
    {
        isMaterialNode = true;
        if (this->getStatus() == "free")
        {
            this->conditionItem = ItemFactory::createItem(itemName);
            if (this->conditionItem)
            {
                requiredQuantity = itemQuantity;
                float itemScaleX = nodeSpr->getContentSize().width * (nodeScaleX * 0.28) / this->conditionItem->getItemSprite()->getContentSize().width;
                this->conditionItem->setScale(itemScaleX);
                this->conditionItem->setPosition(Vec2(nodeSpr->getContentSize().width / 3, nodeSpr->getContentSize().height / 3) + Vec2(nodeSpr->getContentSize().width, nodeSpr->getContentSize().height) / 2);
                this->conditionItem->getItemSprite()->setOpacity(100);
                quantityLabel->setPosition(quantityLabel->getPositionX() / 2 + quantityLabel->getContentSize().width / 2, quantityLabel->getPositionY());
                quantityLabel->setString("0/" + std::to_string(requiredQuantity));
                quantityLabel->setTextColor(Color4B::RED);
                quantityLabel->setVisible(true);
                this->addChild(this->conditionItem);
                this->conditionItem->setVisible(false);
            }
            return;
        }
    }
    else {
        requiredQuantity = itemQuantity;
        if (this->conditionItem) this->conditionItem->removeFromParentAndCleanup(true);
        this->conditionItem = ItemFactory::createItem(itemName);
        if (this->conditionItem)
        {
            float itemScaleX = nodeSpr->getContentSize().width * (nodeScaleX * 0.28) / this->conditionItem->getItemSprite()->getContentSize().width;
            float itemScaleY = nodeSpr->getContentSize().height * (nodeScaleY * 0.5) / this->conditionItem->getItemSprite()->getContentSize().height;
            this->conditionItem->setScale(itemScaleX);
            this->conditionItem->setPosition(Vec2(nodeSpr->getContentSize().width / 3, nodeSpr->getContentSize().height / 3) + Vec2(nodeSpr->getContentSize().width, nodeSpr->getContentSize().height) / 2);
            this->conditionItem->getItemSprite()->setOpacity(100);
            quantityLabel->setPosition(quantityLabel->getPositionX() / 2 + quantityLabel->getContentSize().width / 2, quantityLabel->getPositionY());
            quantityLabel->setString(std::to_string(quantityItem) + "/" + std::to_string(requiredQuantity));
            if (quantityItem < requiredQuantity) 
                quantityLabel->setTextColor(Color4B::RED);
            else 
                quantityLabel->setTextColor(Color4B::GREEN);
            
            quantityLabel->setVisible(true);
            this->addChild(this->conditionItem);
            this->conditionItem->setVisible(false);
        }
        return;
    }
    return;
}

BaseItem* InventoryNode::getBaseItemToUpgrade() const {
    return this->conditionItem;
}

BaseEquipment* InventoryNode::getBaseEquipment() const
{
    return this->baseEquipment;
}

BaseItem* InventoryNode::getBaseItem() const
{
    return this->baseItem;
}

void InventoryNode::setStatus(std::string stt)
{
    this->status = stt;
}

std::string InventoryNode::getStatus() const
{
    return this->status;
}

void InventoryNode::setQuantity(int qtt)
{
    this->quantityItem = qtt;
    if (!isMaterialNode) {
        if (this->baseItem) {
            quantityLabel->setVisible(true);
            quantityLabel->setString(std::to_string(quantityItem));
        }
    }
    else {
        if (this->baseItem) {
            quantityLabel->setVisible(true);
            if (quantityItem > requiredQuantity) quantityItem = requiredQuantity;
            quantityLabel->setString(std::to_string(quantityItem) + "/" + std::to_string(requiredQuantity));
            quantityLabel->setScale(0.6);
            if (quantityItem < requiredQuantity) {
                quantityLabel->setTextColor(Color4B::RED);
                checkQuantityRequired = false;
            }
            else {
                checkQuantityRequired = true;
                quantityLabel->setTextColor(Color4B::GREEN);
            }
        }
    }
}

int InventoryNode::getQuantity() const
{
    return this->quantityItem;
}

InventoryNode::~InventoryNode() {
    this->release();
}