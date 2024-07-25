#ifndef _OBELISK_H_
#define _OBELISK_H_
#include "cocos2d.h"
#include "Engine/Engine.h"

USING_NS_CC;

class Obelisk : public cocos2d::Node
{
private:
	Animate *ObeliskAnimate;
	Sprite *ObeliskSprite;
	std::string ObeliskKnowledge;
	
public:
	bool init();
	static Obelisk *createObelisk();
	void playObeliskAnimation();
	CREATE_FUNC(Obelisk);
};
#endif // !_Obelisk_H
