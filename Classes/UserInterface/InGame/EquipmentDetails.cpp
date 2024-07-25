#include "EquipmentDetails.h"
#include "Inventory/Inventory.h"
#include "Inventory/UpgradeInventory.h"
#include "Player/Player.h"
#include "Game/Game.h"

EquipmentDetails::EquipmentDetails(const std::string imagePath, const std::string equipmentName, const std::string& description, const std::string& level, const std::string props, const std::string equipmentType, const int equipmentLevel, const bool isPlayerUsing, const bool isInUpgrade, int equipmentTypeId, std::string weaponElement) : _imagePath(imagePath), _equipmentName(equipmentName), _description(description), _level(level), _props(props), _equipmentType(equipmentType), _equipmentLevel(equipmentLevel), _isPlayerUsing(isPlayerUsing), _isInUpgrade(isInUpgrade), _equipmentTypeId(equipmentTypeId), _weaponElement(weaponElement) {}
EquipmentDetails::EquipmentDetails() {}
EquipmentDetails::~EquipmentDetails() {}

EquipmentDetails* EquipmentDetails::create(const std::string imagePath, const std::string equipmentName, const std::string& description, const std::string& level, const std::string props, const std::string equipmentType, const int equipmentLevel, const bool isPlayerUsing, const bool isInUpgrade, int equipmentTypeId, std::string weaponElement) {
	EquipmentDetails* object = new (std::nothrow)EquipmentDetails(imagePath, equipmentName, description, level, props, equipmentType, equipmentLevel, isPlayerUsing, isInUpgrade, equipmentTypeId, weaponElement);
	if (object && object->init()) {
		object->autorelease();
		return object;
	}
	CC_SAFE_DELETE(object);
	return nullptr;
}

