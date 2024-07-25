#ifndef __HEALTH_POTION__
#define __HEALTH_POTION__

#include "cocos2d.h"
#include "../BaseItem.h"
USING_NS_CC;
class Game;
class HealthPotion : public BaseItem
{
public:
    virtual bool init();
    void use() override;
    void updateCooldown(float dt);
    CREATE_FUNC(HealthPotion);
};

#endif // __POTION__