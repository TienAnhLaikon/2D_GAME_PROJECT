#ifndef __SPEED_POTION__
#define __SPEED_POTION__

#include "cocos2d.h"
#include "../BaseItem.h"
USING_NS_CC;

class SpeedPotion : public BaseItem
{
public:
    virtual bool init();
    void use() override;
    void updateCooldown(float dt);
    CREATE_FUNC(SpeedPotion);
};

#endif // __POTION__