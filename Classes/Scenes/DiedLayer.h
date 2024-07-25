#ifndef __DIED_LAYER__
#define __DIED_LAYER__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
class Game;
class DiedLayer : public cocos2d::Layer
{
public:
    static DiedLayer* createDiedLayer();
    virtual bool init();
    float currentPercent = 0;
    bool onTouchBegan(Touch* touch, Event* event) { return true; }
    Label* continueLabel;
    void tabToContinue();
    CREATE_FUNC(DiedLayer);
};

#endif // __SPLASH_H__