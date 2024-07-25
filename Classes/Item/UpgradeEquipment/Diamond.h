#ifndef __DIAMOND__
#define __DIAMOND__

#include "cocos2d.h"
#include "../BaseItem.h"
USING_NS_CC;

class Diamond : public BaseItem
{
public:
    virtual bool init();
    CREATE_FUNC(Diamond);
};

#endif // __DIAMOND__