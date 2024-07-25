#ifndef NPC_H
#define NPC_H
#include "cocos2d.h"
#include "Player/Player.h"
#include "State/NPCState/NPCStateBase.h"
#include "State/NPCState/NPCIdle.h"
#include "State/NPCState/NPCTalking.h"
#include "Quest/Quest.h"
#include "Engine/Engine.h"
#include "DialogueManager/DialogueManager.h"

USING_NS_CC;
class NPC : public Node
{
public:
	NPCStateBase* currentState;
	NPCStateBase* idleState;
	NPCStateBase* talkingState;

	// Animation
	Animate* NPC_Idle_E;
	Animate* NPC_Idle_N;
	Animate* NPC_Idle_NE;
	Animate* NPC_Idle_NW;
	Animate* NPC_Idle_S;
	Animate* NPC_Idle_SE;
	Animate* NPC_Idle_SW;
	Animate* NPC_Idle_W;

	Animate* NPC_Select_E;
	Animate* NPC_Select_N;
	Animate* NPC_Select_NE;
	Animate* NPC_Select_NW;
	Animate* NPC_Select_S;
	Animate* NPC_Select_SE;
	Animate* NPC_Select_SW;
	Animate* NPC_Select_W;
	void Initalize(NPCStateBase* startingState)
	{
		currentState = startingState;
		currentState->EnterState();
	}
	void SwitchState(NPCStateBase* newState)
	{
		currentState->ExitState();
		currentState = newState;
		currentState->EnterState();
	}

	void detectPlayerCharacter();

	//bool onTouchBeganDialogue(Touch* touch, Event* event);
	float calculateAngle(const Vec2& vectorA, const Vec2& vectorB) {
		// Tnh gc gia hai vector s dng atan2
		float angle = vectorA.getAngle(vectorB);
		// Chuyn gc t radian sang 
		auto degree = CC_RADIANS_TO_DEGREES(angle);
		return degree;
	}

	void setTarget(Player* newTarget) {
		if (newTarget == nullptr) {
			/*CCLOG("No target");*/
			isTargetInInteractRange = false;
			if (target && target->getChildByName("InteractSprite"))target->removeChildByName("InteractSprite");
			DialogueManager::getInstance()->currentPage = 1;
			DialogueManager::getInstance()->hideMessageDialogue();
			return;
		}

		target = newTarget;
		if (target->getChildByName("InteractSprite"))target->removeChildByName("InteractSprite");
		if (!target->getChildByName("InteractSprite")) {
			target->addChild(interactSprite, 10);
			interactSprite->setPosition(Vec2(50, 0));
		}
		if (!interactSprite->isVisible() && !DialogueManager::getInstance()->isVisible()) {
			interactSprite->setVisible(true);
			interactSprite->setPosition(Vec2(50, 0));
		}
		isTargetInInteractRange = true;

		Vec2 direction = (target->getPosition() - this->getPosition()).getNormalized();

		float angleResult = calculateAngle(direction, Vec2(1, 0));

		if (angleResult >= -45 && angleResult < 0)
		{
			this->direction = 1;
		} // E
		if (angleResult >= -90 && angleResult < -45)
		{
			this->direction = 2;
		} // NE
		if (angleResult >= -135 && angleResult < -90)
		{
			this->direction = 3;
		} // N
		if (angleResult >= -180 && angleResult < -135)
		{
			this->direction = 4;
		} // NW
		if (angleResult >= 135 && angleResult < 180)
		{
			this->direction = 5;
		} // W
		if (angleResult >= 90 && angleResult < 135)
		{
			this->direction = 6;
		} // SW
		if (angleResult >= 45 && angleResult < 90)
		{
			this->direction = 7;
		} // S
		if (angleResult >= 0 && angleResult < 45)
		{
			this->direction = 8;
		} // SE
		this->SwitchState(this->idleState);

	}

	void updateTarget(float);

	virtual bool init();

	int direction;

	Sprite* getCurrentSprite() {
		return this->NPCCurrentSprite;
	}

	const int getCurrentDirection() {
		return direction;
	}
	//Dialogue
	std::vector<std::string> dialogue;

	Sprite* NPCCurrentSprite;

	//Interact Player

	float detectionRadius;

	Player* target = nullptr;

	bool isTargetInInteractRange = false;

	int isPlayerInteract = 0;

	int numberOfDialogue = 0;

	//Interact Sprite
	Sprite* interactSprite = nullptr;

	Sprite* talkSprite = nullptr;

	Sprite* upgradeSprite = nullptr;

	Label* talkLabel = nullptr;

	Label* upgradeLabel = nullptr;

	Label* nameLabel = nullptr;

	CREATE_FUNC(NPC);
};
#endif // !_NPC_H_
