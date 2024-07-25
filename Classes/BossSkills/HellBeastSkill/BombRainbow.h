#ifndef __BOMBRAINBOW_H__
#define __BOMBRAINBOW_H__

#include "cocos2d.h"
#include "BossSkills/BossSkillBase.h"
USING_NS_CC;

class BombRainbow : public BossSkillBase {
public:
	virtual bool init() override;
	virtual void performSkill(Vec2 target) override;
	virtual void castRangeIndicator(Vec2 target) override;
	CREATE_FUNC(BombRainbow);
private:
	std::vector<Sprite*> _skillSprites;
	std::vector<Sprite*> _rangeIndicators;
	std::vector<Animate*> _animates;
};
#endif // !__FORTIFY_H__
