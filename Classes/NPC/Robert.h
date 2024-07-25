#ifndef  _ROBERT_H_
#define _ROBERT_H_
#include "cocos2d.h"
#include "NPC.h"
USING_NS_CC;
class Robert : public NPC {
public:
	virtual bool init();
	void loadAnimate();

	bool onTouchBeganToInteract(Touch* touch, Event* event);
	bool onTouchBeganToTalk(Touch* touch, Event* event);
	bool onTouchBeganToQuest(Touch* touch, Event* event);
	bool onTouchBeganToShop(Touch* touch, Event* event);


	CREATE_FUNC(Robert);
};
#endif // ! _ROBERT_H_
