#include "./TutorialManager.h"
#include "UserInterface/InGame/InGameUI.h"
#include "UserInterface/InGame/InventoryButton.h"
#include "UserInterface/InGame/TalentButton.h"
#include "UserInterface/InGame/QuestButton.h"
#include "Inventory/Inventory.h"
#include "UserInterface/InGame/Joystick.h"
#include "Game/Game.h"
#include "Skills/SkillTree.h"
TutorialManager* TutorialManager::instance = nullptr;

void TutorialManager::initLayer(Layer* layer) {
	auto inGameUI = dynamic_cast<InGameUI*>(layer);

	Size size = Director::getInstance()->getVisibleSize();
	float scale = size.width / size.height * 2;
	auto sizeWD4 = size.width / 4;
	auto sizeHD4 = size.height / 4;

	// normal attack
	normalAttack = Node::create();
	normalAttack->setPosition(Vec2((sizeWD4 + 2) / scale, -sizeHD4 / scale) + Vec2(20, 5));
	auto normalAttackLabel = Label::createWithTTF("Excute normal attack", "fonts/Diablo Light.ttf", 13);
	auto normalAttackArrow = Sprite::create("res/yellow arrow1.png");
	normalAttackArrow->setScale(1.25 * Director::getInstance()->getContentScaleFactor());
	normalAttack->addChild(normalAttackLabel);
	normalAttack->addChild(normalAttackArrow);
	normalAttackArrow->setPosition(normalAttackLabel->getPosition() + Vec2(normalAttackLabel->getContentSize().width / 2 + normalAttackArrow->getContentSize().width / 2 - 20, -normalAttackLabel->getContentSize().height * 1.1));
	normalAttack->setVisible(false);

	// inventory 
	inventoryButton = Node::create();
	auto inventoryLabel = Label::createWithTTF("Press 'here' to open inventory!", "fonts/Diablo Light.ttf", 13);
	auto inventoryArror = Sprite::create("res/yellow arrow2.png");
	inventoryArror->setScale(1.25 * Director::getInstance()->getContentScaleFactor());
	inventoryButton->addChild(inventoryLabel);
	inventoryButton->addChild(inventoryArror);
	inventoryArror->setPosition(inventoryLabel->getPosition() + Vec2(-(inventoryLabel->getContentSize().width / 2 + inventoryArror->getContentSize().width / 2), inventoryLabel->getContentSize().height * 1.1));
	if (inGameUI->inventoryButton != nullptr) {
		inventoryButton->setPosition(inGameUI->inventoryButton->getPosition() + Vec2(inventoryLabel->getContentSize().width / 2 + inventoryArror->getContentSize().width / 2 + (inGameUI->inventoryButton->getContentSize().width * inGameUI->inventoryButton->getScale() / 2) + 10, ( - (inventoryLabel->getContentSize().height + inventoryArror->getContentSize().height)) - 20));
	}
	inventoryButton->setVisible(false);
	
	// talent 
	talentButton = Node::create();
	auto talentLabel = Label::createWithTTF("Press 'here' to open talent!", "fonts/Diablo Light.ttf", 13);
	auto talentArror = Sprite::create("res/yellow arrow2.png");
	talentArror->setScale(1.25 * Director::getInstance()->getContentScaleFactor());
	talentButton->addChild(talentLabel);
	talentButton->addChild(talentArror);
	talentArror->setPosition(talentLabel->getPosition() + Vec2(-(talentLabel->getContentSize().width / 2 + talentArror->getContentSize().width / 2), talentLabel->getContentSize().height * 1.1));
	if (inGameUI->talentButton != nullptr) {
		talentButton->setPosition(inGameUI->talentButton->getPosition() + Vec2(talentLabel->getContentSize().width / 2 + talentArror->getContentSize().width / 2 + (inGameUI->inventoryButton->getContentSize().width * inGameUI->inventoryButton->getScale() / 2) + 10, -(talentLabel->getContentSize().height + talentArror->getContentSize().height) - 20));
	}
	talentButton->setVisible(false);

	// movement 
	movement = Node::create();
	auto movementLabel = Label::createWithTTF("Hold and move on the left side of the screen", "fonts/Diablo Light.ttf", 13);
	auto movementArrow = Sprite::create("res/arrow.png");
	movementArrow->setScale(1.25 * Director::getInstance()->getContentScaleFactor());
	movement->addChild(movementLabel);
	movement->addChild(movementArrow);
	movementLabel->setPosition(20,50);
	movementArrow->setPosition(Vec2(-30, -movementArrow->getContentSize().height + 5));
	movement->setPosition(Vec2(-size.width / 4 + inGameUI->_joystick->getSize().width, -size.height / 2 + size.height / 4 + inGameUI->_joystick->getSize().height) + Vec2(movementLabel->getContentSize().width / 2 - 50, movementLabel->getContentSize().height + 60));
	movement->setVisible(false);

	// inventoryNode 
	inventoryNode = Node::create();
	auto invenNodeLabel = Label::createWithTTF("Press 'here' to view item details!", "fonts/Diablo Light.ttf", 13);
	inventoryNode->addChild(invenNodeLabel);
	if (inGameUI->inventory->getFirstNode() != nullptr) {
		inventoryNode->setPosition(inGameUI->inventory->getFirstNode()->getPosition() - Vec2(-inGameUI->inventory->getFirstNode()->getContentSize().width / 2, invenNodeLabel->getContentSize().height + inGameUI->inventory->getFirstNode()->getContentSize().height / 1.3));
	}
	inventoryNode->setVisible(false);

	// compass 
	compassNode = Node::create();
	auto compassLabel = Label::createWithTTF("Move in the direction of the arrow!", "fonts/Diablo Light.ttf", 13);
	compassNode->addChild(compassLabel);
	compassNode->setPosition(0, compassLabel->getContentSize().height / 2 + 30);
	compassNode->setVisible(false);

	// messageButtonNode 
	messageButtonNode = Node::create();
	auto msgLabel = Label::createWithTTF("Press to talk with NPC!", "fonts/Diablo Light.ttf", 13);
	auto msgArrow = Sprite::create("res/yellow arrow1.png");
	msgArrow->setScale(1.25 * Director::getInstance()->getContentScaleFactor());
	messageButtonNode->addChild(msgLabel);
	messageButtonNode->addChild(msgArrow);
	msgArrow->setPosition(msgLabel->getPosition() + Vec2(msgLabel->getContentSize().width / 2 + msgArrow->getContentSize().width / 2, -msgLabel->getContentSize().height * 1.1));
	messageButtonNode->setPosition(inGameUI->_listNPC[0]->interactSprite->getPosition() + Vec2(-msgArrow->getContentSize().width - 30, msgArrow->getContentSize().height / 2 + msgLabel->getContentSize().height / 2));
	messageButtonNode->setVisible(false);

	// talent 
	questButton = Node::create();
	auto questLabel = Label::createWithTTF("Press to open quest details!", "fonts/Diablo Light.ttf", 13);
	auto questArrow = Sprite::create("res/arrow4.png");
	questLabel->setPosition(-350, 25);
	questButton->addChild(questLabel);
	questButton->addChild(questArrow);
	questArrow->setPosition(questLabel->getPosition() + Vec2(-(questLabel->getContentSize().width / 2 + questArrow->getContentSize().width / 2) + 315 , questLabel->getContentSize().height - 15));
	if (inGameUI->questButton != nullptr) {
		questButton->setPosition(inGameUI->questButton->getPosition() + Vec2(questLabel->getContentSize().width / 2 + questArrow->getContentSize().width / 2 + (inGameUI->questButton->getContentSize().width * inGameUI->questButton->getScale() / 2), -(questLabel->getContentSize().height + questArrow->getContentSize().height)));
	}
	questButton->setVisible(false);

	// talent 
	upgradeButton = Node::create();
	auto upgradeButtonLabel = Label::createWithTTF("Press to upgrade!", "fonts/Diablo Light.ttf", 13);
	auto upgradeButtonArrow = Sprite::create("res/arrow2.png");
	upgradeButtonLabel->setPosition(0, 75);

	upgradeButton->addChild(upgradeButtonLabel);
	upgradeButton->addChild(upgradeButtonArrow);
	upgradeButtonArrow->setPosition(Vec2(0, upgradeButtonArrow->getContentSize().height / 2 + upgradeButtonLabel->getContentSize().height + 35));
	if (inGameUI->_listNPC[0] != nullptr) {
		Vec2 upgradeInWorldSpace = inGameUI->_listNPC[0]->upgradeSprite->convertToWorldSpace(inGameUI->_listNPC[0]->upgradeSprite->getPosition());
		Vec2 upgradeInNodeSpace = inGameUI->convertToNodeSpace(upgradeInWorldSpace) + Vec2(upgradeButtonArrow->getContentSize().width - 50, -upgradeButtonArrow->getContentSize().height - upgradeButtonLabel->getContentSize().height);
		upgradeButton->setPosition(upgradeInNodeSpace);
	}
	upgradeButton->setVisible(false);

	// talentinfo
	talentInfo = Node::create();
	auto talentInfoLabel = Label::createWithTTF("This is skill point to unlock skill or upgrade skill!", "fonts/Diablo Light.ttf", 13);
	auto talentInfoArrow = Sprite::create("res/arrow2.png");
	talentInfoLabel->setPosition(0,75);
	talentInfo->addChild(talentInfoLabel);
	talentInfo->addChild(talentInfoArrow);
	talentInfoArrow->setPosition(Vec2(0, talentInfoArrow->getContentSize().height / 2 + talentInfoLabel->getContentSize().height + 25));
	if (inGameUI != nullptr) {
		Vec2 talentInfoInWorldSpace = inGameUI->talentCarousel->skillTreeAoe->convertToWorldSpace(inGameUI->talentCarousel->skillTreeAoe->moneyLabel->getPosition());
		Vec2 talentInfoInNodeSpace = inGameUI->convertToNodeSpace(talentInfoInWorldSpace) + Vec2(0, -(talentInfoLabel->getContentSize().height + talentInfoArrow->getContentSize().height + 200));
		talentInfo->setPosition(talentInfoInNodeSpace);
	}
	talentInfo->setVisible(false);

	// talentinfo2
	talentInfo2 = Node::create();
	auto talentInfo2Label = Label::createWithTTF("Press to unlock or upgrade skill!", "fonts/Diablo Light.ttf", 13);
	auto talentInfo2Arrow = Sprite::create("res/arrow1.png");
	talentInfo2->addChild(talentInfo2Label);
	talentInfo2->addChild(talentInfo2Arrow);
	talentInfo2Arrow->setPosition(Vec2(0, talentInfo2Arrow->getContentSize().height / 2 + talentInfo2Label->getContentSize().height));
	if (inGameUI != nullptr) {
		Vec2 talentInfo2InWorldSpace = inGameUI->talentCarousel->skillTreeAoe->convertToWorldSpace(std::get<1>(inGameUI->talentCarousel->skillTreeAoe->skillRects[0])->getPosition());
		Vec2 talentInfo2InNodeSpace = inGameUI->convertToNodeSpace(talentInfo2InWorldSpace) + Vec2(0, -(talentInfo2Label->getContentSize().height + talentInfo2Arrow->getContentSize().height + 20));
		talentInfo2->setPosition(talentInfo2InNodeSpace);
	}
	talentInfo2->setVisible(false);

	// skilldetails
	skillDetails = Node::create();
	auto skillDetailsLabel = Label::createWithTTF("Required skill point to unlock!", "fonts/Diablo Light.ttf", 13);
	auto skillDetailsArrow = Sprite::create("res/arrow1.png");
	skillDetailsLabel->setPosition(Vec2(75,0));
	skillDetails->addChild(skillDetailsLabel);
	skillDetails->addChild(skillDetailsArrow);
	skillDetailsArrow->setPosition(Vec2(0, skillDetailsArrow->getContentSize().height / 2 + skillDetailsLabel->getContentSize().height));
	Vec2 skillDetailsInNodeSpace = inGameUI->talentCarousel->convertToWorldSpace(Vec2(170.6 / Director::getInstance()->getContentScaleFactor() - 100, 50 + 59.73 / Director::getInstance()->getContentScaleFactor())) + Vec2(skillDetailsArrow->getContentSize().width, 0);
	skillDetails->setPosition(skillDetailsInNodeSpace);
	skillDetails->setVisible(false);

	// skilldetails
	unlockSkillDetails = Node::create();
	auto unlockSkillDetailsLabel = Label::createWithTTF("Press to unlock skill!", "fonts/Diablo Light.ttf", 13);
	auto unlockSkillDetailsArrow = Sprite::create("res/yellow arrow1.png");
	unlockSkillDetailsArrow->setScale(1.25 * Director::getInstance()->getContentScaleFactor());
	unlockSkillDetails->addChild(unlockSkillDetailsLabel);
	unlockSkillDetails->addChild(unlockSkillDetailsArrow);
	unlockSkillDetailsArrow->setPosition(unlockSkillDetailsLabel->getPosition() + Vec2(unlockSkillDetailsLabel->getContentSize().width / 2 + unlockSkillDetailsArrow->getContentSize().width / 2, -unlockSkillDetailsLabel->getContentSize().height * 1.1));
	Vec2 unlockSkillDetailsInNodeSpace = inGameUI->talentCarousel->convertToWorldSpace(Vec2( 95 / Director::getInstance()->getContentScaleFactor(), -75)) + Vec2(skillDetailsArrow->getContentSize().width - 125, 0);
	unlockSkillDetails->setPosition(unlockSkillDetailsInNodeSpace);
	unlockSkillDetails->setVisible(false);

	// skilldetails
	yesOrNoUnlock = Node::create();
	auto yesOrNoUnlockLabel = Label::createWithTTF("", "fonts/Diablo Light.ttf", 13);
	auto yesOrNoUnlockArrow = Sprite::create("res/yellow arrow1.png");
	yesOrNoUnlockArrow->setScale(1.25 * Director::getInstance()->getContentScaleFactor());
	yesOrNoUnlock->addChild(yesOrNoUnlockLabel);
	yesOrNoUnlock->addChild(yesOrNoUnlockArrow);
	yesOrNoUnlockArrow->setPosition(yesOrNoUnlockLabel->getPosition() + Vec2(yesOrNoUnlockLabel->getContentSize().width / 2 + yesOrNoUnlockArrow->getContentSize().width / 2, -yesOrNoUnlockLabel->getContentSize().height * 1.1));
	Vec2 yesOrNoUnlockInNodeSpace = inGameUI->talentCarousel->convertToWorldSpace(Vec2(95 / Director::getInstance()->getContentScaleFactor(), 213.33 / Director::getInstance()->getContentScaleFactor())) + Vec2(yesOrNoUnlockArrow->getContentSize().width - 200, 0);
	yesOrNoUnlock->setPosition(yesOrNoUnlockInNodeSpace);
	yesOrNoUnlock->setVisible(false);

	// skilldetails
	equipSkillDetails = Node::create();
	auto equipSkillDetailsLabel = Label::createWithTTF("Press to equip skill!", "fonts/Diablo Light.ttf", 13);
	auto equipSkillDetailsArrow = Sprite::create("res/yellow arrow1.png");
	equipSkillDetailsLabel->setPosition(Vec2(-25,0));
	equipSkillDetailsArrow->setScale(1.25 * Director::getInstance()->getContentScaleFactor());
	equipSkillDetails->addChild(equipSkillDetailsLabel);
	equipSkillDetails->addChild(equipSkillDetailsArrow);
	equipSkillDetailsArrow->setPosition(equipSkillDetailsLabel->getPosition() + Vec2(equipSkillDetailsLabel->getContentSize().width / 2 + equipSkillDetailsArrow->getContentSize().width / 2, -equipSkillDetailsLabel->getContentSize().height * 1.1));
	Vec2 equipSkillDetailsInNodeSpace = inGameUI->talentCarousel->convertToWorldSpace(Vec2(95 / Director::getInstance()->getContentScaleFactor(), 213.33 / Director::getInstance()->getContentScaleFactor())) + Vec2(equipSkillDetailsArrow->getContentSize().width - 100, -100);
	equipSkillDetails->setPosition(equipSkillDetailsInNodeSpace);
	equipSkillDetails->setVisible(false);

	// skilldetails
	slotNode = Node::create();
	auto slotNodeLabel = Label::createWithTTF("Choose the slot for skill!", "fonts/Diablo Light.ttf", 13);
	auto slotNodeArrow = Sprite::create("res/yellow arrow1.png");
	slotNodeArrow->setScale(1.25 * Director::getInstance()->getContentScaleFactor());
	slotNode->addChild(slotNodeLabel);
	slotNode->addChild(slotNodeArrow);
	slotNodeArrow->setPosition(slotNodeLabel->getPosition() + Vec2(slotNodeLabel->getContentSize().width / 2 + slotNodeArrow->getContentSize().width / 2, -slotNodeLabel->getContentSize().height * 1.1));
	Vec2 slotNodeInNodeSpace = inGameUI->talentCarousel->convertToWorldSpace(Vec2(95 / Director::getInstance()->getContentScaleFactor(), 213.33 / Director::getInstance()->getContentScaleFactor())) + Vec2(slotNodeArrow->getContentSize().width , -125);
	slotNode->setPosition(slotNodeInNodeSpace);
	slotNode->setVisible(false);

	/*Node* talentInfo;
	Node* talentInfo2;
	Node* skillDetails;
	Node* unlockSkillDetails;
	Node* equipSkillDetails;*/

	inGameUI->addChild(movement, 6);
	inGameUI->addChild(normalAttack, 6);
	inGameUI->addChild(inventoryButton, 6);
	inGameUI->addChild(talentButton, 6);
	inGameUI->addChild(inventoryNode, 20000);
	inGameUI->addChild(compassNode, 6);
	inGameUI->addChild(messageButtonNode, 6);
	inGameUI->addChild(questButton, 6);
	inGameUI->addChild(upgradeButton, 6);
	inGameUI->addChild(talentInfo, 6);
	inGameUI->addChild(talentInfo2, 6);
	inGameUI->addChild(skillDetails, 6);
	inGameUI->addChild(unlockSkillDetails, 6);
	//inGameUI->addChild(yesOrNoUnlock, 6);
	inGameUI->addChild(equipSkillDetails, 6);
	inGameUI->addChild(slotNode, 6);
	hideTutorial();
}

