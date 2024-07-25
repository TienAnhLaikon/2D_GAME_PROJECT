#ifndef __BOSS_SKILL_BASE_H__
#define __BOSS_SKILL_BASE_H__

#include "cocos2d.h"
#include "Engine/Engine.h"
USING_NS_CC;

class BossSkillBase : public Node {
public:

	virtual bool init() = 0;

	virtual void performSkill(Vec2 target) = 0;

	virtual void castRangeIndicator(Vec2 target) = 0;
protected:
	Sprite* _skillSprite;
	Sprite* _rangeIndicator;
	Animate* _skillAnimate;
	float skillDamage;
};
#endif // !__BOSS_SKILL_BASE_H__
