#ifndef __FORTIFY_H__
#define __FORTIFY_H__

#include "cocos2d.h"
#include "BossSkills/BossSkillBase.h"
USING_NS_CC;

class Fortify : public BossSkillBase {
public:
	virtual bool init() override;
	virtual void performSkill(Vec2 target) override;
	virtual void castRangeIndicator(Vec2 target) override;
	CREATE_FUNC(Fortify);
};
#endif // !__FORTIFY_H__
