#include "InGameUI.h"
#include "Player/Player.h"
#include "BaseCharacter/BaseCharacter.h"
#include "Enemies/Enemy.h"
#include "UserInterface/InGame/Joystick.h"
#include "UserInterface/InGame/Minimap.h"
#include "Inventory/Inventory.h"
#include "Inventory/UpgradeInventory.h"
#include "UserInterface/InGame/Joystick.h"
#include "MapDecoration/MapDecoration.h"
#include "UserInterface/InGame/TalentCarousel.h"
#include "UserInterface/InGame/TalentButton.h"
#include "UserInterface/InGame/InventoryButton.h"
#include "UserInterface/InGame/UpgradeInventoryButton.h"
#include "UserInterface/InGame/QuestButton.h"
#include "UserInterface/InGame/Dropdown.h"
#include "SettingsManager/HamburgerMenu.h"
#include "UserInterface/InGame/HamburgerMenuButton.h"
#include "UserInterface/InGame/ExpBar.h"
#include "UserInterface/InGame/HealthBar.h"
#include "UserInterface/InGame/ShowQuestButton.h"
#include "NPC/NPC.h"
#include "LevelUpPopup.h"
#include "Quest/QuestManager.h"
#include "DialogueManager/DialogueManager.h"
#include "TutorialManager/TutorialManager.h"
#include "Item/PotionManager.h"
#include "UserInterface/InGame/SelectMapButton.h"
#include "Scenes/SelectMapLayer.h"
#include "Scenes/AnimaticScene.h"
InGameUI* InGameUI::instance = nullptr;
InGameUI::InGameUI(BaseCharacter* player) : _player(player)
{
    init(player);
}

bool InGameUI::init(BaseCharacter* plr)
{
    if (!Layer::init())
    {
        return false;
    }

    setTarget(dynamic_cast<Player*>(plr));
    return true;
}

