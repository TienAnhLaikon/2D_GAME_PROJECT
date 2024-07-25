#ifndef __POTION__
#define __POTION__

#include "cocos2d.h"
#include "../BaseItem.h"
USING_NS_CC;

class Crystal : public BaseItem
{
public:
    virtual bool init();
    CREATE_FUNC(Crystal);
};

#endif // __POTION__