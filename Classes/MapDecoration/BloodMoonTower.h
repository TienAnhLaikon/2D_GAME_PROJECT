#ifndef _BLOODMOONTOWER_H_
#define _BLOODMOONTOWER_H_
#include "cocos2d.h"
#include "Engine/Engine.h"

USING_NS_CC;

class BloodMoonTower : public cocos2d::Node
{
private:
	Animate *BloodMoonTowerAnimate;
	Sprite *BloodMoonTowerSprite;
	std::string BloodMoonTowerKnowledge;

public:
	bool init();
	static BloodMoonTower *createBloodMoonTower();
	void playBloodMoonTowerAnimation();
	CREATE_FUNC(BloodMoonTower);
};
#endif // !_BloodMoonTower_H
