#include "DialogueManager/DialogueManager.h"
#include "HelloWorldScene.h"
#include "UserInterface/InGame/InGameUI.h"

#include "Game/Game.h"
#include "Map/Map.h"
#include "Player/Player.h"

#include "NPC/NPC.h"
#include "TutorialManager/TutorialManager.h"
DialogueManager* DialogueManager::instance = nullptr;

bool DialogueManager::init() {
	// khi to label v spritebackground
	dialogueLabel = Label::createWithTTF("", "fonts/Diablo Light.ttf", 14);
	dialogueLabel->setTextColor(Color4B::WHITE);
	background = Sprite::create("res/box-chat.png");
	background->setScale(0.6 * Director::getInstance()->getContentScaleFactor());
	background->setPosition(-50, 0);

	if (currentNPCNameLabel != nullptr) currentNPCNameLabel->removeFromParentAndCleanup(true);
	currentNPCNameLabel = Label::createWithTTF("", "fonts/Diablo Light.ttf", 18);
	currentNPCNameLabel->setTextColor(Color4B::WHITE);
	currentNPCNameLabel->setPosition(Vec2(-180, 65));
	currentNPCNameLabel->retain();
	this->addChild(currentNPCNameLabel, 10);

	Size size = Director::getInstance()->getVisibleSize();
	dialogueLabel->setPosition(Vec2(background->getContentSize().width / 2, background->getContentSize().height / 2+ 10));
	auto scale = size.width / size.height;
	auto sizeHD4 = size.height / (4 * scale);

	if (dialogueLabel != nullptr) dialogueLabel->removeFromParentAndCleanup(true);
	dialogueLabel->setDimensions(background->getContentSize().width - 100, background->getContentSize().height);
	dialogueLabel->setHorizontalAlignment(TextHAlignment::CENTER);
	dialogueLabel->setVerticalAlignment(TextVAlignment::CENTER);
	background->addChild(dialogueLabel);

	//Next

	if (nextLabel != nullptr) nextLabel->removeFromParentAndCleanup(true);
	nextLabel = Label::createWithTTF("", "fonts/Diablo Light.ttf", 18);
	nextLabel->setColor(Color3B::WHITE);
	nextLabel->setString("Next");

	if (nextButton != nullptr) nextButton->removeFromParentAndCleanup(true);
	nextButton = Sprite::create();
	nextButton->setName("NextButton");
	nextButton->setTextureRect(Rect(0, 0, nextLabel->getContentSize().width, nextLabel->getContentSize().height));
	nextLabel->setPosition(Vec2(nextButton->getBoundingBox().size.width / 2, nextButton->getBoundingBox().size.height / 2));

	auto sqe = Sequence::create(FadeIn::create(0.5f), FadeOut::create(0.5f), nullptr);
	nextLabel->runAction(RepeatForever::create(sqe));

	nextButton->retain();
	nextLabel->retain();

	nextButton->setOpacity(0);
	nextButton->setPosition(75, -40);
	nextLabel->setPosition(75, -40);
	this->addChild(nextButton, 100);
	this->addChild(nextLabel, 100);
	
	//Skip
	if (skipLabel != nullptr) skipLabel->removeFromParentAndCleanup(true);
	skipLabel = Label::createWithTTF("", "fonts/Diablo Light.ttf", 22);
	skipLabel->setColor(Color3B::WHITE);
	skipLabel->setString("Skip");

	if (skipButton != nullptr) skipButton->removeFromParentAndCleanup(true);
	skipButton = Sprite::create();
	skipButton->setName("NextButton");
	skipButton->setTextureRect(Rect(0, 0, skipLabel->getContentSize().width, skipLabel->getContentSize().height));
	skipLabel->setPosition(Vec2(skipButton->getBoundingBox().size.width / 2, skipButton->getBoundingBox().size.height / 2));

	skipButton->setOpacity(0);
	skipButton->setPosition(250, 170);
	skipLabel->setPosition(250, 170);
	this->addChild(skipButton, 100);
	this->addChild(skipLabel, 100);

	//Cancel
	if (cancelLabel != nullptr) cancelLabel->removeFromParentAndCleanup(true);
	cancelLabel = Label::createWithTTF("", "fonts/Diablo Light.ttf", 14);
	cancelLabel->setColor(Color3B::WHITE);
	cancelLabel->setString("Cancel");

	if (cancelButton != nullptr) cancelButton->removeFromParentAndCleanup(true);
	cancelButton = Sprite::create("res/button-dialogue.png");
	cancelButton->setName("CancelButton");
	cancelButton->setScale(0.25 * Director::getInstance()->getContentScaleFactor());
	//cancelButton->setTextureRect(Rect(0, 0, cancelLabel->getContentSize().width, cancelLabel->getContentSize().height));
	cancelLabel->setPosition(Vec2(cancelButton->getBoundingBox().size.width / 2, cancelButton->getBoundingBox().size.height / 2));

	cancelButton->setPosition(217, -20);
	cancelLabel->setPosition(217, -20);
	this->addChild(cancelButton, 100);
	this->addChild(cancelLabel, 101);

	//Extra props
	currentPage = 1;

	cancelButton->retain();
	cancelLabel->retain();

	background->retain();
	dialogueLabel->retain();

	nextLabel->retain();
	nextButton->retain();

	skipButton->retain();
	skipLabel->retain();

	this->addChild(background);


	Scene* game = Director::getInstance()->getRunningScene();
	currentScene = dynamic_cast<Game*>(game->getChildByName("GameInstance"));

	auto touchListener1 = EventListenerTouchOneByOne::create();
	touchListener1->setSwallowTouches(true);
	touchListener1->onTouchBegan = CC_CALLBACK_2(DialogueManager::onTouchBeganToSkipButton, this);
	skipButton->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener1, skipButton);

	auto touchListener2 = EventListenerTouchOneByOne::create();
	touchListener2->setSwallowTouches(true);
	touchListener2->onTouchBegan = CC_CALLBACK_2(DialogueManager::onTouchBeganToNextButton, this);
	nextButton->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener2, nextButton);

	auto touchListener3 = EventListenerTouchOneByOne::create();
	touchListener3->setSwallowTouches(true);
	touchListener3->onTouchBegan = CC_CALLBACK_2(DialogueManager::onTouchBeganToCancelButton, this);
	cancelButton->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener3, cancelButton);

	return true;
}
void DialogueManager::setNPC(NPC* npc) {
	currentNPC = npc;
	currentNPCNameLabel->setString(currentNPC->getName());
}
void DialogueManager::showDialogue()
{
		if (QuestManager::getInstance()->currentQuest && QuestManager::getInstance()->currentQuest->npc == currentNPC) {
			if (QuestManager::getInstance()->currentQuestIndex == 2) {
				int currentTutorial = UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0);
				if (currentTutorial == 11) {
					TutorialManager::getInstance()->nextTutorial();
				}
			}
			//Xu li chua nhan
			if (currentPage > 0 && currentPage <= QuestManager::getInstance()->currentQuest->questDialogue.size() && QuestManager::getInstance()->currentQuest->questStatus == QuestManager::getInstance()->currentQuest->NOT_RECEIVED)
			{
				//Not Receive Quest Dialogue
				std::string textToShow = QuestManager::getInstance()->currentQuest->questDialogue[currentPage - 1]; // -1 v index bt u t 0
				numberOfDialogue = QuestManager::getInstance()->currentQuest->questDialogue.size() - 1;
				showMessageDialogue(textToShow);
			}
			//Xu li da nhan
			else if ((currentPage > 0 && currentPage <= QuestManager::getInstance()->currentQuest->processingQuestDialogue.size() && QuestManager::getInstance()->currentQuest->questStatus == QuestManager::getInstance()->currentQuest->RECEIVED))
			{
				//Received Quest Dialogue
				std::string textToShow = QuestManager::getInstance()->currentQuest->processingQuestDialogue[currentPage - 1]; // -1 v index bt u t 0
				numberOfDialogue = QuestManager::getInstance()->currentQuest->processingQuestDialogue.size() - 1;
				showMessageDialogue(textToShow);
			}
			//Xu li hoan thanh
			else if (currentPage > 0 && currentPage <= QuestManager::getInstance()->currentQuest->completedQuestDialogue.size() && QuestManager::getInstance()->currentQuest->questStatus == QuestManager::getInstance()->currentQuest->COMPLETED) {
				//Completed Quest Dialogue
				std::string textToShow = QuestManager::getInstance()->currentQuest->completedQuestDialogue[currentPage - 1]; // -1 v index bt u t 0
				numberOfDialogue = QuestManager::getInstance()->currentQuest->completedQuestDialogue.size() - 1;
				showMessageDialogue(textToShow);
			}
		}
	//Xu li hoi thoai mac dinh
	else {
		if (currentPage > 0 && currentPage <= currentNPC->dialogue.size()) {
			// Ly vn bn tng ng t mng v hin th n
			std::string textToShow = currentNPC->dialogue[currentPage - 1]; // -1 v index bt u t 0
			numberOfDialogue = currentNPC->dialogue.size() - 1;
			showMessageDialogue(textToShow);
		}
	}

}
bool DialogueManager::onTouchBegan(Touch* touch, Event* event) {
	//if (DialogueManager::getInstance()->isVisible()) {
	//	auto target = static_cast<Sprite*>(event->getCurrentTarget());
	//	Point locationInNode = target->convertToNodeSpace(touch->getLocation());
	//	Size s = target->getContentSize();
	//	Rect rect = Rect(0, 0, s.width, s.height);

	//	if (rect.containsPoint(locationInNode)) {
	//		if (InGameUI::getInstance(_player)->dialogueManager->isVisible() && currentNPC->isTargetInInteractRange) {
	//				if (QuestManager::getInstance()->currentQuest && QuestManager::getInstance()->currentQuest->npc == currentNPC && QuestManager::getInstance()->currentQuest->npc->isQuestDialogue) {
	//					//Xu li chua nhan
	//					if (currentPage >= QuestManager::getInstance()->currentQuest->questDialogue.size() && QuestManager::getInstance()->currentQuest->questStatus == QuestManager::getInstance()->currentQuest->NOT_RECEIVED)
	//					{
	//						// Dialogue
	//						currentPage = 1;
	//						hideMessageDialogue();
	//						// Quest
	//						QuestManager::getInstance()->currentQuest->questStatus = QuestManager::getInstance()->currentQuest->RECEIVED;
	//						QuestManager::getInstance()->addChild(QuestManager::getInstance()->currentQuest, 1);
	//						QuestManager::getInstance()->changeQuestIconOfNPC(QuestManager::getInstance()->currentQuest->questStatus);
	//						QuestManager::getInstance()->showAndHideNoQuest(false);
	//						QuestManager::getInstance()->setVisible(true);
	//						return true;
	//					}
	//					//Xu li da nhan 
	//					else if (currentPage >= QuestManager::getInstance()->currentQuest->processingQuestDialogue.size() && QuestManager::getInstance()->currentQuest->questStatus == QuestManager::getInstance()->currentQuest->RECEIVED)
	//					{
	//						// Dialogue
	//						currentPage = 1;
	//						hideMessageDialogue();
	//						return true;
	//					}

	//					else if (currentPage >= QuestManager::getInstance()->currentQuest->completedQuestDialogue.size() && QuestManager::getInstance()->currentQuest->questStatus == QuestManager::getInstance()->currentQuest->COMPLETED)
	//					{
	//						// Dialogue
	//						currentPage = 1;
	//						hideMessageDialogue();
	//						// Quest
	//						QuestManager::getInstance()->currentQuest->questStatus = QuestManager::getInstance()->currentQuest->COMPLETE_PENDING;
	//						currentNPC->target->gainExp(QuestManager::getInstance()->currentQuest->reward.exp);
	//						currentNPC->setTarget(_player);
	//						CCLOG("%f", QuestManager::getInstance()->currentQuest->reward.exp);
	//						QuestManager::getInstance()->resetOrderOfListQuest();
	//						QuestManager::getInstance()->removeChildByName("CurrentQuest");
	//						QuestManager::getInstance()->showAndHideNoQuest(true);
	//						return true;
	//					}

	//				}
	//			else {
	//				//No Quest
	//				if (currentPage >= currentNPC->dialogue.size()) {
	//					currentPage = 1;
	//					DialogueManager::getInstance()->hideMessageDialogue();
	//					return true;
	//				}
	//			}
	//			currentPage++;
	//			showDialogue();
	//			return true;
	//		}
	//	}
	//	return false;
	//}
	return false;
}
bool DialogueManager::onTouchBeganToNextButton(Touch* touch, Event* event) {
	if (DialogueManager::getInstance()->isVisible()) {
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		//currentPage = numberOfDialogue + 1;
		if (rect.containsPoint(locationInNode)) {
			UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
			if (InGameUI::getInstance(_player)->dialogueManager->isVisible() && currentNPC->isTargetInInteractRange) {
				if (QuestManager::getInstance()->currentQuest && QuestManager::getInstance()->currentQuest->npc == currentNPC) {
					//Xu li chua nhan
					if (currentPage >= QuestManager::getInstance()->currentQuest->questDialogue.size() && QuestManager::getInstance()->currentQuest->questStatus == QuestManager::getInstance()->currentQuest->NOT_RECEIVED)
					{
						// Dialogue
						currentPage = 1;
						hideMessageDialogue();

						int currentTutorial = UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0);
						if (currentTutorial == 10) {
							TutorialManager::getInstance()->nextTutorial();
						}

						// Quest
						QuestManager::getInstance()->currentQuest->questStatus = QuestManager::getInstance()->currentQuest->RECEIVED;
						QuestManager::getInstance()->addChild(QuestManager::getInstance()->currentQuest, 1);
						QuestManager::getInstance()->changeQuestIconOfNPC(QuestManager::getInstance()->currentQuest->questStatus);
						QuestManager::getInstance()->showAndHideNoQuest(false);
						QuestManager::getInstance()->detailDisplay();
						return true;
					}
					//Xu li da nhan 
					else if (currentPage >= QuestManager::getInstance()->currentQuest->processingQuestDialogue.size() && QuestManager::getInstance()->currentQuest->questStatus == QuestManager::getInstance()->currentQuest->RECEIVED)
					{
						// Dialogue
						currentPage = 1;
						hideMessageDialogue();
						return true;
					}

					else if (currentPage >= QuestManager::getInstance()->currentQuest->completedQuestDialogue.size() && QuestManager::getInstance()->currentQuest->questStatus == QuestManager::getInstance()->currentQuest->COMPLETED)
					{
						// Dialogue
						currentPage = 1;
						hideMessageDialogue();
						// Quest
						QuestManager::getInstance()->currentQuest->questStatus = QuestManager::getInstance()->currentQuest->COMPLETE_PENDING;
						currentNPC->target->gainExp(QuestManager::getInstance()->currentQuest->reward.exp);
						//currentNPC->setTarget(_player);
						CCLOG("%f", QuestManager::getInstance()->currentQuest->reward.exp);
						QuestManager::getInstance()->resetOrderOfListQuest();
						QuestManager::getInstance()->removeChildByName("CurrentQuest");
						QuestManager::getInstance()->showAndHideNoQuest(true);
						return true;
					}

				}

				else {
					//No Quest
					if (currentPage >= currentNPC->dialogue.size()) {
						currentPage = 1;
						DialogueManager::getInstance()->hideMessageDialogue();
						return true;
					}
				}
				currentPage++;
				showDialogue();
				return true;
			}
			return true;
		}
		return false;
	}
	return false;
}

