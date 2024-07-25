#ifndef __TELEPORT_EFFECT__
#define __TELEPORT_EFFECT__

#include "cocos2d.h"
USING_NS_CC;
class TeleportEffect : public cocos2d::Sprite
{
public:
    virtual bool init();
    CREATE_FUNC(TeleportEffect);
};

#endif // __TELEPORT_EFFECT__