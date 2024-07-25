#ifndef __PLAYER_ATTACK_H__
#define __PLAYER_ATTACK_H__

#include "cocos2d.h"
#include "PlayerStateBase.h"
USING_NS_CC;

class PlayerAttack : public PlayerStateBase
{
public:
    PlayerAttack(Player* crt) : PlayerStateBase(crt) {}

    virtual void EnterState() override;
    virtual void UpdateState() override;
    virtual void ExitState() override;

    float calculateAngle(const Vec2& vectorA, const Vec2& vectorB) {
        // Tnh gc gia hai vector s dng atan2
        float angle = vectorA.getAngle(vectorB);

        // Chuyn gc t radian sang 
        auto degree = CC_RADIANS_TO_DEGREES(angle);


        return degree;
    }

private:
    int lastDir;
};

#endif 
