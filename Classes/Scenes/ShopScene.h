#ifndef __SHOP_SCENE__
#define __SHOP_SCENE__

#include "cocos2d.h"

USING_NS_CC;
class Game;
class ShopScene : public cocos2d::Scene
{
public:
    static Scene *createScene();
    virtual bool init();
    void unpauseCallback(Ref *pSender);
    int clickButtonSoundId;
    Game* game;

    CREATE_FUNC(ShopScene);
};

#endif // __SPLASH_H__