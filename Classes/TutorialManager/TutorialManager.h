#ifndef __TUTORIAL_MANAGER__
#define __TUTORIAL_MANAGER__
#include "cocos2d.h"
USING_NS_CC;

class InGameUI;
class TutorialManager
{
public:
	static TutorialManager* getInstance()
	{
		if (!instance)
		{
			instance = new TutorialManager();
		}
		return instance;
	}

	void destroyInstance() {
		if (instance) {
			delete instance;
			instance = nullptr;
		}
	}

	void showTutorial(int id);
	void hideTutorial();
	void initLayer(Layer* layer);
	void nextTutorial();
	void prevTutorial();
	int currentTutorial = UserDefault::getInstance()->getIntegerForKey("current_tutorial", 0);
private:
	InGameUI* inGameUI;
	Node* movement;
	Node* normalAttack;
	Node* inventoryButton;
	Node* inventoryNode;
	Node* talentButton;
	Node* talentInfo;
	Node* talentInfo2;
	Node* skillDetails;
	Node* unlockSkillDetails;
	Node* yesOrNoUnlock;
	Node* equipSkillDetails;
	Node* slotNode;
	Node* compassNode;
	Node* messageButtonNode;
	Node* receiveQuest;
	Node* questButton;
	Node* upgradeButton;
	Node* upgradeNode;
	TutorialManager();
	~TutorialManager();
	static TutorialManager* instance;
};

#endif // __TUTORIAL_MANAGER__