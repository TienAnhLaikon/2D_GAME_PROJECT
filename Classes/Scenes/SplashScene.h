#ifndef __SPLASH_H__
#define __SPLASH_H__

#include "cocos2d.h"

USING_NS_CC;
class SplashScene : public cocos2d::Scene
{
public:
    static Scene *createScene();
    virtual bool init();
    CREATE_FUNC(SplashScene);
};

#endif // __SPLASH_H__