#ifndef __MULTIFIREBALL_H__
#define __MULTIFIREBALL_H__

#include "cocos2d.h"
#include "BossSkills/BossSkillBase.h"
USING_NS_CC;

class MultiFireBall : public BossSkillBase {
public:
	virtual bool init() override;
	virtual void performSkill(Vec2 target) override;
	virtual void castRangeIndicator(Vec2 target) override;
	void update(float);
	CREATE_FUNC(MultiFireBall);
private:
	std::vector<Sprite*> _skillSprites;
	std::vector<Sprite*> _rangeIndicators;
	std::vector<Animate*> _animates;
};
#endif // !__FORTIFY_H__
