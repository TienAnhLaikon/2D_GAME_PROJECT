#ifndef __BURST_H__
#define __BURST_H__

#include "cocos2d.h"
#include "BossSkills/BossSkillBase.h"
USING_NS_CC;

class Burst : public BossSkillBase {
public:
	virtual bool init() override;
	virtual void performSkill(Vec2 target) override;
	virtual void castRangeIndicator(Vec2 target) override;
	CREATE_FUNC(Burst);
};
#endif // !__FORTIFY_H__
