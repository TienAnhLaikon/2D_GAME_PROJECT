#ifndef __SCROLL_PAPER__
#define __SCROLL_PAPER__

#include "cocos2d.h"
#include "../BaseItem.h"
USING_NS_CC;

class ScrollPaper : public BaseItem
{
public:
    virtual bool init();
    void use() override;
    void updateCooldown(float dt);
    CREATE_FUNC(ScrollPaper);
};

#endif // __POTION__