bool EquipmentDetails::init() {
	if (!Node::init()) {
		return false;
	}

    auto visibleSize = Director::getInstance()->getVisibleSize();
    background = Sprite::create("res/c_container.png");
    background->retain();
    this->addChild(background, 1);

    Sprite* iconBorder = Sprite::create("res/talent_slot.png");
    iconBorder->setPosition(-(background->getContentSize().width / 2 - iconBorder->getContentSize().width / 2), background->getContentSize().height / 2 - iconBorder->getContentSize().height / 2);
    iconBorder->setScale(0.5 * Director::getInstance()->getContentScaleFactor());
    this->addChild(iconBorder, 2);

    if (_equipmentTypeId == 2) {
        iconBorder->setTexture("res/talent_slot_rare.png");
    }
    else if (_equipmentTypeId == 1) {
        iconBorder->setTexture("res/talent_slot_enhanced.png");
    }
    else if (_equipmentTypeId == 3) {
        iconBorder->setTexture("res/talent_slot_enhanced.png");
    }

    Sprite* icon = Sprite::create(_imagePath);
    icon->setPosition(iconBorder->getPosition() - icon->getContentSize() / 2);
    icon->setAnchorPoint(Vec2(0, 0));
    this->addChild(icon, 2);

    Label* weaponName = Label::createWithTTF(_equipmentName, "fonts/Diablo Light.ttf", 9);
    weaponName->setPosition(20, background->getContentSize().height / 2 - weaponName->getContentSize().height * 0.5 - icon->getContentSize().height);
    this->addChild(weaponName, 2);

    if (_weaponElement == "Fire" || _weaponElement == "Thunder") {
        Label* weaponElement = Label::createWithTTF(_weaponElement == "Fire" ? "Element: Fire" : _weaponElement == "Thunder" ? "Element: Thunder" : "Element: None", "fonts/Diablo Light.ttf", 7);
        _weaponElement == "Fire" ? weaponElement->setColor(Color3B::ORANGE) : _weaponElement == "Thunder" ? weaponElement->setColor(Color3B::MAGENTA) : weaponElement->setColor(Color3B::WHITE);
        weaponElement->setPosition(weaponName->getPosition() + Vec2(0, -weaponName->getContentSize().height * 2));
        this->addChild(weaponElement, 2);
    }

    Label* level = Label::createWithTTF("+" + _level, "fonts/Diablo Light.ttf", 9);
    level->setPosition(weaponName->getPosition() + Vec2(weaponName->getContentSize().width / 2 + 5, 0));
    this->addChild(level, 2);

    Label* props = Label::createWithTTF(_props, "fonts/Diablo Light.ttf", 7);
    props->setPosition(iconBorder->getPosition().x + props->getContentSize().width / 4, icon->getContentSize().height / 2 - 3 * Director::getInstance()->getContentScaleFactor());
    this->addChild(props, 2);

    Label* description = Label::createWithTTF(_description, "fonts/Diablo Light.ttf", 7);
    description->setPosition(background->getPosition() + Vec2(0, -props->getContentSize().height - 12 * Director::getInstance()->getContentScaleFactor()));
    this->addChild(description, 2);

    equipButton = Sprite::create("res/button_gray.png");
    equipButton->setScale(0.28 * Director::getInstance()->getContentScaleFactor());
    Label* equip = Label::createWithTTF("Equip", "fonts/Diablo Light.ttf", 13);
    equipButton->setPosition(background->getPosition() + background->getContentSize() / 2 + Vec2(equipButton->getContentSize().width * 0.28 * Director::getInstance()->getContentScaleFactor() / 2, -background->getContentSize().height / 2 - equipButton->getContentSize().height * 0.2 * Director::getInstance()->getContentScaleFactor()));
    this->addChild(equipButton);
    equip->setPosition(equipButton->getPosition());
    this->addChild(equip);

    if (_isInUpgrade == false) {
        dropButton = Sprite::create("res/button_gray-PUSH.png");
        dropButton->setScale(0.28 * Director::getInstance()->getContentScaleFactor());
        Label* drop = Label::createWithTTF("Drop", "fonts/Diablo Light.ttf", 13);
        dropButton->setPosition(background->getPosition() + background->getContentSize() / 2 + Vec2(equipButton->getContentSize().width * 0.28 * Director::getInstance()->getContentScaleFactor() / 2, -background->getContentSize().height / 2 - equipButton->getContentSize().height * 0.2 * Director::getInstance()->getContentScaleFactor() - 40));
        this->addChild(dropButton);
        drop->setPosition(dropButton->getPosition());
        this->addChild(drop);
    }
    else {
        equip->setString("Remove");
    }

    if (_isPlayerUsing == true) {
        equip->setString("Unequip");
    }


    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [&](Touch* touch, Event* event) {
        if (isVisible()) {
            auto startPoint = this->convertToNodeSpace(touch->getLocation());
            if (background->getBoundingBox().containsPoint(startPoint) || equipButton->getBoundingBox().containsPoint(startPoint) || dropButton && dropButton->getBoundingBox().containsPoint(startPoint)) {
                if (equipButton->getBoundingBox().containsPoint(startPoint)) {
                    UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/equip_item_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
                    if (!_isPlayerUsing) {
                         //equip item
                        if (_upgradeInventory && _isInUpgrade == false) {
                            if (_upgradeInventory->weaponNode->getBaseEquipment()) {
                                _upgradeInventory->addEquipment(_upgradeInventory->weaponNode->getBaseEquipment()->getEquipmentName(), _upgradeInventory->weaponNode->getBaseEquipment()->getLevel(), 1);
                                _upgradeInventory->weaponNode->removeBaseEquipment();
                            }
                            _upgradeInventory->weaponNode->setBaseEquipment(_equipmentName, _equipmentLevel);
                            _upgradeInventory->weaponNode->setQuantity(1);
                            _upgradeInventory->equipment = _upgradeInventory->weaponNode->getBaseEquipment();
                            _upgradeInventory->currentNode->removeBaseEquipment();
                            auto mtrs = _upgradeInventory->weaponNode->getBaseEquipment()->materialsToUpgrade[_upgradeInventory->equipment->getLevel() - 1].materialsItem;
                            int count = 0;
                            for (auto mt : mtrs)
                            {
                                auto name = std::get<0>(mt);
                                auto quan = std::get<1>(mt);
                                _upgradeInventory->materialNodes[count]->setMaterialsToUpgrade(name, quan);
                                count++;
                            }
                            _upgradeInventory->upgradeProgressBar->updateProgress((_upgradeInventory->weaponNode->getBaseEquipment()->getLevel() - 1) * 20, 100);
                            _upgradeInventory->dmgStat->setString(" DMG: " + std::to_string(static_cast<int>(_upgradeInventory->weaponNode->getBaseEquipment()->getDamage())));
                            _upgradeInventory->mpStat->setString(" MP: " + std::to_string(static_cast<int>(_upgradeInventory->weaponNode->getBaseEquipment()->getMP())));
                            _upgradeInventory->hpStat->setString(" HP: " + std::to_string(static_cast<int>(_upgradeInventory->weaponNode->getBaseEquipment()->getHP())));
                            _upgradeInventory->msStat->setString(" Movement speed: " + std::to_string(static_cast<int>(_upgradeInventory->weaponNode->getBaseEquipment()->getMovementSpeed())));
                            _upgradeInventory->cdrStat->setString(" Skill duration: " + std::to_string(static_cast<int>(_upgradeInventory->weaponNode->getBaseEquipment()->getCDR())));
                            _upgradeInventory->armStat->setString(" Armor: " + std::to_string(static_cast<int>(_upgradeInventory->weaponNode->getBaseEquipment()->getArmor())));
                            _upgradeInventory->equipment = nullptr;
                        }
                        if (_isInUpgrade) {
                            _upgradeInventory->upgradeProgressBar->updateProgress(0, 100);
                            for (auto i : _upgradeInventory->materialNodes) {
                                if (i->getBaseItem()) {
                                    _upgradeInventory->addItem(i->getBaseItem()->getItemName(), i->getQuantity());
                                    i->removeBaseItem();
                                }
                                i->removeMaterialCondition();
                            }
                            _upgradeInventory->addEquipment(_upgradeInventory->weaponNode->getBaseEquipment()->getEquipmentName(), _upgradeInventory->weaponNode->getBaseEquipment()->getLevel(), 1);
                            _upgradeInventory->weaponNode->removeBaseEquipment();
                            _upgradeInventory->dmgStat->setString(" DMG  ");
                            _upgradeInventory->mpStat->setString(" MP  ");
                            _upgradeInventory->hpStat->setString(" HP  ");
                            _upgradeInventory->msStat->setString(" Movement speed  ");
                            _upgradeInventory->cdrStat->setString(" Skill duration  ");
                            _upgradeInventory->armStat->setString(" Armor  ");
                        }
                        if (_inventory) {
                            if (_equipmentType == "Weapon") {
                                if (_inventory->weaponNode->getBaseEquipment()) {
                                    _inventory->addEquipment(_inventory->weaponNode->getBaseEquipment()->getEquipmentName(), _inventory->weaponNode->getBaseEquipment()->getLevel(), 1);
                                    _inventory->weaponNode->removeBaseEquipment();
                                    _inventory->player->removeEquipment("Weapon");
                                }
                                _inventory->weaponNode->setBaseEquipment(_equipmentName, _equipmentLevel);
                                _inventory->weaponNode->setQuantity(1);
                                _inventory->equipment = _inventory->weaponNode->getBaseEquipment();
                                _inventory->player->setEquipment(_equipmentName, _equipmentLevel, "Weapon");
                                _inventory->currentNode->removeBaseEquipment();
                            }
                            else if (_equipmentType == "Shoe") {
                                if (_inventory->shoeNode->getBaseEquipment()) {
                                    _inventory->addEquipment(_inventory->shoeNode->getBaseEquipment()->getEquipmentName(), _inventory->shoeNode->getBaseEquipment()->getLevel(), 1);
                                    _inventory->shoeNode->removeBaseEquipment();
                                    _inventory->player->removeEquipment("Shoe");
                                }
                                _inventory->shoeNode->setBaseEquipment(_equipmentName, _equipmentLevel);
                                _inventory->shoeNode->setQuantity(1);
                                _inventory->equipment = _inventory->weaponNode->getBaseEquipment();
                                _inventory->player->setEquipment(_equipmentName, _equipmentLevel, "Shoe");
                                _inventory->currentNode->removeBaseEquipment();
                            }
                            else if (_equipmentType == "Ring") {
                                if (_inventory->ringNode->getBaseEquipment()) {
                                    _inventory->addEquipment(_inventory->ringNode->getBaseEquipment()->getEquipmentName(), _inventory->ringNode->getBaseEquipment()->getLevel(), 1);
                                    _inventory->ringNode->removeBaseEquipment();
                                    _inventory->player->removeEquipment("Ring");
                                }
                                _inventory->ringNode->setBaseEquipment(_equipmentName, _equipmentLevel);
                                _inventory->ringNode->setQuantity(1);
                                _inventory->equipment = _inventory->weaponNode->getBaseEquipment();
                                _inventory->player->setEquipment(_equipmentName, _equipmentLevel, "Ring");
                                _inventory->currentNode->removeBaseEquipment();
                            }
                            else if (_equipmentType == "Shield") {
                                if (_inventory->shieldNode->getBaseEquipment()) {
                                    _inventory->addEquipment(_inventory->shieldNode->getBaseEquipment()->getEquipmentName(), _inventory->shieldNode->getBaseEquipment()->getLevel(), 1);
                                    _inventory->shieldNode->removeBaseEquipment();
                                    _inventory->player->removeEquipment("Shield");
                                }
                                _inventory->shieldNode->setBaseEquipment(_equipmentName, _equipmentLevel);
                                _inventory->shieldNode->setQuantity(1);
                                _inventory->equipment = _inventory->weaponNode->getBaseEquipment();
                                _inventory->player->setEquipment(_equipmentName, _equipmentLevel, "Shield");
                                _inventory->currentNode->removeBaseEquipment();
                            }
                            else if (_equipmentType == "Helmet") {
                                if (_inventory->helmetNode->getBaseEquipment()) {
                                    _inventory->addEquipment(_inventory->helmetNode->getBaseEquipment()->getEquipmentName(), _inventory->helmetNode->getBaseEquipment()->getLevel(), 1);
                                    _inventory->helmetNode->removeBaseEquipment();
                                    _inventory->player->removeEquipment("Helmet");
                                }
                                _inventory->helmetNode->setBaseEquipment(_equipmentName, _equipmentLevel);
                                _inventory->helmetNode->setQuantity(1);
                                _inventory->equipment = _inventory->weaponNode->getBaseEquipment();
                                _inventory->player->setEquipment(_equipmentName, _equipmentLevel, "Helmet");
                                _inventory->currentNode->removeBaseEquipment();
                            }
                            else if (_equipmentType == "Armor") {
                                if (_inventory->armorNode->getBaseEquipment()) {
                                    _inventory->addEquipment(_inventory->armorNode->getBaseEquipment()->getEquipmentName(), _inventory->armorNode->getBaseEquipment()->getLevel(), 1);
                                    _inventory->armorNode->removeBaseEquipment();
                                    _inventory->player->removeEquipment("Armor");
                                }
                                _inventory->armorNode->setBaseEquipment(_equipmentName, _equipmentLevel);
                                _inventory->armorNode->setQuantity(1);
                                _inventory->equipment = _inventory->weaponNode->getBaseEquipment();
                                _inventory->player->setEquipment(_equipmentName, _equipmentLevel, "Armor");
                                _inventory->currentNode->removeBaseEquipment();
                            }
                            _inventory->equipment = nullptr;
                            _inventory->dmgStat->setString(" DMG: " + std::to_string(static_cast<int>(_inventory->player->getDamage() + _inventory->player->getEquipmentDamage())));
                            _inventory->mpStat->setString(" MP: " + std::to_string(static_cast<int>(_inventory->player->getMP() + _inventory->player->getEquipmentMP())));
                            _inventory->hpStat->setString(" HP: " + std::to_string(static_cast<int>(_inventory->player->getHP() + _inventory->player->getEquipmentHP())));
                            _inventory->msStat->setString(" Movement speed: " + std::to_string(static_cast<int>(_inventory->player->getMovementSpeed() + _inventory->player->getEquipmentMS())));
                            _inventory->cdrStat->setString(" Skill duration: " + std::to_string(static_cast<int>(_inventory->player->getCDR() + _inventory->player->getEquipmentCDR())));
                            _inventory->armStat->setString(" Armor: " + std::to_string(static_cast<int>(_inventory->player->getArmor() + _inventory->player->getEquipmentArmor())));
                            NotificationManager::getInstance()->showMessageNotification("Equip successfully!", Vec2::ZERO, Color3B(161, 251, 142), 17);
                        }
                        EquipmentDetails::hide();
                        return true;
                    }
                    else {
                       //  unequip item
                        if (_upgradeInventory) {
                            if (_upgradeInventory->weaponNode->getBaseEquipment()) {
                                _upgradeInventory->addEquipment(_upgradeInventory->weaponNode->getBaseEquipment()->getEquipmentName(), _upgradeInventory->weaponNode->getBaseEquipment()->getLevel(), 1);
                                _upgradeInventory->weaponNode->removeBaseEquipment();
                            }
                            _upgradeInventory->upgradeProgressBar->updateProgress(0, 100);
                            for (auto i : _upgradeInventory->materialNodes) {
                                if (i->getBaseItem()) {
                                    _upgradeInventory->addItem(i->getBaseItem()->getItemName(), i->getQuantity());
                                    i->removeBaseItem();
                                }
                                i->removeMaterialCondition();
                            }
                            _upgradeInventory->dmgStat->setString(" DMG  ");
                            _upgradeInventory->mpStat->setString(" MP  ");
                            _upgradeInventory->hpStat->setString(" HP  ");
                            _upgradeInventory->msStat->setString(" Movement speed  ");
                            _upgradeInventory->cdrStat->setString(" Skill duration  ");
                            _upgradeInventory->armStat->setString(" Armor  ");
                            _upgradeInventory->equipment = nullptr;
                            NotificationManager::getInstance()->showMessageNotification("Unequip successfully!", Vec2::ZERO, Color3B(161, 251, 142), 17);
                            EquipmentDetails::hide();
                            return true;
                        }
                        if (_inventory) {
                            if (_equipmentType == "Weapon") {
                                if (_inventory->weaponNode->getBaseEquipment()) {
                                    _inventory->addEquipment(_inventory->weaponNode->getBaseEquipment()->getEquipmentName(), _inventory->weaponNode->getBaseEquipment()->getLevel(), 1);
                                    _inventory->weaponNode->removeBaseEquipment();
                                    _inventory->player->removeEquipment("Weapon");
                                }
                            }
                            else if (_equipmentType == "Shoe") {
                                if (_inventory->shoeNode->getBaseEquipment()) {
                                    _inventory->addEquipment(_inventory->shoeNode->getBaseEquipment()->getEquipmentName(), _inventory->shoeNode->getBaseEquipment()->getLevel(), 1);
                                    _inventory->shoeNode->removeBaseEquipment();
                                    _inventory->player->removeEquipment("Shoe");
                                }
                            }
                            else if (_equipmentType == "Ring") {
                                if (_inventory->ringNode->getBaseEquipment()) {
                                    _inventory->addEquipment(_inventory->ringNode->getBaseEquipment()->getEquipmentName(), _inventory->ringNode->getBaseEquipment()->getLevel(), 1);
                                    _inventory->ringNode->removeBaseEquipment();
                                    _inventory->player->removeEquipment("Ring");
                                }
                            }
                            else if (_equipmentType == "Shield") {
                                if (_inventory->shieldNode->getBaseEquipment()) {
                                    _inventory->addEquipment(_inventory->shieldNode->getBaseEquipment()->getEquipmentName(), _inventory->shieldNode->getBaseEquipment()->getLevel(), 1);
                                    _inventory->shieldNode->removeBaseEquipment();
                                    _inventory->player->removeEquipment("Shield");
                                }
                            }
                            else if (_equipmentType == "Helmet") {
                                if (_inventory->helmetNode->getBaseEquipment()) {
                                    _inventory->addEquipment(_inventory->helmetNode->getBaseEquipment()->getEquipmentName(), _inventory->helmetNode->getBaseEquipment()->getLevel(), 1);
                                    _inventory->helmetNode->removeBaseEquipment();
                                    _inventory->player->removeEquipment("Helmet");
                                }
                            }
                            else if (_equipmentType == "Armor") {
                                if (_inventory->armorNode->getBaseEquipment()) {
                                    _inventory->addEquipment(_inventory->armorNode->getBaseEquipment()->getEquipmentName(), _inventory->armorNode->getBaseEquipment()->getLevel(), 1);
                                    _inventory->armorNode->removeBaseEquipment();
                                    _inventory->player->removeEquipment("Armor");
                                }

                            }
                            _inventory->equipment = nullptr;
                            _inventory->dmgStat->setString(" DMG: " + std::to_string(static_cast<int>(_inventory->player->getDamage() + _inventory->player->getEquipmentDamage())));
                            _inventory->mpStat->setString(" MP: " + std::to_string(static_cast<int>(_inventory->player->getMP() + _inventory->player->getEquipmentMP())));
                            _inventory->hpStat->setString(" HP: " + std::to_string(static_cast<int>(_inventory->player->getHP() + _inventory->player->getEquipmentHP())));
                            _inventory->msStat->setString(" Movement speed: " + std::to_string(static_cast<int>(_inventory->player->getMovementSpeed() + _inventory->player->getEquipmentMS())));
                            _inventory->cdrStat->setString(" Skill duration: " + std::to_string(static_cast<int>(_inventory->player->getCDR() + _inventory->player->getEquipmentCDR())));
                            _inventory->armStat->setString(" Armor: " + std::to_string(static_cast<int>(_inventory->player->getArmor() + _inventory->player->getEquipmentArmor())));
                            NotificationManager::getInstance()->showMessageNotification("Unequip successfully!", Vec2::ZERO, Color3B(161, 251, 142), 17);
                        }
                        EquipmentDetails::hide();
                        return true;
                    }
                }
                else if (dropButton && dropButton->getBoundingBox().containsPoint(startPoint)) {
                  //   drop item
                    if (_inventory) {


                        Scene* currentScene = Director::getInstance()->getRunningScene();
                        Game* game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));

                        if (game && game->getPlayer()) {
                            /*int round = Engine::getInstance()->randomNumber(0, 3);
                            int ran = Engine::getInstance()->randomNumber(15, 30);
                            int ran2 = Engine::getInstance()->randomNumber(15, 30);
                            if (round == 1) {
                                ran = -ran;
                            }
                            else if (round == 2) {
                                ran2 = -ran2;
                            } else {
                                ran = -ran;
                                ran2 = -ran2;
                            }*/
                            Vec2 dropPos = Vec2(30, 50);
                            auto droppedItem = EquipmentFactory::createEquipment(_equipmentName, _inventory->currentNode->getBaseEquipment()->getLevel());
                            if (droppedItem) {
                                droppedItem->showLootIndicator();
                                droppedItem->setPosition(game->getPlayer()->getPosition() + dropPos);
                                game->listOfEquipment.push_back(droppedItem);
                                game->addChild(droppedItem, 6.8);
                            }
                        }

                        if(_inventory && _inventory->currentNode && _inventory->currentNode->getBaseEquipment() != nullptr && _inventory->currentNode->getStatus() == "busy")
                            _inventory->currentNode->removeBaseEquipment();

                        if (_isPlayerUsing && _inventory->player) {
                            if(_inventory->player->getEquipment(_equipmentType))  
                                _inventory->player->removeEquipment(_equipmentType);
                            if (_inventory->player->getEquipment(_equipmentType)->getEquipmentName() == _equipmentName)
                                _inventory->player->dropEquipment(_equipmentName);
                            _inventory->dmgStat->setString(" DMG: " + std::to_string(static_cast<int>(_inventory->player->getDamage() + _inventory->player->getEquipmentDamage())));
                            _inventory->mpStat->setString(" MP: " + std::to_string(static_cast<int>(_inventory->player->getMP() + _inventory->player->getEquipmentMP())));
                            _inventory->hpStat->setString(" HP: " + std::to_string(static_cast<int>(_inventory->player->getHP() + _inventory->player->getEquipmentHP())));
                            _inventory->msStat->setString(" Movement speed: " + std::to_string(static_cast<int>(_inventory->player->getMovementSpeed() + _inventory->player->getEquipmentMS())));
                            _inventory->cdrStat->setString(" Skill duration: " + std::to_string(static_cast<int>(_inventory->player->getCDR() + _inventory->player->getEquipmentCDR())));
                            _inventory->armStat->setString(" Armor: " + std::to_string(static_cast<int>(_inventory->player->getArmor() + _inventory->player->getEquipmentArmor())));
                        }
                       // NotificationManager::getInstance()->showMessageNotification("Drop equipment successfully!", Vec2::ZERO, Color3B(161, 251, 142), 17);


                        EquipmentDetails::hide();
                        return true;
                    }
                }
                else {
                    EquipmentDetails::hide();
                    return true;
                }
            }
            else {
                EquipmentDetails::hide();
                return true;
            }
        }
        return false;
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);


	return true;
}

void EquipmentDetails::hide() {
    _inventory->overlay->setVisible(false);
	this->setVisible(false);
}

void EquipmentDetails::show() {
	this->setVisible(true);
}