void TutorialManager::showTutorial(int id) {
	currentTutorial = id;
	hideTutorial();
	if (id == 0) {
		movement->setVisible(true);
	}
	else if (id == 1) {
		normalAttack->setVisible(true);
	}
	else if (id == 2) {
		inventoryButton->setVisible(true);
	}
	else if (id == 3) {
		inventoryNode->setVisible(true);
	}
	else if (id == 4) {
		talentButton->setVisible(true);
	}
	else if (id == 5) {
		talentInfo2->setVisible(true);
	}
	else if (id == 6) {
		// skillDetails->setVisible(true);
		unlockSkillDetails->setVisible(true);
	}
	else if (id == 7) {
		//yesOrNoUnlock->setVisible(true);
	}
	else if (id == 8) {
		slotNode->setVisible(true);
	}
	else if (id == 9) {
		compassNode->setVisible(true);
	}
	else if (id == 10) {
		messageButtonNode->setVisible(true);
	}
	else if (id == 11) {
		//questButton->setVisible(true);
	}
	else if (id == 12) {
	}
	else if (id == 13) { 
		upgradeButton->setVisible(true);
	}
	else {
		currentTutorial = 500;
	}
	UserDefault::getInstance()->setIntegerForKey("current_tutorial", id);
}

void TutorialManager::nextTutorial() {
	int nextTutorial = currentTutorial + 1;
	showTutorial(nextTutorial);
}

void TutorialManager::prevTutorial() {
	int prevTutorial = currentTutorial - 1;
	showTutorial(prevTutorial);
}

void TutorialManager::hideTutorial() {
	movement->setVisible(false);
	normalAttack->setVisible(false);
	inventoryButton->setVisible(false);
	talentButton->setVisible(false);
	inventoryNode->setVisible(false);
	compassNode->setVisible(false);
	messageButtonNode->setVisible(false);
	questButton->setVisible(false);
	upgradeButton->setVisible(false);
	talentInfo->setVisible(false);
	talentInfo2->setVisible(false);
	skillDetails->setVisible(false);
	unlockSkillDetails->setVisible(false);
	//yesOrNoUnlock->setVisible(false);
	equipSkillDetails->setVisible(false);
	slotNode->setVisible(false);
}

TutorialManager::TutorialManager()
{
}

TutorialManager::~TutorialManager()
{
}