void InGameUI::setTarget(Player* plr) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Player* player = plr;


    if (healthBar != nullptr) healthBar->removeFromParentAndCleanup(true);
    if (expBar != nullptr) expBar->removeFromParentAndCleanup(true);
    if (inventory != nullptr) inventory->removeFromParentAndCleanup(true);
    if (inventoryButton != nullptr) inventoryButton->removeFromParentAndCleanup(true);
    if (upgradeInventory != nullptr) upgradeInventory->removeFromParentAndCleanup(true);
    if (upgradeInventoryButton != nullptr) upgradeInventoryButton->removeFromParentAndCleanup(true);
    if (questButton != nullptr) questButton->removeFromParentAndCleanup(true);
    if (hamburgerButton != nullptr) hamburgerButton->removeFromParentAndCleanup(true);
    if (_joystick != nullptr) { _joystick->removeFromParentAndCleanup(true); _joystick = nullptr; }
    if (talentCarousel != nullptr) talentCarousel->removeFromParentAndCleanup(true);
    if (talentButton != nullptr) talentButton->removeFromParentAndCleanup(true);
    if (subInventory != nullptr) subInventory->removeFromParentAndCleanup(true);
    if (dialogueManager != nullptr) dialogueManager->removeFromParentAndCleanup(true);
    if (levelUpPopup != nullptr) levelUpPopup->removeFromParentAndCleanup(true);
    if (potionManager != nullptr) potionManager->removeFromParentAndCleanup(true);
    if (selectMapLayer != nullptr) selectMapLayer->removeFromParentAndCleanup(true);
    if (selectMapButton != nullptr) selectMapButton->removeFromParentAndCleanup(true);
    if (storyScene != nullptr) storyScene->removeFromParentAndCleanup(true);
    if (dropdownButton != nullptr) dropdownButton->removeFromParentAndCleanup(true);
    if (questBtn != nullptr) questBtn->removeFromParentAndCleanup(true);

    if (this->getChildByName("SkillButton4")) {
        this->getChildByName("SkillButton4")->removeFromParentAndCleanup(true);
        this->removeChildByName("SkillButton4");
    }
    if (this->getChildByName("SkillButton3")) {
        this->getChildByName("SkillButton3")->removeFromParentAndCleanup(true);
        this->removeChildByName("SkillButton3");
    }
    if (this->getChildByName("SkillButton2")) {
        this->getChildByName("SkillButton2")->removeFromParentAndCleanup(true);
        this->removeChildByName("SkillButton2");
    }
    if (this->getChildByName("SkillButton1")) {
        this->getChildByName("SkillButton1")->removeFromParentAndCleanup(true);
        this->removeChildByName("SkillButton1");
    }

    _player_temp = player;
    _player_temp->retain();

    for (auto skill : _player_temp->skills) {
        if (skill->getSkillIndex() != 0)
            this->addChild(skill->_skillButton);
    }
    _player_temp->resetSkillSlotOrder();

    healthBar = HealthBar::create("res/health-bar-border.png", "res/health-bar-inside.png", player->getHP(), player->getMP());
    healthBar->setPosition(Vec2(0, -visibleSize.height / 2 + visibleSize.height / 4 + healthBar->getHeight() / 2) + Vec2(0, 10 * Director::getInstance()->getContentScaleFactor()));
    healthBar->setScale(0.25 * Director::getInstance()->getContentScaleFactor());
    this->addChild(healthBar, 5);

    expBar = ExpBar::create();
    expBar->setPosition(healthBar->getPosition() + Vec2(0, -((healthBar->getHeight() + 10 * Director::getInstance()->getContentScaleFactor()) / 2 + expBar->getContentSize().height * 4)));
    if(player)
        expBar->updateExpBar(player->getCurrentExp(), player->getExpRequiredToLevelUp());
    expBar->setScale(2);
    this->addChild(expBar, 2);

    inventory = player->getInventory();
    this->addChild(inventory, 20);

    subInventory = player->getSubInventory();
    subInventory->setInventory(inventory);
    subInventory->setPosition(Vec2(0, -visibleSize.height / 2 + visibleSize.height / 4 + healthBar->getHeight() / 2) + Vec2(0, 10 * Director::getInstance()->getContentScaleFactor()));
    subInventory->setScale(0.25 * Director::getInstance()->getContentScaleFactor());
    this->addChild(subInventory, 1);


    inventory->setSubInventory(subInventory);

    upgradeInventory = player->getUpgradeInventory();
    upgradeInventory->setScale(visibleSize.width / upgradeInventory->bg->getContentSize().width / 2, visibleSize.height / upgradeInventory->bg->getContentSize().height / 2);
    this->addChild(upgradeInventory, 20);

    dropdownButton = Dropdown::create(this);
    dropdownButton->setPosition(visibleSize.width / 4 - 80, visibleSize.height / 5 - 5 - dropdownButton->bg->getContentSize().height * dropdownButton->bg->getScaleY() / 1.6);
    this->addChild(dropdownButton, 3);
    

    upgradeInventoryButton = UpgradeInventoryButton::create(upgradeInventory);
    upgradeInventoryButton->setPosition(Vec2(-visibleSize.width / 4 + 130, visibleSize.height / 5 - 5));
    upgradeInventoryButton->setScale(0.15 * Director::getInstance()->getContentScaleFactor());
    upgradeInventoryButton->setVisible(false);
    this->addChild(upgradeInventoryButton, 6);

    questBtn = QuestBtn::create();
    questBtn->setPosition(dropdownButton->getPosition() - Vec2(dropdownButton->bg->getContentSize().width * dropdownButton->bg->getScaleX() / 2, -questBtn->btnSprite->getContentSize().height / 4 - 8));
    questBtn->setScale(0.35 * Director::getInstance()->getContentScaleFactor());
    this->addChild(questBtn, 6);

    questButton = QuestButton::create();
    questButton->setPosition(visibleSize.width / 4 - 60, visibleSize.height / 5 - 50);
    questButton->setScale(0.17 * Director::getInstance()->getContentScaleFactor());
    this->addChild(questButton, 2);
    questButton->retain();

    selectMapLayer = SelectMapLayer::create();
    selectMapLayer->setScale(visibleSize.width / selectMapLayer->_background->getContentSize().width / 2, visibleSize.height / selectMapLayer->_background->getContentSize().height / 2);
    this->addChild(selectMapLayer, 50);

    selectMapButton = SelectMapButton::create(selectMapLayer);
    selectMapButton->setPosition(visibleSize.width / 4 - 100 * Director::getInstance()->getContentScaleFactor(), -visibleSize.height / 5 + 66 * Director::getInstance()->getContentScaleFactor());
    selectMapButton->setScale(0.25 * Director::getInstance()->getContentScaleFactor());
    this->addChild(selectMapButton, 6);

    hamburgerMenu = HamburgerMenu::create();
    this->addChild(hamburgerMenu, 20);

    hamburgerButton = HamburgerMenuButton::create(hamburgerMenu);
    hamburgerButton->setPosition(Vec2(visibleSize.width / 4 - 55, visibleSize.height / 5 - 10));
    //hamburgerButton->setScale(0.25 * Director::getInstance()->getContentScaleFactor());
    this->addChild(hamburgerButton, 6);

    _joystick = Joystick::create();
    _joystick->setPosition(Vec2(-visibleSize.width / 4 + _joystick->getSize().width, -visibleSize.height / 2 + visibleSize.height / 4 + _joystick->getSize().height));
    _joystick->setScale(0.8 * Director::getInstance()->getContentScaleFactor());
    this->addChild(_joystick);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(false);
    touchListener->onTouchBegan = [&](Touch* touch, Event* event) {
        auto startPoint = this->convertToNodeSpace(touch->getLocation());
        
        if (startPoint.x < talentButton->getPositionX() - 200) {
            _joystick->setPosition(startPoint);
            return true;
        }

        return false;
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    levelUpPopup = LevelUpPopup::create(_player->getLevel());
    levelUpPopup->setPosition(0, visibleSize.height / 5);
    this->addChild(levelUpPopup);

    talentCarousel = TalentCarousel::create(player);
    talentCarousel->setScale(visibleSize.width / talentCarousel->talentBorder->getContentSize().width / 2, visibleSize.height / talentCarousel->talentBorder->getContentSize().height / 2);
    talentCarousel->setPosition(Vec2());
    this->addChild(talentCarousel, 6);

    talentButton = TalentButton::create(talentCarousel);
    talentButton->setPosition(questBtn->getPosition() - Vec2(questBtn->btnSprite->getContentSize().width * questBtn->btnSprite->getScaleX() / 2, 0));
    //inventoryButton->setPosition(questBtn->getPosition() - Vec2(questBtn->btnSprite->getContentSize().width * questBtn->btnSprite->getScaleX() / 3, 0));
    talentButton->setScale(0.35 * Director::getInstance()->getContentScaleFactor());
    this->addChild(talentButton, 5);

    inventoryButton = InventoryButton::create(inventory);
    inventoryButton->setPosition(talentButton->getPosition() - Vec2(questBtn->btnSprite->getContentSize().width * questBtn->btnSprite->getScaleX() / 2, 0));
    //talentButton->setPosition(inventoryButton->getPosition() - Vec2(questBtn->btnSprite->getContentSize().width * questBtn->btnSprite->getScaleX() / 3, 0));
    inventoryButton->setScale(0.35 * Director::getInstance()->getContentScaleFactor());
    this->addChild(inventoryButton, 5);
    inventoryButton->retain();

    DialogueManager::getInstance()->init();
    dialogueManager = DialogueManager::getInstance();
    dialogueManager->setPosition(Vec2(0, -60));
    dialogueManager->setVisible(false);
    this->addChild(dialogueManager);

    potionManager = PotionManager::create();
    potionManager->setPosition(Vec2(-visibleSize.width / 4 + 20, 0));
    potionManager->setAnchorPoint(Vec2(0, 0.5));
    this->addChild(potionManager, 5);

    if (TutorialManager::getInstance()->currentTutorial == 0) {
        storyScene = AnimaticScene::create();
        storyScene->setScale(visibleSize.width / storyScene->_background->getContentSize().width / 4, visibleSize.height / storyScene->_background->getContentSize().height / 4);
        this->addChild(storyScene, 50);
        storyScene->show();
    }
    talentButton->setVisible(false);
    inventoryButton->setVisible(false);
}

