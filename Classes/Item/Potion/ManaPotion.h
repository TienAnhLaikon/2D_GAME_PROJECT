#ifndef __MANA_POTION__
#define __MANA_POTION__

#include "cocos2d.h"
#include "../BaseItem.h"
USING_NS_CC;
class Game;
class ManaPotion : public BaseItem
{
public:
    virtual bool init();
    void use() override;
    CREATE_FUNC(ManaPotion);
};

#endif // __POTION__