#include "InventoryButton.h"
#include "../../Inventory/Inventory.h"
#include "Audio/AudioEngine.h"
#include "SettingsManager/SettingsData.h"
#include "TutorialManager/TutorialManager.h"
#include "Game/Game.h"
InventoryButton* InventoryButton::create(Inventory* inventory) {
    auto node = new InventoryButton();
    if (node && node->init(inventory)) {
        node->autorelease();
        return node;
    }
    CC_SAFE_DELETE(node);
    return nullptr;
}

bool InventoryButton::init(Inventory* inventory) {
    if (!Node::init()) {
        return false;
    }

    inven = inventory;
    btnSprite = Sprite::create("res/action_bar_btn3.png");
    btnSprite->setScale(2);
    Sprite* iconSprite = Sprite::create("res/chest_icon.png");
    iconSprite->setScale(0.25 * Director::getInstance()->getContentScaleFactor());
    iconSprite->setPosition(Vec2(btnSprite->getContentSize().width / 2, btnSprite->getContentSize().height / 2));
    this->addChild(btnSprite);
    btnSprite->addChild(iconSprite);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(InventoryButton::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, btnSprite);
    return true;
}

bool InventoryButton::onTouchBegan(Touch* touch, Event* event) {
    if (isVisible() && btnSprite->isVisible()&& this->getParent()->isVisible() && UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0) >= 2) {
        if (btnSprite->getBoundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation()))) {
            
            UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
           /* Director::getInstance()->pause();
            Director::getInstance()->getRunningScene()->getPhysicsWorld()->setSpeed(0);*/
            
            
            if (inven->isShow()) {
                Scene* currentScene = Director::getInstance()->getRunningScene();
                Game* game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
                game->resumeGame();
                inven->hideInventory();
                inven->dmgStat->setString(" DMG: " + std::to_string(static_cast<int>(game->getPlayer()->getDamage() + game->getPlayer()->getEquipmentDamage())));
                inven->mpStat->setString(" MP: " + std::to_string(static_cast<int>(game->getPlayer()->getMP() + game->getPlayer()->getEquipmentMP())));
                inven->hpStat->setString(" HP: " + std::to_string(static_cast<int>(game->getPlayer()->getHP() + game->getPlayer()->getEquipmentHP())));
                inven->msStat->setString(" Movement speed: " + std::to_string(static_cast<int>(game->getPlayer()->getMS() + game->getPlayer()->getEquipmentMS())));
                inven->cdrStat->setString(" Skill duration: " + std::to_string(static_cast<int>(game->getPlayer()->getCDR() + game->getPlayer()->getEquipmentCDR())));
                inven->armStat->setString(" Armor: " + std::to_string(static_cast<int>(game->getPlayer()->getArmor() + game->getPlayer()->getEquipmentArmor())));
            }
            else {
                inven->showInventory();
                Scene* currentScene = Director::getInstance()->getRunningScene();
                Game* game = dynamic_cast<Game*>(currentScene->getChildByName("GameInstance"));
                game->pauseGame();
                inven->dmgStat->setString(" DMG: " + std::to_string(static_cast<int>(game->getPlayer()->getDamage() + game->getPlayer()->getEquipmentDamage())));
                inven->mpStat->setString(" MP: " + std::to_string(static_cast<int>(game->getPlayer()->getMP() + game->getPlayer()->getEquipmentMP())));
                inven->hpStat->setString(" HP: " + std::to_string(static_cast<int>(game->getPlayer()->getHP() + game->getPlayer()->getEquipmentHP())));
                inven->msStat->setString(" Movement speed: " + std::to_string(static_cast<int>(game->getPlayer()->getMS() + game->getPlayer()->getEquipmentMS())));
                inven->cdrStat->setString(" Skill duration: " + std::to_string(static_cast<int>(game->getPlayer()->getCDR() + game->getPlayer()->getEquipmentCDR())));
                inven->armStat->setString(" Armor: " + std::to_string(static_cast<int>(game->getPlayer()->getArmor() + game->getPlayer()->getEquipmentArmor())));

            }

            int currentTutorial = UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0);
            if (currentTutorial == 2) {
                TutorialManager::getInstance()->nextTutorial();
            }
            return true;
        }
        return false;
    }
    return false;
}