bool DialogueManager::onTouchBeganToCancelButton(Touch* touch, Event* event) {
	if (DialogueManager::getInstance()->isVisible()) {
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);

		if (rect.containsPoint(locationInNode)) {
			currentPage = numberOfDialogue + 1;
			UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
			if (InGameUI::getInstance(_player)->dialogueManager->isVisible() && currentNPC->isTargetInInteractRange) {
				if (QuestManager::getInstance()->currentQuest && QuestManager::getInstance()->currentQuest->npc == currentNPC ) {
					//Xu li chua nhan
					if (currentPage >= QuestManager::getInstance()->currentQuest->questDialogue.size() && QuestManager::getInstance()->currentQuest->questStatus == QuestManager::getInstance()->currentQuest->NOT_RECEIVED)
					{
						// Dialogue
						currentPage = 1;
						hideMessageDialogue();
						return true;
					}
					//Xu li da nhan 
					else if (currentPage >= QuestManager::getInstance()->currentQuest->processingQuestDialogue.size() && QuestManager::getInstance()->currentQuest->questStatus == QuestManager::getInstance()->currentQuest->RECEIVED)
					{
						// Dialogue
						currentPage = 1;
						hideMessageDialogue();
						return true;
					}

					else if (currentPage >= QuestManager::getInstance()->currentQuest->completedQuestDialogue.size() && QuestManager::getInstance()->currentQuest->questStatus == QuestManager::getInstance()->currentQuest->COMPLETED)
					{
						// Dialogue
						currentPage = 1;
						hideMessageDialogue();
						return true;
					}

				}

				else {
					//No Quest
					if (currentPage >= currentNPC->dialogue.size()) {
						currentPage = 1;
						DialogueManager::getInstance()->hideMessageDialogue();
						return true;
					}
				}
				currentPage++;
				showDialogue();
				return true;
			}
			return true;
		}
		return false;
	}
	return false;
}

