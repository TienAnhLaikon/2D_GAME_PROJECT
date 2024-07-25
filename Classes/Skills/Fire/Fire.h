#ifndef __FIRE_H__
#define __FIRE_H__

#include "cocos2d.h"
#include "Skills/SkillBase.h"
#include "Enemies/Enemy.h"
#include "AudioEngine.h"
#include "Audio/AudioEngine.h"
#include "SettingsManager/SettingsData.h"

class Boss;
USING_NS_CC;

class Fire : public SkillBase
{
public:
    bool init();
    CREATE_FUNC(Fire);
protected:
    int flamesSkillOnId;
    float applyRange;
    Sprite* _targetSprite;
    std::vector<Enemy*> enemies;
    Vec2 dir;
    float lastAngleRotate;
    
};

#endif 