void InGameUI::update(float dt) {

}

void InGameUI::showLevelUpPopup() {
   // NotificationManager::getInstance()->showMessageNotification("Level Up\n" + std::to_string(_player->getLevel()), Vec2::ZERO, Color3B::GREEN, 20);
}

void InGameUI::setNPCs(std::vector<NPC*> listNPC) {
    if (questManager != nullptr) {
        QuestManager::getInstance()->destroyInstance();
    }

    _listNPC = listNPC;
    NPC* durin = nullptr;
    for (auto npc : listNPC) {
        if (npc->getName() == "Durin") {
            durin = npc;
            break;
        }
   }
    if (durin) {
        dialogueManager->setNPC(durin);
        durin->upgradeSprite->setPosition(Vec2(dialogueManager->background->getBoundingBox().size.width / 2 +50, 30));
        durin->upgradeSprite->setName("UpgradeSprite");
        dialogueManager->addChild(durin->upgradeSprite, 10);
    }
    QuestManager::getInstance()->init(_listNPC);
    QuestManager::getInstance()->generalDisplay();
    questManager = QuestManager::getInstance();

    this->addChild(questManager, 5);

    int currentTutorial = UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0);
    if (currentTutorial != 500) {
        tutorialManager = TutorialManager::getInstance();
        tutorialManager->initLayer(this);
        if (currentTutorial != 3 && !(currentTutorial >= 5 && currentTutorial <= 8) && currentTutorial != 10 && currentTutorial != 13) {
            tutorialManager->showTutorial(currentTutorial);
        } 
        else {
            if (currentTutorial == 3) {
                tutorialManager->showTutorial(2);
            }
            if (currentTutorial >= 5 && currentTutorial <= 9) {
                tutorialManager->showTutorial(4);
            }
            if (currentTutorial == 9) {
                tutorialManager->showTutorial(8);
            }
            if (currentTutorial == 10) {
                tutorialManager->showTutorial(9);
                tutorialManager->hideTutorial();
            }
            if (currentTutorial == 12) {
                tutorialManager->showTutorial(11);
            }
        }
    }
}