bool DialogueManager::onTouchBeganToSkipButton(Touch* touch, Event* event) {
	if (DialogueManager::getInstance()->isVisible()) {
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);

		if (rect.containsPoint(locationInNode)) {
			currentPage = numberOfDialogue + 1;
			UserDefault::getInstance()->setIntegerForKey("sound_effect", Audio::getInstance()->play2d("sound/sounds effect/click_button_sound.mp3", false, SettingsData::getInstance()->getSoundSlider() / 100.0f));
			if (InGameUI::getInstance(_player)->dialogueManager->isVisible() && currentNPC->isTargetInInteractRange) {
					if (QuestManager::getInstance()->currentQuest && QuestManager::getInstance()->currentQuest->npc == currentNPC) {
						//Xu li chua nhan
						if (currentPage >= QuestManager::getInstance()->currentQuest->questDialogue.size() && QuestManager::getInstance()->currentQuest->questStatus == QuestManager::getInstance()->currentQuest->NOT_RECEIVED)
						{
							int currentTutorial = UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0);
							if (currentTutorial == 10) {
								TutorialManager::getInstance()->nextTutorial();
							}
							// Dialogue
							currentPage = 1;
							hideMessageDialogue();
							// Quest
							QuestManager::getInstance()->currentQuest->questStatus = QuestManager::getInstance()->currentQuest->RECEIVED;
							QuestManager::getInstance()->addChild(QuestManager::getInstance()->currentQuest, 1);
							QuestManager::getInstance()->changeQuestIconOfNPC(QuestManager::getInstance()->currentQuest->questStatus);
							QuestManager::getInstance()->showAndHideNoQuest(false);
							QuestManager::getInstance()->detailDisplay();
							return true;
						}
						//Xu li da nhan 
						else if (currentPage >= QuestManager::getInstance()->currentQuest->processingQuestDialogue.size() && QuestManager::getInstance()->currentQuest->questStatus == QuestManager::getInstance()->currentQuest->RECEIVED)
						{
							// Dialogue
							currentPage = 1;
							hideMessageDialogue();
							return true;
						}

						else if (currentPage >= QuestManager::getInstance()->currentQuest->completedQuestDialogue.size() && QuestManager::getInstance()->currentQuest->questStatus == QuestManager::getInstance()->currentQuest->COMPLETED)
						{
							// Dialogue
							currentPage = 1;
							hideMessageDialogue();
							// Quest
							QuestManager::getInstance()->currentQuest->questStatus = QuestManager::getInstance()->currentQuest->COMPLETE_PENDING;
							currentNPC->target->gainExp(QuestManager::getInstance()->currentQuest->reward.exp);
							//currentNPC->setTarget(_player);
							CCLOG("%f", QuestManager::getInstance()->currentQuest->reward.exp);
							QuestManager::getInstance()->resetOrderOfListQuest();
							QuestManager::getInstance()->removeChildByName("CurrentQuest");
							QuestManager::getInstance()->showAndHideNoQuest(true);
							if (currentScene)
								currentScene->save();
							return true;
						}

					}
				
				else {
					//No Quest
					if (currentPage >= currentNPC->dialogue.size()) {
						currentPage = 1;
						DialogueManager::getInstance()->hideMessageDialogue();
						return true;
					}
				}
				currentPage++;
				showDialogue();
				return true;
			}
			return true;
		}
		return false;
	}
	return false;
}
void DialogueManager::showMessageDialogue(const std::string& message)
{

	InGameUI::getInstance(_player)->dialogueManager->setVisible(true);
	InGameUI::getInstance(_player)->dialogueManager->dialogueLabel->setString(message);
	InGameUI::getInstance(_player)->dialogueManager->background->setVisible(true);
	InGameUI::getInstance(_player)->dialogueManager->dialogueLabel->setVisible(true);

	InGameUI::getInstance(_player)->hideUI();

	//NPC Stuffs
	if(InGameUI::getInstance(_player)->dialogueManager->currentNPC->interactSprite->getParent())
		InGameUI::getInstance(_player)->dialogueManager->currentNPC->interactSprite->setVisible(false);
}

void DialogueManager::hideMessageDialogue()
{
	InGameUI::getInstance(_player)->dialogueManager->setVisible(false);
	InGameUI::getInstance(_player)->dialogueManager->background->setVisible(false);
	InGameUI::getInstance(_player)->dialogueManager->dialogueLabel->setVisible(false);

	InGameUI::getInstance(_player)->showUI();

	//NPC Stuffs
	if (InGameUI::getInstance(_player)->dialogueManager->currentNPC->interactSprite->getParent())
		InGameUI::getInstance(_player)->dialogueManager->currentNPC->interactSprite->setVisible(true);
}
DialogueManager::DialogueManager()
{
	if (currentScene)
	{
		auto children = currentScene->gameMap->getTiledMap()->getChildren();
		for (const auto& child : children)
		{
			// Kim tra xem i tng con c phi l Player khng
			auto player = dynamic_cast<Player*>(child);
			if (player)
			{
				_player = player;

			}
		}
	}
}

DialogueManager::~DialogueManager()
{
}
