#ifndef __DIALOGUE_MANAGER_H__
#define __DIALOGUE_MANAGER_H__
#include "cocos2d.h"
class Player;
class NPC;
class Game;
USING_NS_CC;

class DialogueManager : public Node
{
public:
	static DialogueManager* getInstance() {
		if (!instance)
		{
			instance = new DialogueManager();
		}
		return instance;
	}
	bool init();
	void showDialogue();
	void showMessageDialogue(const std::string& message);
	void hideMessageDialogue();
	bool onTouchBegan(Touch* touch, Event* event);
	bool onTouchBeganToSkipButton(Touch* touch, Event* event);
	bool onTouchBeganToNextButton(Touch* touch, Event* event);
	bool onTouchBeganToCancelButton(Touch* touch, Event* event);

	void setNPC(NPC* npc);

	Label* currentNPCNameLabel = nullptr;
	Label* dialogueLabel = nullptr; // Label  hin th thng bo
	Sprite* background = nullptr; // Sprite  hin th nn cho khung chat

	Sprite* skipButton = nullptr;
	Label* skipLabel = nullptr;

	Sprite* nextButton = nullptr;
	Label* nextLabel = nullptr;

	Sprite* cancelButton = nullptr;
	Label* cancelLabel = nullptr;

	int currentPage = 1;
	int numberOfDialogue = 0;
	NPC* currentNPC = nullptr;
private:
	Game* currentScene = nullptr;
	DialogueManager();
	~DialogueManager();
	static DialogueManager* instance;
	Player* _player = nullptr;
};

#endif // __ENGINE_H__