#ifndef _DURIN_H_
#define _DURIN_H_
#include "cocos2d.h"
#include "NPC.h"
USING_NS_CC;
class Durin : public NPC
{
public:
	virtual bool init();
	void loadAnimate();
	
	bool onTouchBeganToInteract(Touch* touch, Event* event);
	bool onTouchBeganToTalk(Touch* touch, Event* event);
	bool onTouchBeganToUpgrade(Touch* touch, Event* event);

	//std::vector<std::string> dialogue;

	//bool isQuestDialogue = false;

	//bool isTargetInInteractRange = false;

	//int isPlayerInteract = 0;

	//int numberOfDialogue = 0;

	//Player* target = nullptr;
	CREATE_FUNC(Durin);
};
#endif // ! _Durin_H_
