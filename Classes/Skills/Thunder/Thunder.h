#ifndef __THUNDER_H__
#define __THUNDER_H__

#include "cocos2d.h"
#include "Skills/SkillBase.h"
#include "Enemies/Enemy.h"
#include "AudioEngine.h"
#include "SettingsManager/SettingsData.h"
class Boss;

USING_NS_CC;

class Thunder : public SkillBase
{
public:
    int lightningSkillOnId;
    bool init();
    CREATE_FUNC(Thunder);
protected:
    float applyRange;
    Sprite* _targetSprite;
    std::vector<Enemy*> enemies;
    Vec2 dir;
    float lastAngleRotate;
};

#endif 