void InGameUI::hideUI() {
    //Hide Healthbar
    healthBar->setVisible(false);
    
    //Hide Exp and Level
    expBar->setVisible(false);
    
    //Hide subiventory
    subInventory->setVisible(false);
    

    
    //Hide PauseGame
    hamburgerButton->setVisible(false);
    
    //Hide QuestButton
    questButton->setVisible(false);
    
    if (dropdownButton->isOpen == true) {
        //Hide inventory button
        inventoryButton->setVisible(false);
        //Hide Skill Tree
        talentButton->setVisible(false);
    }
    //Hide Player's Skills
    _player_temp->setVisibleSkillButton(false);
    
    //Hide QuestManager
    questManager->setVisible(false);

    questBtn->setVisible(false);

    dropdownButton->setVisible(false);
}

void InGameUI::showUI() {
    //Show Healthbar
    healthBar->setVisible(true);
    
    //Show Exp and Level
    expBar->setVisible(true);
    
    //Show subiventory
    subInventory->setVisible(true);
    
    if (dropdownButton->isOpen) {
        //Hide inventory button
        inventoryButton->setVisible(true);
        //Hide Skill Tree
        talentButton->setVisible(true);
    }
    
    //Show PauseGame
    hamburgerButton->setVisible(true);
    
    //Show QuestButton
    questButton->setVisible(true);
    
    //Show Player's Skills
    _player_temp->setVisibleSkillButton(true);
   
    //Show QuestManager
    questManager->setVisible(true);

    questBtn->setVisible(true);

    dropdownButton->setVisible(true);
}