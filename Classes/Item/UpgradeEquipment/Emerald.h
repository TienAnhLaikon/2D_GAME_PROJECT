#ifndef __EMERALD__
#define __EMERALD__

#include "cocos2d.h"
#include "../BaseItem.h"
USING_NS_CC;

class Emerald : public BaseItem
{
public:
    virtual bool init();
    CREATE_FUNC(Emerald);
};

#endif // __EMERALD__